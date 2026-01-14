// Copyright Red Lotus Games, All Rights Reserved.


#include "SignetAbilitySystemComponent.h"

#include "TagCache.h"
#include "SignetGame/Player/Components/TargetingComponent.h"
#include "StateEffects/GE_State_Casting.h"
#include "StateEffects/GE_State_Crafting.h"
#include "StateEffects/GE_State_Cutscene.h"
#include "StateEffects/GE_State_Dead.h"
#include "StateEffects/GE_State_Engaged.h"
#include "StateEffects/GE_State_Idle.h"
#include "StateEffects/GE_State_Resting.h"
#include "StateEffects/GE_State_Sitting.h"
#include "StateEffects/GE_State_Stunned.h"
#include "State/GA_Attack.h"

USignetAbilitySystemComponent::USignetAbilitySystemComponent()
{
	StateGameplayEffects.Add(FTagCache::Get().State.Idle, UGE_State_Idle::StaticClass());
	StateGameplayEffects.Add(FTagCache::Get().State.Engaged, UGE_State_Engaged::StaticClass());
	StateGameplayEffects.Add(FTagCache::Get().State.Casting, UGE_State_Casting::StaticClass());
	StateGameplayEffects.Add(FTagCache::Get().State.Resting, UGE_State_Resting::StaticClass());
	StateGameplayEffects.Add(FTagCache::Get().State.Sitting, UGE_State_Sitting::StaticClass());
	StateGameplayEffects.Add(FTagCache::Get().State.Dead, UGE_State_Dead::StaticClass());
	StateGameplayEffects.Add(FTagCache::Get().State.Stunned, UGE_State_Stunned::StaticClass());
	StateGameplayEffects.Add(FTagCache::Get().State.Cutscene, UGE_State_Cutscene::StaticClass());
	StateGameplayEffects.Add(FTagCache::Get().State.Crafting, UGE_State_Crafting::StaticClass());
}

void USignetAbilitySystemComponent::BeginPlay()
{
	Super::BeginPlay();

	BindStateGuards();

	if (!CurrentStateTag.IsValid())
	{
		ForceSetState(FTagCache::Get().State.Idle);
	}
}

void USignetAbilitySystemComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	CurrentStateTagChangedDelegate.RemoveAll(this);
}

bool USignetAbilitySystemComponent::HasStatusEffect(const FGameplayTag& EffectTag) const
{
	if (!EffectTag.IsValid())
	{
		UE_LOG(LogTemp, Warning, TEXT("HasStatusEffect: Invalid EffectTag"));
		return false;
	}
	
	return HasMatchingGameplayTag(EffectTag);
}

bool USignetAbilitySystemComponent::HasAnyEffectsWithTags(const FGameplayTagContainer& Tags) const
{
	return HasAnyMatchingGameplayTags(Tags);
}

bool USignetAbilitySystemComponent::IsInState(const FGameplayTag& StateTag) const
{
	return StateTag.IsValid() && HasMatchingGameplayTag(StateTag);
}

FGameplayTag USignetAbilitySystemComponent::GetCurrentState() const
{
	return CurrentStateTag.IsValid() ? CurrentStateTag : FTagCache::Get().State.Idle;
}

bool USignetAbilitySystemComponent::TrySetState(const FGameplayTag NewState, UGameplayAbility* SourceAbility)
{
	if (!NewState.IsValid() || !NewState.MatchesTag(FTagCache::Get().State.Root))
	{
		UE_LOG(LogTemp, Warning, TEXT("TrySetState: %s is not a State.* tag"), *NewState.ToString());
		return false;
	}
	if (NewState == CurrentStateTag)
		return true;
	
	ClearCurrentStateGE();
	if (!ApplyStateGE(NewState))
		return false;

	CurrentStateTag = NewState;

	// Auto-attack management
	// if (IsInState(FTagCache::Get().State.Engaged) && !IsAutoAttacking())
	// {
	// 	if (const auto TargetComp = GetOwner()->GetComponentByClass<UTargetSystemComponent>())
	// 	{
	// 		SetAutoAttackTarget(TargetComp->GetLockedOnTargetActor());
	// 	}
	// }
	// else if (!IsInState(FTagCache::Get().State.Engaged) && IsAutoAttacking())
	// {
	// 	StopAutoAttackLoop();
	// }
	
	return true;
}

