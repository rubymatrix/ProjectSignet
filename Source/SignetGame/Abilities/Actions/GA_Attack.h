// Copyright Red Lotus Games, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "SignetGame/Abilities/State/StateAbility.h"
#include "SignetGame/Combat/CombatTypes.h"
#include "GA_Attack.generated.h"

UCLASS()
class SIGNETGAME_API UGA_Attack : public UStateAbility
{
	GENERATED_BODY()

public:

	UGA_Attack();

protected:

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle,
								 const FGameplayAbilityActorInfo* ActorInfo,
								 const FGameplayAbilityActivationInfo ActivationInfo,
								 const FGameplayEventData* TriggerEventData) override;

	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle,
							const FGameplayAbilityActorInfo* ActorInfo,
							const FGameplayAbilityActivationInfo ActivationInfo,
							bool bReplicateEndAbility, bool bWasCancelled) override;

private:

	void PlayNextMontage();
	void TriggerImpactEffects();
	void TriggerVocals();
	void TriggerWhooshEffects();

	UFUNCTION()
	void OnCurrentMontageCompleted();

	UFUNCTION()
	void OnCurrentMontageInterruptedOrCancelled();

	TArray<FAttackResult> CalculateAttackResults(AActor* Target);
	
	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	TSubclassOf<UGameplayEffect> DamageEffectClass;

	UPROPERTY()
	TArray<UAnimMontage*> AttackMontages;

	int32 CurrentMontageIndex = 0;

	FTimerHandle ImpactTimerHandle;
	FTimerHandle VocalsTimerHandle;
	FTimerHandle WhooshTimerHandle;
};
