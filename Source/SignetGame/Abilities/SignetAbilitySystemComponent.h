// Copyright Red Lotus Games, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GSCAbilitySystemComponent.h"
#include "SignetAbilitySystemComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class SIGNETGAME_API USignetAbilitySystemComponent : public UGSCAbilitySystemComponent
{
	GENERATED_BODY()

public:
	
	USignetAbilitySystemComponent();

protected:

	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	
public:

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Ability System")
	bool HasStatusEffect(const FGameplayTag& EffectTag) const;
	bool HasAnyEffectsWithTags(const FGameplayTagContainer& Tags) const;
	bool IsInState(const FGameplayTag& StateTag) const;


	// -- Begin State Management System
	// Used to prevent the Avatar from having multiple active states (e.g., "Stunned" and "Sleeping" at the same time)
	
	UPROPERTY(EditDefaultsOnly, Category = "States")
	TMap<FGameplayTag, TSubclassOf<UGameplayEffect>> StateGameplayEffects;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "States")
	FGameplayTag GetCurrentState() const;

	UFUNCTION(BlueprintCallable, Category = "States")
	bool TrySetState(const FGameplayTag NewState, UGameplayAbility* SourceAbility = nullptr);

	UFUNCTION(BlueprintCallable, Category = "States")
	void ForceSetState(const FGameplayTag NewState);

protected:

	virtual void ClientActivateAbilityFailed_Implementation(FGameplayAbilitySpecHandle AbilityToActivate, int16 PredictionKey) override;

private:

	
	FGameplayTag CurrentStateTag;
	FActiveGameplayEffectHandle CurrentStateHandle;
	FOnGameplayEffectTagCountChanged CurrentStateTagChangedDelegate;

	void ClearCurrentStateGE();
	bool ApplyStateGE(const FGameplayTag StateTag);
	void BindStateGuards();
	void OnAnyStateTagChanged(const FGameplayTag Tag, int32 NewCount);


	// --- BEGIN AUTO ATTACK SYSTEM

public:

	void SetAutoAttackTarget(AActor* NewTarget);
	AActor* GetAutoAttackTarget() const;
	void StartAutoAttackLoop();
	void StopAutoAttackLoop();
	bool IsAutoAttacking() const;

private:

	void AutoAttackTick();
	void TryPerformAutoAttack();

	bool IsAutoAttackAllowed() const;
	bool IsTargetValidAndAlive() const;
	bool IsTargetInRange() const;
	bool IsTargetInFacingCone() const;
	float GetAttackDelaySeconds() const;
	float GetAutoAttackRangeCm() const;


	TWeakObjectPtr<AActor> AutoAttackTarget;
	FTimerHandle AutoAttackLoopTimer;
	FTimerHandle AttackCooldownTimer;
	bool bAttackOnCooldown = false;

};
