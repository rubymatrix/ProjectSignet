// Copyright Red Lotus Games, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GSCGameplayAbility.h"
#include "SignetGame/Abilities/ActionTypes.h"
#include "StateAbility.generated.h"

class UAbilityTask_PlayMontageAndWait;
class UAnimMontage;
class UActionData;

UCLASS()
class SIGNETGAME_API UStateAbility : public UGSCGameplayAbility
{
	GENERATED_BODY()

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Settings")
	bool bTransitionBeforeMontage = false;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Settings")
	bool bTransitionState = false;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Settings")
	FGameplayTag TargetStateTag = FGameplayTag::EmptyTag;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Settings")
	EMontageType ActionType = EMontageType::None;

public:

	UStateAbility();

protected:
	
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle,
								 const FGameplayAbilityActorInfo* ActorInfo,
								 const FGameplayAbilityActivationInfo ActivationInfo,
								 const FGameplayEventData* TriggerEventData) override;

	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle,
							const FGameplayAbilityActorInfo* ActorInfo,
							const FGameplayAbilityActivationInfo ActivationInfo,
							bool bReplicateEndAbility, bool bWasCancelled) override;
	
	
	UFUNCTION()
	virtual void OnMontageCompleted();

	UFUNCTION()
	virtual void OnMontageInterruptedOrCancelled();


	UFUNCTION()
	virtual void OnMontageBlendIn();
	
	UFUNCTION()
	virtual void OnMontageBlendOut();

private:

	bool TryStateTransition() const;
	
	UPROPERTY()
	TObjectPtr<UAbilityTask_PlayMontageAndWait> MontageTask;

	UAnimMontage* GetAnimMontage() const;
};
