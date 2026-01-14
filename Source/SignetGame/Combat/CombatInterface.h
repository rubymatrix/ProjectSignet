// Copyright Red Lotus Games, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/CameraShakeComponent.h"
#include "UObject/Interface.h"
#include "CombatInterface.generated.h"

struct FAttackResult;
enum class EVocalizationType : uint8;

// This class does not need to be modified.
UINTERFACE(NotBlueprintable)
class UCombatInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class ICombatInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	UFUNCTION(BlueprintCallable)
	virtual float GetHealth() { return 0.0f; }
	UFUNCTION(BlueprintCallable)
	virtual float GetMaxHealth() { return 0.0f; }
	UFUNCTION(BlueprintCallable)
	virtual float GetPower() { return 0.0f; }
	UFUNCTION(BlueprintCallable)
	virtual float GetMaxPower() { return 0.0f; }
	UFUNCTION(BlueprintCallable)
	virtual float GetTP() { return 0.0f; }
	UFUNCTION(BlueprintCallable)
	virtual void PlayHitReaction() {}
	UFUNCTION(BlueprintCallable)
	virtual void HitFlash() {}
	UFUNCTION(BlueprintCallable)
	virtual void PlayVocalization(const EVocalizationType VoiceType) {}
	UFUNCTION(BlueprintCallable)
	virtual void PlayWeaponSwingAtLocation(const FVector& Location) {}
	UFUNCTION(BlueprintCallable)
	virtual void PlayImpactSoundAtLocation(const FVector& Location) {}

	UFUNCTION(BlueprintCallable)
	virtual void PlayWeaponWhoosh() {}

	UFUNCTION(BlueprintCallable)
	virtual void PlayWeaponImpact() {}

	UFUNCTION(BlueprintCallable)
	virtual void PushAttackResults(const TArray<FAttackResult>& Results) {}

	UFUNCTION(BlueprintCallable)
	virtual void AdvanceAttackResult() {}

	UFUNCTION(BlueprintCallable)
	virtual bool GetCurrentAttackResult(FAttackResult& OutResult) { return false; }

	UFUNCTION(BlueprintCallable)
	virtual void ClearAttackResults() {}

	UFUNCTION(BlueprintCallable)
	virtual void DoAttackSequence() {}

	UFUNCTION(BlueprintCallable)
	virtual void DamageCombatText(float Amount, bool bCritical = false) {}
	
	UFUNCTION(BlueprintCallable)
	virtual void HealingCombatText(float Amount, bool bIsCritical = false) {}

	UFUNCTION(BlueprintCallable)
	virtual void StatusEffectText(const FString& Text) {}
	
	UFUNCTION(BlueprintCallable)
	virtual void CameraShake(const ECameraShakeType& Shake, float Intensity = 1.f) {}
};