void USignetAbilitySystemComponent::ForceSetState(const FGameplayTag NewState)
{
	ClearCurrentStateGE();

	if (ApplyStateGE(NewState))
	{
		CurrentStateTag = NewState;
	}
	else
	{
		if (NewState != FTagCache::Get().State.Idle)
		{
			ApplyStateGE(FTagCache::Get().State.Idle);
			CurrentStateTag = FTagCache::Get().State.Idle;
		}
	}
}

void USignetAbilitySystemComponent::ClientActivateAbilityFailed_Implementation(
	FGameplayAbilitySpecHandle AbilityToActivate, int16 PredictionKey)
{
	Super::ClientActivateAbilityFailed_Implementation(AbilityToActivate, PredictionKey);

	
}

void USignetAbilitySystemComponent::ClearCurrentStateGE()
{
	if (CurrentStateHandle.IsValid())
	{
		RemoveActiveGameplayEffect(CurrentStateHandle);
		CurrentStateHandle.Invalidate();
	}
}

bool USignetAbilitySystemComponent::ApplyStateGE(const FGameplayTag StateTag)
{
	auto* EffectClass = StateGameplayEffects.Find(StateTag);
	if (!EffectClass || !*EffectClass)
	{
		UE_LOG(LogTemp, Warning, TEXT("ApplyStateGE: No GameplayEffect found for state tag %s"), *StateTag.ToString());
		return false;
	}

	auto Ctx = MakeEffectContext();
	Ctx.AddSourceObject(this);
	auto Spec = MakeOutgoingSpec(*EffectClass, 1.f, Ctx);
	if (!Spec.IsValid()) return false;

	CurrentStateHandle = ApplyGameplayEffectSpecToSelf(*Spec.Data.Get());
	return CurrentStateHandle.IsValid();
}

void USignetAbilitySystemComponent::BindStateGuards()
{
	CurrentStateTagChangedDelegate = RegisterGameplayTagEvent(FTagCache::Get().State.Root, EGameplayTagEventType::AnyCountChange);
	CurrentStateTagChangedDelegate.AddUObject(this, &USignetAbilitySystemComponent::OnAnyStateTagChanged);
}

void USignetAbilitySystemComponent::OnAnyStateTagChanged(const FGameplayTag Tag, int32 NewCount)
{
	if (Tag.MatchesTag(FTagCache::Get().State.Root))
	{
		if (!CurrentStateTag.IsValid())
		{
			ForceSetState(FTagCache::Get().State.Idle);
		}
		else
		{
			ForceSetState(CurrentStateTag);
		}

		
	}
}

void USignetAbilitySystemComponent::SetAutoAttackTarget(AActor* NewTarget)
{
	AutoAttackTarget = NewTarget;
	if (IsInState(FTagCache::Get().State.Engaged))
	{
		StartAutoAttackLoop();
	}
}

AActor* USignetAbilitySystemComponent::GetAutoAttackTarget() const
{
	if (const auto Actor = GetAvatarActor())
	{
		if (const auto Targeting = Actor->GetComponentByClass<UTargetingComponent>())
		{
			return Targeting->GetLockedOnTargetActor();
		}
	}
	return AutoAttackTarget.IsValid() ? AutoAttackTarget.Get() : nullptr;
}

void USignetAbilitySystemComponent::StartAutoAttackLoop()
{
	if (!GetWorld()) return;
	if (!GetWorld()->GetTimerManager().IsTimerActive(AutoAttackLoopTimer))
	{
		GetWorld()->GetTimerManager().SetTimer(AutoAttackLoopTimer, this, &USignetAbilitySystemComponent::AutoAttackTick, 0.1f, true);
	}
}

