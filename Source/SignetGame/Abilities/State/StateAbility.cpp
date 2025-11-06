// Copyright Red Lotus Games, All Rights Reserved.

#include "StateAbility.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "SignetGame/Abilities/SignetAbilitySystemComponent.h"

UStateAbility::UStateAbility()
{
	InstancingPolicy   = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::LocalPredicted;
}


void UStateAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                    const FGameplayAbilityActorInfo* ActorInfo,
                                    const FGameplayAbilityActivationInfo ActivationInfo,
                                    const FGameplayEventData* TriggerEventData)
{
	if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	const auto Montage = GetAnimMontage();
	
	if (!Montage)
	{
		bool bTrans = TryStateTransition();
		EndAbility(Handle, ActorInfo, ActivationInfo, true, !bTrans);
		return;
	}

	MontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, NAME_None, Montage, 1.0f, NAME_None, true, 1.0f);

	if (!MontageTask)
	{
		const bool bTrans = TryStateTransition();
		EndAbility(Handle, ActorInfo, ActivationInfo, true, !bTrans);
		return;
	}

	MontageTask->OnBlendedIn.AddDynamic(this, &UStateAbility::OnMontageBlendIn);
	MontageTask->OnCompleted.AddDynamic(this, &UStateAbility::OnMontageCompleted);
	MontageTask->OnInterrupted.AddDynamic(this, &UStateAbility::OnMontageInterruptedOrCancelled);
	MontageTask->OnCancelled.AddDynamic(this, &UStateAbility::OnMontageInterruptedOrCancelled);
	MontageTask->OnBlendOut.AddDynamic(this, &UStateAbility::OnMontageBlendOut);
	MontageTask->ReadyForActivation();
}

void UStateAbility::EndAbility(const FGameplayAbilitySpecHandle Handle,
                               const FGameplayAbilityActorInfo* ActorInfo,
                               const FGameplayAbilityActivationInfo ActivationInfo,
                               bool bReplicateEndAbility, bool bWasCancelled)
{
	if (MontageTask)
	{
		MontageTask->EndTask();
		MontageTask = nullptr;
	}
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UStateAbility::OnMontageCompleted()
{
}

void UStateAbility::OnMontageBlendIn()
{
	if (bTransitionBeforeMontage)
		TryStateTransition();
}

void UStateAbility::OnMontageBlendOut()
{
	if (bTransitionBeforeMontage)
	{
		// Already transitioned
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
		return;
	}
	
	bool bTrans = TryStateTransition();
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, !bTrans);
}

bool UStateAbility::TryStateTransition() const
{
	if (!CurrentActorInfo || !CurrentActorInfo->AbilitySystemComponent.IsValid())
		return false;

	if (!bTransitionState)
		return true;
	
	if (const auto DragonAsc = Cast<USignetAbilitySystemComponent>(CurrentActorInfo->AbilitySystemComponent.Get()))
	{
		return DragonAsc->TrySetState(TargetStateTag, const_cast<UStateAbility*>(this));
	}

	return false;
}


void UStateAbility::OnMontageInterruptedOrCancelled()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
}



UAnimMontage* UStateAbility::GetAnimMontage() const
{
	if (!CurrentActorInfo || !CurrentActorInfo->AbilitySystemComponent.IsValid())
		return nullptr;

	// if (const auto ActionComp = CurrentActorInfo->AvatarActor->FindComponentByClass<UActionComponent>())
	// {
	// 	return ActionComp->GetMontageForAction(ActionType);
	// }

	return nullptr;
}

