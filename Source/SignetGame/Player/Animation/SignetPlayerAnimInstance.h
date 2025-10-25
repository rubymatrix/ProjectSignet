// Copyright Red Lotus Games, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "AlsAnimationInstance.h"
#include "ModularAnimData.h"
#include "SignetPlayerAnimInstance.generated.h"

#define STATE_PROPERTY() UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "State|Tags")
#define ANIM_PROPERTY() UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Config|Modular Animations")

class ASignetPlayerCharacter;
/**
 * Custom ALS Animation instance, much simplified
 */
UCLASS(ClassGroup=(Signet), AutoExpandCategories=("Config", "Config|Modular Animations"))
class SIGNETGAME_API USignetPlayerAnimInstance : public UAlsAnimationInstance
{
	GENERATED_BODY()

	
public:
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Transient)
	TWeakObjectPtr<ASignetPlayerCharacter> SignetCharacter;

// Gameplay Tag Caches

	STATE_PROPERTY() FGameplayTag MainWeaponSkillTag;
	STATE_PROPERTY() FGameplayTag SubWeaponSkillTag;
	STATE_PROPERTY() FGameplayTag RangedWeaponSkillTag;
	STATE_PROPERTY() FGameplayTag StateTag;
	STATE_PROPERTY() FGameplayTag CastingTag;
	STATE_PROPERTY() FGameplayTag GaitTag;
	STATE_PROPERTY() FGameplayTag RotationModeTag;
	STATE_PROPERTY() FGameplayTag MoveDirectionTag;
	
	
// Begin Animation Setting References
	
	ANIM_PROPERTY() FModularAnimData Animations;
	ANIM_PROPERTY() TMap<FGameplayTag, FLayeredSequence> CastingAnims;
	ANIM_PROPERTY() TMap<FGameplayTag, FWeaponOverlay> WeaponOverlayAnims;


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

#undef STATE_PROPERTY
#undef ANIM_PROPERTY