void USignetAbilitySystemComponent::StopAutoAttackLoop()
{
	if (!GetWorld()) return;
	GetWorld()->GetTimerManager().ClearTimer(AutoAttackLoopTimer);
}

bool USignetAbilitySystemComponent::IsAutoAttacking() const
{
	return GetWorld() && GetWorld()->GetTimerManager().IsTimerActive(AutoAttackLoopTimer);
}

void USignetAbilitySystemComponent::AutoAttackTick()
{
	// Disengage if we no longer have a valid target
	if (!IsTargetValidAndAlive() && IsInState(FTagCache::Get().State.Engaged))
	{
		// if (const auto ActionComp = GetOwner()->GetComponentByClass<UActionComponent>())
		// {
		// 	ActionComp->Disengage();
		// }
	}

	if (!IsAutoAttackAllowed()) return;
	if (!IsTargetInRange()) return;
	if (!IsTargetInFacingCone()) return;

	TryPerformAutoAttack();
}

void USignetAbilitySystemComponent::TryPerformAutoAttack()
{
	TryActivateAbilityByClass(UGA_Attack::StaticClass(), true);
}

bool USignetAbilitySystemComponent::IsAutoAttackAllowed() const
{
	if (!IsInState(FTagCache::Get().State.Engaged)) return false;

	FGameplayTagContainer BlockingTags;
	BlockingTags.AddTag(FTagCache::Get().State.Dead);
	BlockingTags.AddTag(FTagCache::Get().State.Stunned);
	BlockingTags.AddTag(FTagCache::Get().State.Crafting);
	BlockingTags.AddTag(FTagCache::Get().State.Cutscene);
	BlockingTags.AddTag(FTagCache::Get().State.Mounted);
	
	if (HasAnyMatchingGameplayTags(BlockingTags)) return false;
	
	return true;
}

bool USignetAbilitySystemComponent::IsTargetValidAndAlive() const
{
	const AActor* Target = AutoAttackTarget.Get();
	if (!Target) return false;
	if (Target->IsPendingKillPending()) return false;

	// if (const auto EntityComp = Target->GetComponentByClass<UEntityComponent>())
	// {
	// 	return EntityComp->IsAlive();
	// }
	
	return true;
}

bool USignetAbilitySystemComponent::IsTargetInRange() const
{
	const AActor* Owner = GetAvatarActor();
	const AActor* Target = AutoAttackTarget.Get();
	if (!Owner || !Target) return false;

	const float DistSquared = FVector::DistSquared(Owner->GetActorLocation(), Target->GetActorLocation());
	const float Range  = GetAutoAttackRangeCm();

	return DistSquared <= (Range * Range);
}

bool USignetAbilitySystemComponent::IsTargetInFacingCone() const
{
	const AActor* Owner = GetAvatarActor();
	const AActor* Target = AutoAttackTarget.Get();
	if (!Owner || !Target) return false;

	auto ToTarget = Target->GetActorLocation() - Owner->GetActorLocation();
	ToTarget.Z = 0.f;

	const FVector Dir = ToTarget.GetSafeNormal();
	const FVector Fwd = Owner->GetActorForwardVector().GetSafeNormal();

	return FVector::DotProduct(Fwd, Dir) >= 0.f;
}

float USignetAbilitySystemComponent::GetAttackDelaySeconds() const
{
	// TODO:
	return 5.f; 
}

float USignetAbilitySystemComponent::GetAutoAttackRangeCm() const
{
	return 300.f;
	// bool bFound = false;
	// auto AttackRange = GetGameplayAttributeValue(UDragonAttributeSet_Combat::GetMeleeRangeAttribute(), bFound);
	// if (AttackRange <= 0.f || !bFound) AttackRange = 200.f;
	// return AttackRange;
}

