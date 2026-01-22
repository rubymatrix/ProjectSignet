// Copyright Red Lotus Games, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "AlsAnimationInstance.h"
#include "ModularAnimData.h"
#include "SignetPlayerAnimInstance.generated.h"


class ASignetPlayerCharacter;
/**
 * Custom ALS Animation instance, much simplified
 */
UCLASS(ClassGroup=(Signet), AutoExpandCategories=("Config", "Config|Modular AnimData"))
class SIGNETGAME_API USignetPlayerAnimInstance : public UAlsAnimationInstance
{
	GENERATED_BODY()

public:
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Transient)
	TWeakObjectPtr<ASignetPlayerCharacter> SignetCharacter;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Config|Animation")
	bool bEnableLowerBodySlot = false;

// Gameplay Tag Caches

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "State|Tags")
	FGameplayTag MainWeaponSkillTag;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "State|Tags")
	FGameplayTag SubWeaponSkillTag;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "State|Tags")
	FGameplayTag RangedWeaponSkillTag;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "State|Tags")
	FGameplayTag StateTag;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "State|Tags")
	FGameplayTag CastingTag;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "State|Tags")
	FGameplayTag GaitTag;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "State|Tags")
	FGameplayTag RotationModeTag;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "State|Tags")
	FGameplayTag MoveDirectionTag;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State|Animation")
	float OrientationWarpAngle = 0.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State|Animation")
	bool bLookAtEnabled = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State|Animation")
	FVector LookAtLocation = FVector::ZeroVector;
	
// Begin Animation Setting References
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Config|Modular AnimData")
	FModularAnimData AnimData;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Config|Modular AnimData")
	TMap<FGameplayTag, FLayeredSequence> CastingAnims;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Config|Modular AnimData")
	TMap<FGameplayTag, FWeaponOverlay> WeaponOverlayAnims;
	
	USignetPlayerAnimInstance();

	virtual void NativeInitializeAnimation() override;

	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	UFUNCTION(BlueprintCallable, BlueprintPure, meta=(BlueprintThreadSafe))
	bool IsMoving() const;

	UFUNCTION(BlueprintCallable, Category = "Dragon|Animation", meta=(BlueprintThreadSafe))
	FWeaponOverlay GetWeaponData(const FGameplayTag& InWeaponTag) const;
	
	UFUNCTION(BlueprintCallable, Category = "Dragon|Animation", meta=(BlueprintThreadSafe))
	FLayeredSequence GetCastingSchedule(const FGameplayTag& InCastingTag) const;
	
	UFUNCTION(BlueprintCallable, Category = "Dragon|Animation", meta=(BlueprintThreadSafe))
	FLayeredSequence GetCurrentCastingSchedule() const;

	UFUNCTION(BlueprintCallable, Category = "Dragon|Animation", meta=(BlueprintThreadSafe))
	FWeaponOverlay GetCurrentWeaponData() const;

	UFUNCTION(BlueprintCallable, Category = "Dragon|Animation", meta=(BlueprintThreadSafe))
	FLayeredSequence GetCurrentState() const;
	
	UFUNCTION(BlueprintCallable, Category = "Dragon|Animation", meta=(BlueprintThreadSafe))
	FLayeredSequence GetCurrentWalkAnim() const;

	UFUNCTION(BlueprintCallable, Category = "Dragon|Animation", meta=(BlueprintThreadSafe))
	FLayeredSequence GetCurrentRunAnim() const;
	
	UFUNCTION(BlueprintCallable, Category = "Dragon|Animation", meta=(BlueprintThreadSafe))
	FLayeredSequence GetCurrentIdleAnim() const;

private:

	void DetermineStateTag();
	
	void DetermineCastingTag();

	void DetermineWeaponTags();

	FGameplayTag DetermineDirectionTag();
	
};
