// Copyright Red Lotus Games, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "SignetGame/Combat/CombatTypes.h"
#include "GameplayTagContainer.h"
#include "SignetCombatStatics.generated.h"

class USignetAbilitySystemComponent;

/**
 * Static library for porting legacy combat calculations to the Signet Ability System.
 */
UCLASS()
class SIGNETGAME_API USignetCombatStatics : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	/** Calculates the hit rate (0-100) based on accuracy and evasion. Ported from battleutils::GetHitRate. */
	UFUNCTION(BlueprintCallable, Category = "Combat")
	static float CalculateHitRate(const USignetAbilitySystemComponent* AttackerAsc, const USignetAbilitySystemComponent* TargetAsc, bool bIsRanged = false, int32 AccuracyBonus = 0);

	/** Calculates the pDIF damage ratio. Ported from battleutils::GetDamageRatio. */
	UFUNCTION(BlueprintCallable, Category = "Combat")
	static float CalculateDamageRatio(const USignetAbilitySystemComponent* AttackerAsc, const USignetAbilitySystemComponent* TargetAsc, bool bIsCritical, float AttackBonus = 1.0f, FGameplayTag SkillTag = FGameplayTag(), EGearSlot WeaponSlot = EGearSlot::Main);

	/** Calculates the Strength-based base damage bonus (fSTR). Ported from battleutils::GetFSTR. */
	UFUNCTION(BlueprintCallable, Category = "Combat")
	static int32 CalculateFSTR(const USignetAbilitySystemComponent* AttackerAsc, const USignetAbilitySystemComponent* TargetAsc, EGearSlot WeaponSlot = EGearSlot::Main);

	/** Calculates the base damage before pDIF. Ported from CAttack::ProcessDamage and physical_utilities.lua. */
	UFUNCTION(BlueprintCallable, Category = "Combat")
	static int32 CalculateBaseDamage(const USignetAbilitySystemComponent* AttackerAsc, const USignetAbilitySystemComponent* TargetAsc, EGearSlot WeaponSlot, EPhysicalAttackType AttackType, bool bIsH2H, bool bIsFirstSwing, bool bIsSA, bool bIsTA, float DamageRatio);

	/** Ported from xi.combat.physicalHitRate.checkAnticipated Lua. */
	UFUNCTION(BlueprintCallable, Category = "Combat")
	static bool CheckAnticipated(const USignetAbilitySystemComponent* AttackerAsc, const USignetAbilitySystemComponent* TargetAsc);

	/** Ported from xi.combat.counter.checkSeiganCounter Lua. */
	UFUNCTION(BlueprintCallable, Category = "Combat")
	static bool CheckSeiganCounter(const USignetAbilitySystemComponent* AttackerAsc, const USignetAbilitySystemComponent* TargetAsc, bool& bOutIsCountered, bool& bOutIsCritical);

	/** Calculates the Extra Kick Attack rate (0-100). */
	UFUNCTION(BlueprintCallable, Category = "Combat")
	static float CalculateKickAttackRate(const USignetAbilitySystemComponent* AttackerAsc);

	/** Calculates Defense: floor( VIT * 1.5 ) + Level + 8 */
	UFUNCTION(BlueprintCallable, Category = "Combat")
	static float CalculateDefense(const USignetAbilitySystemComponent* Asc);

	/** Calculates Attack: 8 + Combat Skill + STR */
	UFUNCTION(BlueprintCallable, Category = "Combat")
	static float CalculateAttack(const USignetAbilitySystemComponent* Asc, FGameplayTag SkillTag);

	/** Calculates Accuracy: Accuracy from DEX + Accuracy from Skill + Accuracy from Traits/Gear/etc. */
	UFUNCTION(BlueprintCallable, Category = "Combat")
	static float CalculateAccuracy(const USignetAbilitySystemComponent* Asc, FGameplayTag SkillTag);

	/** Helper to get the numeric value of a skill based on its tag. */
	UFUNCTION(BlueprintCallable, Category = "Combat")
	static float GetSkillValue(const USignetAbilitySystemComponent* Asc, FGameplayTag SkillTag);

	/** Determines if an attack is blocked by a shield. Ported from attackutils::IsBlocked. */
	UFUNCTION(BlueprintCallable, Category = "Combat")
	static bool CheckShieldBlock(const USignetAbilitySystemComponent* AttackerAsc, const USignetAbilitySystemComponent* TargetAsc);

	/** Determines if an attack is guarded. Ported from attackutils::IsGuarded. */
	UFUNCTION(BlueprintCallable, Category = "Combat")
	static bool CheckGuarded(const USignetAbilitySystemComponent* AttackerAsc, const USignetAbilitySystemComponent* TargetAsc);

	/** Checks for specialized damage multipliers. Ported from attackutils::CheckForDamageMultiplier. */
	UFUNCTION(BlueprintCallable, Category = "Combat")
	static int32 ApplyDamageMultipliers(const USignetAbilitySystemComponent* AttackerAsc, int32 CurrentDamage, EPhysicalAttackType AttackType, EGearSlot WeaponSlot, bool bIsFirstSwing);
};
