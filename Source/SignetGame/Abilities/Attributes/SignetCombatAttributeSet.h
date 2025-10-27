// Copyright 2025, Red Lotus Games

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AttributeMacros.h"
#include "AbilitySystemComponent.h"
#include "Abilities/Attributes/GSCAttributeSetBase.h"
#include "SignetCombatAttributeSet.generated.h"

UCLASS()
class SIGNETGAME_API USignetCombatAttributeSet : public UGSCAttributeSetBase
{
	GENERATED_BODY()

public:

	USignetCombatAttributeSet();

	// =========================
	// Special
	// =========================
	UPROPERTY(BlueprintReadOnly, Category="Stats|Combat|Special", ReplicatedUsing=OnRep_Reraise)
	FGameplayAttributeData Reraise;
	ATTRIBUTE_ACCESSORS(USignetCombatAttributeSet, Reraise);
	UFUNCTION()
	void OnRep_Reraise(const FGameplayAttributeData& OldReraise);

	UPROPERTY(BlueprintReadOnly, Category="Stats|Combat|Special", ReplicatedUsing=OnRep_Regen)
	FGameplayAttributeData Regen;
	ATTRIBUTE_ACCESSORS(USignetCombatAttributeSet, Regen);
	UFUNCTION()
	void OnRep_Regen(const FGameplayAttributeData& OldRegen);

	UPROPERTY(BlueprintReadOnly, Category="Stats|Combat|Special", ReplicatedUsing=OnRep_Regain)
	FGameplayAttributeData Regain;
	ATTRIBUTE_ACCESSORS(USignetCombatAttributeSet, Regain);
	UFUNCTION()
	void OnRep_Regain(const FGameplayAttributeData& OldRegain);

	UPROPERTY(BlueprintReadOnly, Category="Stats|Combat|Special", ReplicatedUsing=OnRep_Refresh)
	FGameplayAttributeData Refresh;
	ATTRIBUTE_ACCESSORS(USignetCombatAttributeSet, Refresh);
	UFUNCTION()
	void OnRep_Refresh(const FGameplayAttributeData& OldRefresh);

	UPROPERTY(BlueprintReadOnly, Category="Stats|Combat|Special", ReplicatedUsing=OnRep_RegenPercent)
	FGameplayAttributeData RegenPercent;
	ATTRIBUTE_ACCESSORS(USignetCombatAttributeSet, RegenPercent);
	UFUNCTION()
	void OnRep_RegenPercent(const FGameplayAttributeData& OldRegenPercent);

	UPROPERTY(BlueprintReadOnly, Category="Stats|Combat|Special", ReplicatedUsing=OnRep_RefreshPercent)
	FGameplayAttributeData RefreshPercent;
	ATTRIBUTE_ACCESSORS(USignetCombatAttributeSet, RefreshPercent);
	UFUNCTION()
	void OnRep_RefreshPercent(const FGameplayAttributeData& OldRefreshPercent);

	// =========================
	// Attack & Accuracy
	// =========================
	UPROPERTY(BlueprintReadOnly, Category="Stats|Combat", ReplicatedUsing=OnRep_Attack)
	FGameplayAttributeData Attack;
	ATTRIBUTE_ACCESSORS(USignetCombatAttributeSet, Attack);
	UFUNCTION()
	void OnRep_Attack(const FGameplayAttributeData& OldAttack);

	UPROPERTY(BlueprintReadOnly, Category="Stats|Combat", ReplicatedUsing=OnRep_AttackPercent)
	FGameplayAttributeData AttackPercent;
	ATTRIBUTE_ACCESSORS(USignetCombatAttributeSet, AttackPercent);
	UFUNCTION()
	void OnRep_AttackPercent(const FGameplayAttributeData& OldAttackPercent);

	UPROPERTY(BlueprintReadOnly, Category="Stats|Combat", ReplicatedUsing=OnRep_Accuracy)
	FGameplayAttributeData Accuracy;
	ATTRIBUTE_ACCESSORS(USignetCombatAttributeSet, Accuracy);
	UFUNCTION()
	void OnRep_Accuracy(const FGameplayAttributeData& OldAccuracy);

	UPROPERTY(BlueprintReadOnly, Category="Stats|Combat", ReplicatedUsing=OnRep_RangedAttack)
	FGameplayAttributeData RangedAttack;
	ATTRIBUTE_ACCESSORS(USignetCombatAttributeSet, RangedAttack);
	UFUNCTION()
	void OnRep_RangedAttack(const FGameplayAttributeData& OldRangedAttack);

	UPROPERTY(BlueprintReadOnly, Category="Stats|Combat", ReplicatedUsing=OnRep_RangedAttackPercent)
	FGameplayAttributeData RangedAttackPercent;
	ATTRIBUTE_ACCESSORS(USignetCombatAttributeSet, RangedAttackPercent);
	UFUNCTION()
	void OnRep_RangedAttackPercent(const FGameplayAttributeData& OldRangedAttackPercent);

	UPROPERTY(BlueprintReadOnly, Category="Stats|Combat", ReplicatedUsing=OnRep_RangedAccuracy)
	FGameplayAttributeData RangedAccuracy;
	ATTRIBUTE_ACCESSORS(USignetCombatAttributeSet, RangedAccuracy);
	UFUNCTION()
	void OnRep_RangedAccuracy(const FGameplayAttributeData& OldRangedAccuracy);

	UPROPERTY(BlueprintReadOnly, Category="Stats|Combat", ReplicatedUsing=OnRep_MagicAttack)
	FGameplayAttributeData MagicAttack;
	ATTRIBUTE_ACCESSORS(USignetCombatAttributeSet, MagicAttack);
	UFUNCTION()
	void OnRep_MagicAttack(const FGameplayAttributeData& OldMagicAttack);

	UPROPERTY(BlueprintReadOnly, Category="Stats|Combat", ReplicatedUsing=OnRep_MagicAccuracy)
	FGameplayAttributeData MagicAccuracy;
	ATTRIBUTE_ACCESSORS(USignetCombatAttributeSet, MagicAccuracy);
	UFUNCTION()
	void OnRep_MagicAccuracy(const FGameplayAttributeData& OldMagicAccuracy);

	// =========================
	// Defensive Stats
	// =========================
	UPROPERTY(BlueprintReadOnly, Category="Stats|Combat|Defensive", ReplicatedUsing=OnRep_Defense)
	FGameplayAttributeData Defense;
	ATTRIBUTE_ACCESSORS(USignetCombatAttributeSet, Defense);
	UFUNCTION()
	void OnRep_Defense(const FGameplayAttributeData& OldDefense);

	UPROPERTY(BlueprintReadOnly, Category="Stats|Combat|Defensive", ReplicatedUsing=OnRep_DefensePercent)
	FGameplayAttributeData DefensePercent;
	ATTRIBUTE_ACCESSORS(USignetCombatAttributeSet, DefensePercent);
	UFUNCTION()
	void OnRep_DefensePercent(const FGameplayAttributeData& OldDefensePercent);

	UPROPERTY(BlueprintReadOnly, Category="Stats|Combat|Defensive", ReplicatedUsing=OnRep_Evasion)
	FGameplayAttributeData Evasion;
	ATTRIBUTE_ACCESSORS(USignetCombatAttributeSet, Evasion);
	UFUNCTION()
	void OnRep_Evasion(const FGameplayAttributeData& OldEvasion);

	UPROPERTY(BlueprintReadOnly, Category="Stats|Combat|Defensive", ReplicatedUsing=OnRep_MagicDefense)
	FGameplayAttributeData MagicDefense;
	ATTRIBUTE_ACCESSORS(USignetCombatAttributeSet, MagicDefense);
	UFUNCTION()
	void OnRep_MagicDefense(const FGameplayAttributeData& OldMagicDefense);

	UPROPERTY(BlueprintReadOnly, Category="Stats|Combat|Defensive", ReplicatedUsing=OnRep_MagicEvasion)
	FGameplayAttributeData MagicEvasion;
	ATTRIBUTE_ACCESSORS(USignetCombatAttributeSet, MagicEvasion);
	UFUNCTION()
	void OnRep_MagicEvasion(const FGameplayAttributeData& OldMagicEvasion);

	UPROPERTY(BlueprintReadOnly, Category="Stats|Combat|Defensive", ReplicatedUsing=OnRep_RangedDefense)
	FGameplayAttributeData RangedDefense;
	ATTRIBUTE_ACCESSORS(USignetCombatAttributeSet, RangedDefense);
	UFUNCTION()
	void OnRep_RangedDefense(const FGameplayAttributeData& OldRangedDefense);

	UPROPERTY(BlueprintReadOnly, Category="Stats|Combat|Defensive", ReplicatedUsing=OnRep_RangedEvasion)
	FGameplayAttributeData RangedEvasion;
	ATTRIBUTE_ACCESSORS(USignetCombatAttributeSet, RangedEvasion);
	UFUNCTION()
	void OnRep_RangedEvasion(const FGameplayAttributeData& OldRangedEvasion);

	UPROPERTY(BlueprintReadOnly, Category="Stats|Combat|Defensive", ReplicatedUsing=OnRep_TacticalParry)
	FGameplayAttributeData TacticalParry;
	ATTRIBUTE_ACCESSORS(USignetCombatAttributeSet, TacticalParry);
	UFUNCTION()
	void OnRep_TacticalParry(const FGameplayAttributeData& OldTacticalParry);

	UPROPERTY(BlueprintReadOnly, Category="Stats|Combat|Defensive", ReplicatedUsing=OnRep_GuardPercent)
	FGameplayAttributeData GuardPercent;
	ATTRIBUTE_ACCESSORS(USignetCombatAttributeSet, GuardPercent);
	UFUNCTION()
	void OnRep_GuardPercent(const FGameplayAttributeData& OldGuardPercent);

	UPROPERTY(BlueprintReadOnly, Category="Stats|Combat|Defensive", ReplicatedUsing=OnRep_ShieldBlockRate)
	FGameplayAttributeData ShieldBlockRate;
	ATTRIBUTE_ACCESSORS(USignetCombatAttributeSet, ShieldBlockRate);
	UFUNCTION()
	void OnRep_ShieldBlockRate(const FGameplayAttributeData& OldShieldBlockRate);

	UPROPERTY(BlueprintReadOnly, Category="Stats|Combat|Defensive", ReplicatedUsing=OnRep_Retaliation)
	FGameplayAttributeData Retaliation;
	ATTRIBUTE_ACCESSORS(USignetCombatAttributeSet, Retaliation);
	UFUNCTION()
	void OnRep_Retaliation(const FGameplayAttributeData& OldRetaliation);

	// =========================
	// Weapon-based Attributes
	// =========================
	UPROPERTY(BlueprintReadOnly, Category="Stats|Combat", ReplicatedUsing=OnRep_MeleeRange)
	FGameplayAttributeData MeleeRange;
	ATTRIBUTE_ACCESSORS(USignetCombatAttributeSet, MeleeRange);
	UFUNCTION()
	void OnRep_MeleeRange(const FGameplayAttributeData& OldMeleeRange);

	UPROPERTY(BlueprintReadOnly, Category="Stats|Combat", ReplicatedUsing=OnRep_DamageRating)
	FGameplayAttributeData DamageRating;
	ATTRIBUTE_ACCESSORS(USignetCombatAttributeSet, DamageRating);
	UFUNCTION()
	void OnRep_DamageRating(const FGameplayAttributeData& OldDamageRating);

	UPROPERTY(BlueprintReadOnly, Category="Stats|Combat", ReplicatedUsing=OnRep_WeaponDelay)
	FGameplayAttributeData WeaponDelay;
	ATTRIBUTE_ACCESSORS(USignetCombatAttributeSet, WeaponDelay);
	UFUNCTION()
	void OnRep_WeaponDelay(const FGameplayAttributeData& OldWeaponDelay);

	UPROPERTY(BlueprintReadOnly, Category="Stats|Combat", ReplicatedUsing=OnRep_WeaponRank)
	FGameplayAttributeData WeaponRank;
	ATTRIBUTE_ACCESSORS(USignetCombatAttributeSet, WeaponRank);
	UFUNCTION()
	void OnRep_WeaponRank(const FGameplayAttributeData& OldWeaponRank);

	UPROPERTY(BlueprintReadOnly, Category="Stats|Combat", ReplicatedUsing=OnRep_WeaponRating)
	FGameplayAttributeData WeaponRating;
	ATTRIBUTE_ACCESSORS(USignetCombatAttributeSet, WeaponRating);
	UFUNCTION()
	void OnRep_WeaponRating(const FGameplayAttributeData& OldWeaponRating);

	UPROPERTY(BlueprintReadOnly, Category="Stats|Combat", ReplicatedUsing=OnRep_WeaponDamage)
	FGameplayAttributeData WeaponDamage;
	ATTRIBUTE_ACCESSORS(USignetCombatAttributeSet, WeaponDamage);
	UFUNCTION()
	void OnRep_WeaponDamage(const FGameplayAttributeData& OldWeaponDamage);

	UPROPERTY(BlueprintReadOnly, Category="Stats|Combat", ReplicatedUsing=OnRep_SubWeaponRank)
	FGameplayAttributeData SubWeaponRank;
	ATTRIBUTE_ACCESSORS(USignetCombatAttributeSet, SubWeaponRank);
	UFUNCTION()
	void OnRep_SubWeaponRank(const FGameplayAttributeData& OldSubWeaponRank);

	UPROPERTY(BlueprintReadOnly, Category="Stats|Combat", ReplicatedUsing=OnRep_SubWeaponRating)
	FGameplayAttributeData SubWeaponRating;
	ATTRIBUTE_ACCESSORS(USignetCombatAttributeSet, SubWeaponRating);
	UFUNCTION()
	void OnRep_SubWeaponRating(const FGameplayAttributeData& OldSubWeaponRating);

	UPROPERTY(BlueprintReadOnly, Category="Stats|Combat", ReplicatedUsing=OnRep_SubWeaponDamage)
	FGameplayAttributeData SubWeaponDamage;
	ATTRIBUTE_ACCESSORS(USignetCombatAttributeSet, SubWeaponDamage);
	UFUNCTION()
	void OnRep_SubWeaponDamage(const FGameplayAttributeData& OldSubWeaponDamage);

	UPROPERTY(BlueprintReadOnly, Category="Stats|Combat", ReplicatedUsing=OnRep_RangedWeaponDelay)
	FGameplayAttributeData RangedWeaponDelay;
	ATTRIBUTE_ACCESSORS(USignetCombatAttributeSet, RangedWeaponDelay);
	UFUNCTION()
	void OnRep_RangedWeaponDelay(const FGameplayAttributeData& OldRangedWeaponDelay);

	UPROPERTY(BlueprintReadOnly, Category="Stats|Combat", ReplicatedUsing=OnRep_RangedWeaponRank)
	FGameplayAttributeData RangedWeaponRank;
	ATTRIBUTE_ACCESSORS(USignetCombatAttributeSet, RangedWeaponRank);
	UFUNCTION()
	void OnRep_RangedWeaponRank(const FGameplayAttributeData& OldRangedWeaponRank);

	UPROPERTY(BlueprintReadOnly, Category="Stats|Combat", ReplicatedUsing=OnRep_RangedWeaponRating)
	FGameplayAttributeData RangedWeaponRating;
	ATTRIBUTE_ACCESSORS(USignetCombatAttributeSet, RangedWeaponRating);
	UFUNCTION()
	void OnRep_RangedWeaponRating(const FGameplayAttributeData& OldRangedWeaponRating);

	UPROPERTY(BlueprintReadOnly, Category="Stats|Combat", ReplicatedUsing=OnRep_RangedWeaponDamage)
	FGameplayAttributeData RangedWeaponDamage;
	ATTRIBUTE_ACCESSORS(USignetCombatAttributeSet, RangedWeaponDamage);
	UFUNCTION()
	void OnRep_RangedWeaponDamage(const FGameplayAttributeData& OldRangedWeaponDamage);

	// =========================
	// Elemental Attack & Accuracy
	// =========================
	UPROPERTY(BlueprintReadOnly, Category="Stats|Combat|Elemental", ReplicatedUsing=OnRep_FireAttack)
	FGameplayAttributeData FireAttack;
	ATTRIBUTE_ACCESSORS(USignetCombatAttributeSet, FireAttack);
	UFUNCTION()
	void OnRep_FireAttack(const FGameplayAttributeData& OldFireAttack);

	UPROPERTY(BlueprintReadOnly, Category="Stats|Combat|Elemental", ReplicatedUsing=OnRep_IceAttack)
	FGameplayAttributeData IceAttack;
	ATTRIBUTE_ACCESSORS(USignetCombatAttributeSet, IceAttack);
	UFUNCTION()
	void OnRep_IceAttack(const FGameplayAttributeData& OldIceAttack);

	UPROPERTY(BlueprintReadOnly, Category="Stats|Combat|Elemental", ReplicatedUsing=OnRep_WindAttack)
	FGameplayAttributeData WindAttack;
	ATTRIBUTE_ACCESSORS(USignetCombatAttributeSet, WindAttack);
	UFUNCTION()
	void OnRep_WindAttack(const FGameplayAttributeData& OldWindAttack);

	UPROPERTY(BlueprintReadOnly, Category="Stats|Combat|Elemental", ReplicatedUsing=OnRep_EarthAttack)
	FGameplayAttributeData EarthAttack;
	ATTRIBUTE_ACCESSORS(USignetCombatAttributeSet, EarthAttack);
	UFUNCTION()
	void OnRep_EarthAttack(const FGameplayAttributeData& OldEarthAttack);

	UPROPERTY(BlueprintReadOnly, Category="Stats|Combat|Elemental", ReplicatedUsing=OnRep_LightningAttack)
	FGameplayAttributeData LightningAttack;
	ATTRIBUTE_ACCESSORS(USignetCombatAttributeSet, LightningAttack);
	UFUNCTION()
	void OnRep_LightningAttack(const FGameplayAttributeData& OldLightningAttack);

	UPROPERTY(BlueprintReadOnly, Category="Stats|Combat|Elemental", ReplicatedUsing=OnRep_WaterAttack)
	FGameplayAttributeData WaterAttack;
	ATTRIBUTE_ACCESSORS(USignetCombatAttributeSet, WaterAttack);
	UFUNCTION()
	void OnRep_WaterAttack(const FGameplayAttributeData& OldWaterAttack);

	UPROPERTY(BlueprintReadOnly, Category="Stats|Combat|Elemental", ReplicatedUsing=OnRep_LightAttack)
	FGameplayAttributeData LightAttack;
	ATTRIBUTE_ACCESSORS(USignetCombatAttributeSet, LightAttack);
	UFUNCTION()
	void OnRep_LightAttack(const FGameplayAttributeData& OldLightAttack);

	UPROPERTY(BlueprintReadOnly, Category="Stats|Combat|Elemental", ReplicatedUsing=OnRep_DarkAttack)
	FGameplayAttributeData DarkAttack;
	ATTRIBUTE_ACCESSORS(USignetCombatAttributeSet, DarkAttack);
	UFUNCTION()
	void OnRep_DarkAttack(const FGameplayAttributeData& OldDarkAttack);

	UPROPERTY(BlueprintReadOnly, Category="Stats|Combat|Elemental", ReplicatedUsing=OnRep_FireAccuracy)
	FGameplayAttributeData FireAccuracy;
	ATTRIBUTE_ACCESSORS(USignetCombatAttributeSet, FireAccuracy);
	UFUNCTION()
	void OnRep_FireAccuracy(const FGameplayAttributeData& OldFireAccuracy);

	UPROPERTY(BlueprintReadOnly, Category="Stats|Combat|Elemental", ReplicatedUsing=OnRep_IceAccuracy)
	FGameplayAttributeData IceAccuracy;
	ATTRIBUTE_ACCESSORS(USignetCombatAttributeSet, IceAccuracy);
	UFUNCTION()
	void OnRep_IceAccuracy(const FGameplayAttributeData& OldIceAccuracy);

	UPROPERTY(BlueprintReadOnly, Category="Stats|Combat|Elemental", ReplicatedUsing=OnRep_WindAccuracy)
	FGameplayAttributeData WindAccuracy;
	ATTRIBUTE_ACCESSORS(USignetCombatAttributeSet, WindAccuracy);
	UFUNCTION()
	void OnRep_WindAccuracy(const FGameplayAttributeData& OldWindAccuracy);

	UPROPERTY(BlueprintReadOnly, Category="Stats|Combat|Elemental", ReplicatedUsing=OnRep_EarthAccuracy)
	FGameplayAttributeData EarthAccuracy;
	ATTRIBUTE_ACCESSORS(USignetCombatAttributeSet, EarthAccuracy);
	UFUNCTION()
	void OnRep_EarthAccuracy(const FGameplayAttributeData& OldEarthAccuracy);

	UPROPERTY(BlueprintReadOnly, Category="Stats|Combat|Elemental", ReplicatedUsing=OnRep_LightningAccuracy)
	FGameplayAttributeData LightningAccuracy;
	ATTRIBUTE_ACCESSORS(USignetCombatAttributeSet, LightningAccuracy);
	UFUNCTION()
	void OnRep_LightningAccuracy(const FGameplayAttributeData& OldLightningAccuracy);

	UPROPERTY(BlueprintReadOnly, Category="Stats|Combat|Elemental", ReplicatedUsing=OnRep_WaterAccuracy)
	FGameplayAttributeData WaterAccuracy;
	ATTRIBUTE_ACCESSORS(USignetCombatAttributeSet, WaterAccuracy);
	UFUNCTION()
	void OnRep_WaterAccuracy(const FGameplayAttributeData& OldWaterAccuracy);

	UPROPERTY(BlueprintReadOnly, Category="Stats|Combat|Elemental", ReplicatedUsing=OnRep_LightAccuracy)
	FGameplayAttributeData LightAccuracy;
	ATTRIBUTE_ACCESSORS(USignetCombatAttributeSet, LightAccuracy);
	UFUNCTION()
	void OnRep_LightAccuracy(const FGameplayAttributeData& OldLightAccuracy);

	UPROPERTY(BlueprintReadOnly, Category="Stats|Combat|Elemental", ReplicatedUsing=OnRep_DarkAccuracy)
	FGameplayAttributeData DarkAccuracy;
	ATTRIBUTE_ACCESSORS(USignetCombatAttributeSet, DarkAccuracy);
	UFUNCTION()
	void OnRep_DarkAccuracy(const FGameplayAttributeData& OldDarkAccuracy);

	// =========================
	// Damage Bonuses
	// =========================
	UPROPERTY(BlueprintReadOnly, Category="Stats|Combat|DamageBonuses", ReplicatedUsing=OnRep_HPDamageBonus)
	FGameplayAttributeData HPDamageBonus;
	ATTRIBUTE_ACCESSORS(USignetCombatAttributeSet, HPDamageBonus);
	UFUNCTION()
	void OnRep_HPDamageBonus(const FGameplayAttributeData& OldHPDamageBonus);

	UPROPERTY(BlueprintReadOnly, Category="Stats|Combat|DamageBonuses", ReplicatedUsing=OnRep_PetHPDamageBonus)
	FGameplayAttributeData PetHPDamageBonus;
	ATTRIBUTE_ACCESSORS(USignetCombatAttributeSet, PetHPDamageBonus);
	UFUNCTION()
	void OnRep_PetHPDamageBonus(const FGameplayAttributeData& OldPetHPDamageBonus);

	UPROPERTY(BlueprintReadOnly, Category="Stats|Combat|DamageBonuses", ReplicatedUsing=OnRep_DamageLimit)
	FGameplayAttributeData DamageLimit;
	ATTRIBUTE_ACCESSORS(USignetCombatAttributeSet, DamageLimit);
	UFUNCTION()
	void OnRep_DamageLimit(const FGameplayAttributeData& OldDamageLimit);

	UPROPERTY(BlueprintReadOnly, Category="Stats|Combat|DamageBonuses", ReplicatedUsing=OnRep_DamageLimitPercent)
	FGameplayAttributeData DamageLimitPercent;
	ATTRIBUTE_ACCESSORS(USignetCombatAttributeSet, DamageLimitPercent);
	UFUNCTION()
	void OnRep_DamageLimitPercent(const FGameplayAttributeData& OldDamageLimitPercent);

	UPROPERTY(BlueprintReadOnly, Category="Stats|Combat|DamageBonuses", ReplicatedUsing=OnRep_MagicBurstBonus)
	FGameplayAttributeData MagicBurstBonus;
	ATTRIBUTE_ACCESSORS(USignetCombatAttributeSet, MagicBurstBonus);
	UFUNCTION()
	void OnRep_MagicBurstBonus(const FGameplayAttributeData& OldMagicBurstBonus);

	UPROPERTY(BlueprintReadOnly, Category="Stats|Combat|DamageBonuses", ReplicatedUsing=OnRep_MagicBurstBonusII)
	FGameplayAttributeData MagicBurstBonusII;
	ATTRIBUTE_ACCESSORS(USignetCombatAttributeSet, MagicBurstBonusII);
	UFUNCTION()
	void OnRep_MagicBurstBonusII(const FGameplayAttributeData& OldMagicBurstBonusII);

	UPROPERTY(BlueprintReadOnly, Category="Stats|Combat|DamageBonuses", ReplicatedUsing=OnRep_BreathDamageBonus)
	FGameplayAttributeData BreathDamageBonus;
	ATTRIBUTE_ACCESSORS(USignetCombatAttributeSet, BreathDamageBonus);
	UFUNCTION()
	void OnRep_BreathDamageBonus(const FGameplayAttributeData& OldBreathDamageBonus);

	UPROPERTY(BlueprintReadOnly, Category="Stats|Combat|DamageBonuses", ReplicatedUsing=OnRep_DayNukeBonus)
	FGameplayAttributeData DayNukeBonus;
	ATTRIBUTE_ACCESSORS(USignetCombatAttributeSet, DayNukeBonus);
	UFUNCTION()
	void OnRep_DayNukeBonus(const FGameplayAttributeData& OldDayNukeBonus);

	UPROPERTY(BlueprintReadOnly, Category="Stats|Combat|DamageBonuses", ReplicatedUsing=OnRep_Iridescence)
	FGameplayAttributeData Iridescence;
	ATTRIBUTE_ACCESSORS(USignetCombatAttributeSet, Iridescence);
	UFUNCTION()
	void OnRep_Iridescence(const FGameplayAttributeData& OldIridescence);

	// =========================
	// Enmity Handling
	// =========================
	UPROPERTY(BlueprintReadOnly, Category="Stats|Combat", ReplicatedUsing=OnRep_Enmity)
	FGameplayAttributeData Enmity;
	ATTRIBUTE_ACCESSORS(USignetCombatAttributeSet, Enmity);
	UFUNCTION()
	void OnRep_Enmity(const FGameplayAttributeData& OldEnmity);

	UPROPERTY(BlueprintReadOnly, Category="Stats|Combat", ReplicatedUsing=OnRep_EnmityLossReduction)
	FGameplayAttributeData EnmityLossReduction;
	ATTRIBUTE_ACCESSORS(USignetCombatAttributeSet, EnmityLossReduction);
	UFUNCTION()
	void OnRep_EnmityLossReduction(const FGameplayAttributeData& OldEnmityLossReduction);

	// =========================
	// TP Generation
	// =========================
	UPROPERTY(BlueprintReadOnly, Category="Stats|Combat", ReplicatedUsing=OnRep_StoreTP)
	FGameplayAttributeData StoreTP;
	ATTRIBUTE_ACCESSORS(USignetCombatAttributeSet, StoreTP);
	UFUNCTION()
	void OnRep_StoreTP(const FGameplayAttributeData& OldStoreTP);

	UPROPERTY(BlueprintReadOnly, Category="Stats|Combat", ReplicatedUsing=OnRep_InhibitTP)
	FGameplayAttributeData InhibitTP;
	ATTRIBUTE_ACCESSORS(USignetCombatAttributeSet, InhibitTP);
	UFUNCTION()
	void OnRep_InhibitTP(const FGameplayAttributeData& OldInhibitTP);

	UPROPERTY(BlueprintReadOnly, Category="Stats|Combat", ReplicatedUsing=OnRep_FencerTPBonus)
	FGameplayAttributeData FencerTPBonus;
	ATTRIBUTE_ACCESSORS(USignetCombatAttributeSet, FencerTPBonus);
	UFUNCTION()
	void OnRep_FencerTPBonus(const FGameplayAttributeData& OldFencerTPBonus);

	UPROPERTY(BlueprintReadOnly, Category="Stats|Combat", ReplicatedUsing=OnRep_TacticalGuard)
	FGameplayAttributeData TacticalGuard;
	ATTRIBUTE_ACCESSORS(USignetCombatAttributeSet, TacticalGuard);
	UFUNCTION()
	void OnRep_TacticalGuard(const FGameplayAttributeData& OldTacticalGuard);

	UPROPERTY(BlueprintReadOnly, Category="Stats|Combat", ReplicatedUsing=OnRep_SubtleBlow)
	FGameplayAttributeData SubtleBlow;
	ATTRIBUTE_ACCESSORS(USignetCombatAttributeSet, SubtleBlow);
	UFUNCTION()
	void OnRep_SubtleBlow(const FGameplayAttributeData& OldSubtleBlow);

	UPROPERTY(BlueprintReadOnly, Category="Stats|Combat", ReplicatedUsing=OnRep_SubtleBlowII)
	FGameplayAttributeData SubtleBlowII;
	ATTRIBUTE_ACCESSORS(USignetCombatAttributeSet, SubtleBlowII);
	UFUNCTION()
	void OnRep_SubtleBlowII(const FGameplayAttributeData& OldSubtleBlowII);

	UPROPERTY(BlueprintReadOnly, Category="Stats|Combat", ReplicatedUsing=OnRep_OccultAcumen)
	FGameplayAttributeData OccultAcumen;
	ATTRIBUTE_ACCESSORS(USignetCombatAttributeSet, OccultAcumen);
	UFUNCTION()
	void OnRep_OccultAcumen(const FGameplayAttributeData& OldOccultAcumen);

	// =========================
	// Weapon Skill
	// =========================
	UPROPERTY(BlueprintReadOnly, Category="Stats|Combat|WeaponSkills", ReplicatedUsing=OnRep_WeaponSkillAccuracy)
	FGameplayAttributeData WeaponSkillAccuracy;
	ATTRIBUTE_ACCESSORS(USignetCombatAttributeSet, WeaponSkillAccuracy);
	UFUNCTION()
	void OnRep_WeaponSkillAccuracy(const FGameplayAttributeData& OldWeaponSkillAccuracy);

	UPROPERTY(BlueprintReadOnly, Category="Stats|Combat|WeaponSkills", ReplicatedUsing=OnRep_WeaponSkillDamage)
	FGameplayAttributeData WeaponSkillDamage;
	ATTRIBUTE_ACCESSORS(USignetCombatAttributeSet, WeaponSkillDamage);
	UFUNCTION()
	void OnRep_WeaponSkillDamage(const FGameplayAttributeData& OldWeaponSkillDamage);

	UPROPERTY(BlueprintReadOnly, Category="Stats|Combat|WeaponSkills", ReplicatedUsing=OnRep_WeaponSkillDamage_FirstHit)
	FGameplayAttributeData WeaponSkillDamage_FirstHit;
	ATTRIBUTE_ACCESSORS(USignetCombatAttributeSet, WeaponSkillDamage_FirstHit);
	UFUNCTION()
	void OnRep_WeaponSkillDamage_FirstHit(const FGameplayAttributeData& OldWeaponSkillDamage_FirstHit);

	UPROPERTY(BlueprintReadOnly, Category="Stats|Combat|WeaponSkills", ReplicatedUsing=OnRep_TPBonus)
	FGameplayAttributeData TPBonus;
	ATTRIBUTE_ACCESSORS(USignetCombatAttributeSet, TPBonus);
	UFUNCTION()
	void OnRep_TPBonus(const FGameplayAttributeData& OldTPBonus);

	UPROPERTY(BlueprintReadOnly, Category="Stats|Combat|WeaponSkills", ReplicatedUsing=OnRep_SaveTP)
	FGameplayAttributeData SaveTP;
	ATTRIBUTE_ACCESSORS(USignetCombatAttributeSet, SaveTP);
	UFUNCTION()
	void OnRep_SaveTP(const FGameplayAttributeData& OldSaveTP);

	UPROPERTY(BlueprintReadOnly, Category="Stats|Combat|WeaponSkills", ReplicatedUsing=OnRep_ConserveTP)
	FGameplayAttributeData ConserveTP;
	ATTRIBUTE_ACCESSORS(USignetCombatAttributeSet, ConserveTP);
	UFUNCTION()
	void OnRep_ConserveTP(const FGameplayAttributeData& OldConserveTP);

	UPROPERTY(BlueprintReadOnly, Category="Stats|Combat|WeaponSkills", ReplicatedUsing=OnRep_WeaponSkillSTRBonus)
	FGameplayAttributeData WeaponSkillSTRBonus;
	ATTRIBUTE_ACCESSORS(USignetCombatAttributeSet, WeaponSkillSTRBonus);
	UFUNCTION()
	void OnRep_WeaponSkillSTRBonus(const FGameplayAttributeData& OldWeaponSkillSTRBonus);

	UPROPERTY(BlueprintReadOnly, Category="Stats|Combat|WeaponSkills", ReplicatedUsing=OnRep_WeaponSkillDEXBonus)
	FGameplayAttributeData WeaponSkillDEXBonus;
	ATTRIBUTE_ACCESSORS(USignetCombatAttributeSet, WeaponSkillDEXBonus);
	UFUNCTION()
	void OnRep_WeaponSkillDEXBonus(const FGameplayAttributeData& OldWeaponSkillDEXBonus);

	UPROPERTY(BlueprintReadOnly, Category="Stats|Combat|WeaponSkills", ReplicatedUsing=OnRep_WeaponSkillVITBonus)
	FGameplayAttributeData WeaponSkillVITBonus;
	ATTRIBUTE_ACCESSORS(USignetCombatAttributeSet, WeaponSkillVITBonus);
	UFUNCTION()
	void OnRep_WeaponSkillVITBonus(const FGameplayAttributeData& OldWeaponSkillVITBonus);

	UPROPERTY(BlueprintReadOnly, Category="Stats|Combat|WeaponSkills", ReplicatedUsing=OnRep_WeaponSkillAGIBonus)
	FGameplayAttributeData WeaponSkillAGIBonus;
	ATTRIBUTE_ACCESSORS(USignetCombatAttributeSet, WeaponSkillAGIBonus);
	UFUNCTION()
	void OnRep_WeaponSkillAGIBonus(const FGameplayAttributeData& OldWeaponSkillAGIBonus);

	UPROPERTY(BlueprintReadOnly, Category="Stats|Combat|WeaponSkills", ReplicatedUsing=OnRep_WeaponSkillINTBonus)
	FGameplayAttributeData WeaponSkillINTBonus;
	ATTRIBUTE_ACCESSORS(USignetCombatAttributeSet, WeaponSkillINTBonus);
	UFUNCTION()
	void OnRep_WeaponSkillINTBonus(const FGameplayAttributeData& OldWeaponSkillINTBonus);

	UPROPERTY(BlueprintReadOnly, Category="Stats|Combat|WeaponSkills", ReplicatedUsing=OnRep_WeaponSkillMNDBonus)
	FGameplayAttributeData WeaponSkillMNDBonus;
	ATTRIBUTE_ACCESSORS(USignetCombatAttributeSet, WeaponSkillMNDBonus);
	UFUNCTION()
	void OnRep_WeaponSkillMNDBonus(const FGameplayAttributeData& OldWeaponSkillMNDBonus);

	UPROPERTY(BlueprintReadOnly, Category="Stats|Combat|WeaponSkills", ReplicatedUsing=OnRep_WeaponSkillCHRBonus)
	FGameplayAttributeData WeaponSkillCHRBonus;
	ATTRIBUTE_ACCESSORS(USignetCombatAttributeSet, WeaponSkillCHRBonus);
	UFUNCTION()
	void OnRep_WeaponSkillCHRBonus(const FGameplayAttributeData& OldWeaponSkillCHRBonus);

	// =========================
	// Recovery
	// =========================
	UPROPERTY(BlueprintReadOnly, Category="Stats|Combat|Recovery", ReplicatedUsing=OnRep_HpHeal)
	FGameplayAttributeData HpHeal;
	ATTRIBUTE_ACCESSORS(USignetCombatAttributeSet, HpHeal);
	UFUNCTION()
	void OnRep_HpHeal(const FGameplayAttributeData& OldHpHeal);

	UPROPERTY(BlueprintReadOnly, Category="Stats|Combat|Recovery", ReplicatedUsing=OnRep_MpHeal)
	FGameplayAttributeData MpHeal;
	ATTRIBUTE_ACCESSORS(USignetCombatAttributeSet, MpHeal);
	UFUNCTION()
	void OnRep_MpHeal(const FGameplayAttributeData& OldMpHeal);

	// =========================
	// Skill Rates
	// =========================
	UPROPERTY(BlueprintReadOnly, Category="Stats|Combat|Skills", ReplicatedUsing=OnRep_CombatSkillUpRate)
	FGameplayAttributeData CombatSkillUpRate;
	ATTRIBUTE_ACCESSORS(USignetCombatAttributeSet, CombatSkillUpRate);
	UFUNCTION()
	void OnRep_CombatSkillUpRate(const FGameplayAttributeData& OldCombatSkillUpRate);

	UPROPERTY(BlueprintReadOnly, Category="Stats|Combat|Skills", ReplicatedUsing=OnRep_MagicSkillUpRate)
	FGameplayAttributeData MagicSkillUpRate;
	ATTRIBUTE_ACCESSORS(USignetCombatAttributeSet, MagicSkillUpRate);
	UFUNCTION()
	void OnRep_MagicSkillUpRate(const FGameplayAttributeData& OldMagicSkillUpRate);

	// =========================
	// Damage Reductions
	// =========================
	UPROPERTY(BlueprintReadOnly, Category="Stats|Combat|DamageReduction", ReplicatedUsing=OnRep_DamageTaken)
	FGameplayAttributeData DamageTaken;
	ATTRIBUTE_ACCESSORS(USignetCombatAttributeSet, DamageTaken);
	UFUNCTION()
	void OnRep_DamageTaken(const FGameplayAttributeData& OldDamageTaken);

	UPROPERTY(BlueprintReadOnly, Category="Stats|Combat|DamageReduction", ReplicatedUsing=OnRep_PhysicalDamageTaken)
	FGameplayAttributeData PhysicalDamageTaken;
	ATTRIBUTE_ACCESSORS(USignetCombatAttributeSet, PhysicalDamageTaken);
	UFUNCTION()
	void OnRep_PhysicalDamageTaken(const FGameplayAttributeData& OldPhysicalDamageTaken);

	UPROPERTY(BlueprintReadOnly, Category="Stats|Combat|DamageReduction", ReplicatedUsing=OnRep_PhysicalDamageTakenII)
	FGameplayAttributeData PhysicalDamageTakenII;
	ATTRIBUTE_ACCESSORS(USignetCombatAttributeSet, PhysicalDamageTakenII);
	UFUNCTION()
	void OnRep_PhysicalDamageTakenII(const FGameplayAttributeData& OldPhysicalDamageTakenII);

	UPROPERTY(BlueprintReadOnly, Category="Stats|Combat|DamageReduction", ReplicatedUsing=OnRep_UncappedPhsyicalDamageTaken)
	FGameplayAttributeData UncappedPhsyicalDamageTaken;
	ATTRIBUTE_ACCESSORS(USignetCombatAttributeSet, UncappedPhsyicalDamageTaken);
	UFUNCTION()
	void OnRep_UncappedPhsyicalDamageTaken(const FGameplayAttributeData& OldUncappedPhsyicalDamageTaken);

	UPROPERTY(BlueprintReadOnly, Category="Stats|Combat|DamageReduction", ReplicatedUsing=OnRep_BreathDamageTaken)
	FGameplayAttributeData BreathDamageTaken;
	ATTRIBUTE_ACCESSORS(USignetCombatAttributeSet, BreathDamageTaken);
	UFUNCTION()
	void OnRep_BreathDamageTaken(const FGameplayAttributeData& OldBreathDamageTaken);

	UPROPERTY(BlueprintReadOnly, Category="Stats|Combat|DamageReduction", ReplicatedUsing=OnRep_UncappedBreathDamageTaken)
	FGameplayAttributeData UncappedBreathDamageTaken;
	ATTRIBUTE_ACCESSORS(USignetCombatAttributeSet, UncappedBreathDamageTaken);
	UFUNCTION()
	void OnRep_UncappedBreathDamageTaken(const FGameplayAttributeData& OldUncappedBreathDamageTaken);

	UPROPERTY(BlueprintReadOnly, Category="Stats|Combat|DamageReduction", ReplicatedUsing=OnRep_MagicDamageTaken)
	FGameplayAttributeData MagicDamageTaken;
	ATTRIBUTE_ACCESSORS(USignetCombatAttributeSet, MagicDamageTaken);
	UFUNCTION()
	void OnRep_MagicDamageTaken(const FGameplayAttributeData& OldMagicDamageTaken);

	UPROPERTY(BlueprintReadOnly, Category="Stats|Combat|DamageReduction", ReplicatedUsing=OnRep_MagicDamageTakenII)
	FGameplayAttributeData MagicDamageTakenII;
	ATTRIBUTE_ACCESSORS(USignetCombatAttributeSet, MagicDamageTakenII);
	UFUNCTION()
	void OnRep_MagicDamageTakenII(const FGameplayAttributeData& OldMagicDamageTakenII);

	UPROPERTY(BlueprintReadOnly, Category="Stats|Combat|DamageReduction", ReplicatedUsing=OnRep_UncappedMagicDamageTaken)
	FGameplayAttributeData UncappedMagicDamageTaken;
	ATTRIBUTE_ACCESSORS(USignetCombatAttributeSet, UncappedMagicDamageTaken);
	UFUNCTION()
	void OnRep_UncappedMagicDamageTaken(const FGameplayAttributeData& OldUncappedMagicDamageTaken);

	UPROPERTY(BlueprintReadOnly, Category="Stats|Combat|DamageReduction", ReplicatedUsing=OnRep_RangedDamageTaken)
	FGameplayAttributeData RangedDamageTaken;
	ATTRIBUTE_ACCESSORS(USignetCombatAttributeSet, RangedDamageTaken);
	UFUNCTION()
	void OnRep_RangedDamageTaken(const FGameplayAttributeData& OldRangedDamageTaken);

	UPROPERTY(BlueprintReadOnly, Category="Stats|Combat|DamageReduction", ReplicatedUsing=OnRep_UncappedRangedDamageTaken)
	FGameplayAttributeData UncappedRangedDamageTaken;
	ATTRIBUTE_ACCESSORS(USignetCombatAttributeSet, UncappedRangedDamageTaken);
	UFUNCTION()
	void OnRep_UncappedRangedDamageTaken(const FGameplayAttributeData& OldUncappedRangedDamageTaken);

	UPROPERTY(BlueprintReadOnly, Category="Stats|Combat|DamageReduction", ReplicatedUsing=OnRep_AreaDamageTaken)
	FGameplayAttributeData AreaDamageTaken;
	ATTRIBUTE_ACCESSORS(USignetCombatAttributeSet, AreaDamageTaken);
	UFUNCTION()
	void OnRep_AreaDamageTaken(const FGameplayAttributeData& OldAreaDamageTaken);

	UPROPERTY(BlueprintReadOnly, Category="Stats|Combat|DamageReduction", ReplicatedUsing=OnRep_SlashingDamageTaken)
	FGameplayAttributeData SlashingDamageTaken;
	ATTRIBUTE_ACCESSORS(USignetCombatAttributeSet, SlashingDamageTaken);
	UFUNCTION()
	void OnRep_SlashingDamageTaken(const FGameplayAttributeData& OldSlashingDamageTaken);

	UPROPERTY(BlueprintReadOnly, Category="Stats|Combat|DamageReduction", ReplicatedUsing=OnRep_PiercingDamageTaken)
	FGameplayAttributeData PiercingDamageTaken;
	ATTRIBUTE_ACCESSORS(USignetCombatAttributeSet, PiercingDamageTaken);
	UFUNCTION()
	void OnRep_PiercingDamageTaken(const FGameplayAttributeData& OldPiercingDamageTaken);

	UPROPERTY(BlueprintReadOnly, Category="Stats|Combat|DamageReduction", ReplicatedUsing=OnRep_ImpactDamageTaken)
	FGameplayAttributeData ImpactDamageTaken;
	ATTRIBUTE_ACCESSORS(USignetCombatAttributeSet, ImpactDamageTaken);
	UFUNCTION()
	void OnRep_ImpactDamageTaken(const FGameplayAttributeData& OldImpactDamageTaken);

	UPROPERTY(BlueprintReadOnly, Category="Stats|Combat|DamageReduction", ReplicatedUsing=OnRep_HandToHandDamageTaken)
	FGameplayAttributeData HandToHandDamageTaken;
	ATTRIBUTE_ACCESSORS(USignetCombatAttributeSet, HandToHandDamageTaken);
	UFUNCTION()
	void OnRep_HandToHandDamageTaken(const FGameplayAttributeData& OldHandToHandDamageTaken);

	UPROPERTY(BlueprintReadOnly, Category="Stats|Combat|DamageReduction", ReplicatedUsing=OnRep_FireDamageTaken)
	FGameplayAttributeData FireDamageTaken;
	ATTRIBUTE_ACCESSORS(USignetCombatAttributeSet, FireDamageTaken);
	UFUNCTION()
	void OnRep_FireDamageTaken(const FGameplayAttributeData& OldFireDamageTaken);

	UPROPERTY(BlueprintReadOnly, Category="Stats|Combat|DamageReduction", ReplicatedUsing=OnRep_IceDamageTaken)
	FGameplayAttributeData IceDamageTaken;
	ATTRIBUTE_ACCESSORS(USignetCombatAttributeSet, IceDamageTaken);
	UFUNCTION()
	void OnRep_IceDamageTaken(const FGameplayAttributeData& OldIceDamageTaken);

	UPROPERTY(BlueprintReadOnly, Category="Stats|Combat|DamageReduction", ReplicatedUsing=OnRep_WindDamageTaken)
	FGameplayAttributeData WindDamageTaken;
	ATTRIBUTE_ACCESSORS(USignetCombatAttributeSet, WindDamageTaken);
	UFUNCTION()
	void OnRep_WindDamageTaken(const FGameplayAttributeData& OldWindDamageTaken);

	UPROPERTY(BlueprintReadOnly, Category="Stats|Combat|DamageReduction", ReplicatedUsing=OnRep_EarthDamageTaken)
	FGameplayAttributeData EarthDamageTaken;
	ATTRIBUTE_ACCESSORS(USignetCombatAttributeSet, EarthDamageTaken);
	UFUNCTION()
	void OnRep_EarthDamageTaken(const FGameplayAttributeData& OldEarthDamageTaken);

	UPROPERTY(BlueprintReadOnly, Category="Stats|Combat|DamageReduction", ReplicatedUsing=OnRep_LightningDamageTaken)
	FGameplayAttributeData LightningDamageTaken;
	ATTRIBUTE_ACCESSORS(USignetCombatAttributeSet, LightningDamageTaken);
	UFUNCTION()
	void OnRep_LightningDamageTaken(const FGameplayAttributeData& OldLightningDamageTaken);

	UPROPERTY(BlueprintReadOnly, Category="Stats|Combat|DamageReduction", ReplicatedUsing=OnRep_WaterDamageTaken)
	FGameplayAttributeData WaterDamageTaken;
	ATTRIBUTE_ACCESSORS(USignetCombatAttributeSet, WaterDamageTaken);
	UFUNCTION()
	void OnRep_WaterDamageTaken(const FGameplayAttributeData& OldWaterDamageTaken);

	UPROPERTY(BlueprintReadOnly, Category="Stats|Combat|DamageReduction", ReplicatedUsing=OnRep_LightDamageTaken)
	FGameplayAttributeData LightDamageTaken;
	ATTRIBUTE_ACCESSORS(USignetCombatAttributeSet, LightDamageTaken);
	UFUNCTION()
	void OnRep_LightDamageTaken(const FGameplayAttributeData& OldLightDamageTaken);

	UPROPERTY(BlueprintReadOnly, Category="Stats|Combat|DamageReduction", ReplicatedUsing=OnRep_DarkDamageTaken)
	FGameplayAttributeData DarkDamageTaken;
	ATTRIBUTE_ACCESSORS(USignetCombatAttributeSet, DarkDamageTaken);
	UFUNCTION()
	void OnRep_DarkDamageTaken(const FGameplayAttributeData& OldDarkDamageTaken);

	// =========================
	// Damage Nullifications
	// =========================
	UPROPERTY(BlueprintReadOnly, Category="Stats|Combat|DamageNullifications", ReplicatedUsing=OnRep_NullifyDamage)
	FGameplayAttributeData NullifyDamage;
	ATTRIBUTE_ACCESSORS(USignetCombatAttributeSet, NullifyDamage);
	UFUNCTION()
	void OnRep_NullifyDamage(const FGameplayAttributeData& OldNullifyDamage);

	UPROPERTY(BlueprintReadOnly, Category="Stats|Combat|DamageNullifications", ReplicatedUsing=OnRep_NullifyPhysicalDamage)
	FGameplayAttributeData NullifyPhysicalDamage;
	ATTRIBUTE_ACCESSORS(USignetCombatAttributeSet, NullifyPhysicalDamage);
	UFUNCTION()
	void OnRep_NullifyPhysicalDamage(const FGameplayAttributeData& OldNullifyPhysicalDamage);

	UPROPERTY(BlueprintReadOnly, Category="Stats|Combat|DamageNullifications", ReplicatedUsing=OnRep_NullifyBreathDamage)
	FGameplayAttributeData NullifyBreathDamage;
	ATTRIBUTE_ACCESSORS(USignetCombatAttributeSet, NullifyBreathDamage);
	UFUNCTION()
	void OnRep_NullifyBreathDamage(const FGameplayAttributeData& OldNullifyBreathDamage);

	UPROPERTY(BlueprintReadOnly, Category="Stats|Combat|DamageNullifications", ReplicatedUsing=OnRep_NullifyMagicDamage)
	FGameplayAttributeData NullifyMagicDamage;
	ATTRIBUTE_ACCESSORS(USignetCombatAttributeSet, NullifyMagicDamage);
	UFUNCTION()
	void OnRep_NullifyMagicDamage(const FGameplayAttributeData& OldNullifyMagicDamage);

	UPROPERTY(BlueprintReadOnly, Category="Stats|Combat|DamageNullifications", ReplicatedUsing=OnRep_NullifyRangedDamage)
	FGameplayAttributeData NullifyRangedDamage;
	ATTRIBUTE_ACCESSORS(USignetCombatAttributeSet, NullifyRangedDamage);
	UFUNCTION()
	void OnRep_NullifyRangedDamage(const FGameplayAttributeData& OldNullifyRangedDamage);

	UPROPERTY(BlueprintReadOnly, Category="Stats|Combat|DamageNullifications", ReplicatedUsing=OnRep_NullifyFireDamage)
	FGameplayAttributeData NullifyFireDamage;
	ATTRIBUTE_ACCESSORS(USignetCombatAttributeSet, NullifyFireDamage);
	UFUNCTION()
	void OnRep_NullifyFireDamage(const FGameplayAttributeData& OldNullifyFireDamage);

	UPROPERTY(BlueprintReadOnly, Category="Stats|Combat|DamageNullifications", ReplicatedUsing=OnRep_NullifyIceDamage)
	FGameplayAttributeData NullifyIceDamage;
	ATTRIBUTE_ACCESSORS(USignetCombatAttributeSet, NullifyIceDamage);
	UFUNCTION()
	void OnRep_NullifyIceDamage(const FGameplayAttributeData& OldNullifyIceDamage);

	UPROPERTY(BlueprintReadOnly, Category="Stats|Combat|DamageNullifications", ReplicatedUsing=OnRep_NullifyWindDamage)
	FGameplayAttributeData NullifyWindDamage;
	ATTRIBUTE_ACCESSORS(USignetCombatAttributeSet, NullifyWindDamage);
	UFUNCTION()
	void OnRep_NullifyWindDamage(const FGameplayAttributeData& OldNullifyWindDamage);

	UPROPERTY(BlueprintReadOnly, Category="Stats|Combat|DamageNullifications", ReplicatedUsing=OnRep_NullifyEarthDamage)
	FGameplayAttributeData NullifyEarthDamage;
	ATTRIBUTE_ACCESSORS(USignetCombatAttributeSet, NullifyEarthDamage);
	UFUNCTION()
	void OnRep_NullifyEarthDamage(const FGameplayAttributeData& OldNullifyEarthDamage);

	UPROPERTY(BlueprintReadOnly, Category="Stats|Combat|DamageNullifications", ReplicatedUsing=OnRep_NullifyLightningDamage)
	FGameplayAttributeData NullifyLightningDamage;
	ATTRIBUTE_ACCESSORS(USignetCombatAttributeSet, NullifyLightningDamage);
	UFUNCTION()
	void OnRep_NullifyLightningDamage(const FGameplayAttributeData& OldNullifyLightningDamage);

	UPROPERTY(BlueprintReadOnly, Category="Stats|Combat|DamageNullifications", ReplicatedUsing=OnRep_NullifyWaterDamage)
	FGameplayAttributeData NullifyWaterDamage;
	ATTRIBUTE_ACCESSORS(USignetCombatAttributeSet, NullifyWaterDamage);
	UFUNCTION()
	void OnRep_NullifyWaterDamage(const FGameplayAttributeData& OldNullifyWaterDamage);

	UPROPERTY(BlueprintReadOnly, Category="Stats|Combat|DamageNullifications", ReplicatedUsing=OnRep_NullifyLightDamage)
	FGameplayAttributeData NullifyLightDamage;
	ATTRIBUTE_ACCESSORS(USignetCombatAttributeSet, NullifyLightDamage);
	UFUNCTION()
	void OnRep_NullifyLightDamage(const FGameplayAttributeData& OldNullifyLightDamage);

	UPROPERTY(BlueprintReadOnly, Category="Stats|Combat|DamageNullifications", ReplicatedUsing=OnRep_NullifyDarkDamage)
	FGameplayAttributeData NullifyDarkDamage;
	ATTRIBUTE_ACCESSORS(USignetCombatAttributeSet, NullifyDarkDamage);
	UFUNCTION()
	void OnRep_NullifyDarkDamage(const FGameplayAttributeData& OldNullifyDarkDamage);

	// =========================
	// Damage Absorbs
	// =========================
	UPROPERTY(BlueprintReadOnly, Category="Stats|Combat|DamageAbsorbs", ReplicatedUsing=OnRep_AbsorbDamage)
	FGameplayAttributeData AbsorbDamage;
	ATTRIBUTE_ACCESSORS(USignetCombatAttributeSet, AbsorbDamage);
	UFUNCTION()
	void OnRep_AbsorbDamage(const FGameplayAttributeData& OldAbsorbDamage);

	UPROPERTY(BlueprintReadOnly, Category="Stats|Combat|DamageAbsorbs", ReplicatedUsing=OnRep_AbsorbPhysicalDamage)
	FGameplayAttributeData AbsorbPhysicalDamage;
	ATTRIBUTE_ACCESSORS(USignetCombatAttributeSet, AbsorbPhysicalDamage);
	UFUNCTION()
	void OnRep_AbsorbPhysicalDamage(const FGameplayAttributeData& OldAbsorbPhysicalDamage);

	UPROPERTY(BlueprintReadOnly, Category="Stats|Combat|DamageAbsorbs", ReplicatedUsing=OnRep_AbsorbMagicDamage)
	FGameplayAttributeData AbsorbMagicDamage;
	ATTRIBUTE_ACCESSORS(USignetCombatAttributeSet, AbsorbMagicDamage);
	UFUNCTION()
	void OnRep_AbsorbMagicDamage(const FGameplayAttributeData& OldAbsorbMagicDamage);

	UPROPERTY(BlueprintReadOnly, Category="Stats|Combat|DamageAbsorbs", ReplicatedUsing=OnRep_AbsorbFireDamage)
	FGameplayAttributeData AbsorbFireDamage;
	ATTRIBUTE_ACCESSORS(USignetCombatAttributeSet, AbsorbFireDamage);
	UFUNCTION()
	void OnRep_AbsorbFireDamage(const FGameplayAttributeData& OldAbsorbFireDamage);

	UPROPERTY(BlueprintReadOnly, Category="Stats|Combat|DamageAbsorbs", ReplicatedUsing=OnRep_AbsorbIceDamage)
	FGameplayAttributeData AbsorbIceDamage;
	ATTRIBUTE_ACCESSORS(USignetCombatAttributeSet, AbsorbIceDamage);
	UFUNCTION()
	void OnRep_AbsorbIceDamage(const FGameplayAttributeData& OldAbsorbIceDamage);

	UPROPERTY(BlueprintReadOnly, Category="Stats|Combat|DamageAbsorbs", ReplicatedUsing=OnRep_AbsorbWindDamage)
	FGameplayAttributeData AbsorbWindDamage;
	ATTRIBUTE_ACCESSORS(USignetCombatAttributeSet, AbsorbWindDamage);
	UFUNCTION()
	void OnRep_AbsorbWindDamage(const FGameplayAttributeData& OldAbsorbWindDamage);

	UPROPERTY(BlueprintReadOnly, Category="Stats|Combat|DamageAbsorbs", ReplicatedUsing=OnRep_AbsorbEarthDamage)
	FGameplayAttributeData AbsorbEarthDamage;
	ATTRIBUTE_ACCESSORS(USignetCombatAttributeSet, AbsorbEarthDamage);
	UFUNCTION()
	void OnRep_AbsorbEarthDamage(const FGameplayAttributeData& OldAbsorbEarthDamage);

	UPROPERTY(BlueprintReadOnly, Category="Stats|Combat|DamageAbsorbs", ReplicatedUsing=OnRep_AbsorbLightningDamage)
	FGameplayAttributeData AbsorbLightningDamage;
	ATTRIBUTE_ACCESSORS(USignetCombatAttributeSet, AbsorbLightningDamage);
	UFUNCTION()
	void OnRep_AbsorbLightningDamage(const FGameplayAttributeData& OldAbsorbLightningDamage);

	UPROPERTY(BlueprintReadOnly, Category="Stats|Combat|DamageAbsorbs", ReplicatedUsing=OnRep_AbsorbWaterDamage)
	FGameplayAttributeData AbsorbWaterDamage;
	ATTRIBUTE_ACCESSORS(USignetCombatAttributeSet, AbsorbWaterDamage);
	UFUNCTION()
	void OnRep_AbsorbWaterDamage(const FGameplayAttributeData& OldAbsorbWaterDamage);

	UPROPERTY(BlueprintReadOnly, Category="Stats|Combat|DamageAbsorbs", ReplicatedUsing=OnRep_AbsorbLightDamage)
	FGameplayAttributeData AbsorbLightDamage;
	ATTRIBUTE_ACCESSORS(USignetCombatAttributeSet, AbsorbLightDamage);
	UFUNCTION()
	void OnRep_AbsorbLightDamage(const FGameplayAttributeData& OldAbsorbLightDamage);

	UPROPERTY(BlueprintReadOnly, Category="Stats|Combat|DamageAbsorbs", ReplicatedUsing=OnRep_AbsorbDarkDamage)
	FGameplayAttributeData AbsorbDarkDamage;
	ATTRIBUTE_ACCESSORS(USignetCombatAttributeSet, AbsorbDarkDamage);
	UFUNCTION()
	void OnRep_AbsorbDarkDamage(const FGameplayAttributeData& OldAbsorbDarkDamage);

	// =========================
	// Attack Speed
	// =========================
	UPROPERTY(BlueprintReadOnly, Category="Stats|Combat|Offensive", ReplicatedUsing=OnRep_Delay)
	FGameplayAttributeData Delay;
	ATTRIBUTE_ACCESSORS(USignetCombatAttributeSet, Delay);
	UFUNCTION()
	void OnRep_Delay(const FGameplayAttributeData& OldDelay);

	UPROPERTY(BlueprintReadOnly, Category="Stats|Combat|Offensive", ReplicatedUsing=OnRep_DelayPercent)
	FGameplayAttributeData DelayPercent;
	ATTRIBUTE_ACCESSORS(USignetCombatAttributeSet, DelayPercent);
	UFUNCTION()
	void OnRep_DelayPercent(const FGameplayAttributeData& OldDelayPercent);

	UPROPERTY(BlueprintReadOnly, Category="Stats|Combat|Offensive", ReplicatedUsing=OnRep_RangedDelay)
	FGameplayAttributeData RangedDelay;
	ATTRIBUTE_ACCESSORS(USignetCombatAttributeSet, RangedDelay);
	UFUNCTION()
	void OnRep_RangedDelay(const FGameplayAttributeData& OldRangedDelay);

	UPROPERTY(BlueprintReadOnly, Category="Stats|Combat|Offensive", ReplicatedUsing=OnRep_RangedDelayPercent)
	FGameplayAttributeData RangedDelayPercent;
	ATTRIBUTE_ACCESSORS(USignetCombatAttributeSet, RangedDelayPercent);
	UFUNCTION()
	void OnRep_RangedDelayPercent(const FGameplayAttributeData& OldRangedDelayPercent);

	UPROPERTY(BlueprintReadOnly, Category="Stats|Combat|Offensive", ReplicatedUsing=OnRep_AbilityHaste)
	FGameplayAttributeData AbilityHaste;
	ATTRIBUTE_ACCESSORS(USignetCombatAttributeSet, AbilityHaste);
	UFUNCTION()
	void OnRep_AbilityHaste(const FGameplayAttributeData& OldAbilityHaste);

	UPROPERTY(BlueprintReadOnly, Category="Stats|Combat|Offensive", ReplicatedUsing=OnRep_MagicHaste)
	FGameplayAttributeData MagicHaste;
	ATTRIBUTE_ACCESSORS(USignetCombatAttributeSet, MagicHaste);
	UFUNCTION()
	void OnRep_MagicHaste(const FGameplayAttributeData& OldMagicHaste);

	UPROPERTY(BlueprintReadOnly, Category="Stats|Combat|Offensive", ReplicatedUsing=OnRep_GearHaste)
	FGameplayAttributeData GearHaste;
	ATTRIBUTE_ACCESSORS(USignetCombatAttributeSet, GearHaste);
	UFUNCTION()
	void OnRep_GearHaste(const FGameplayAttributeData& OldGearHaste);

	UPROPERTY(BlueprintReadOnly, Category="Stats|Combat|Offensive", ReplicatedUsing=OnRep_TwoHandAbilityHaste)
	FGameplayAttributeData TwoHandAbilityHaste;
	ATTRIBUTE_ACCESSORS(USignetCombatAttributeSet, TwoHandAbilityHaste);
	UFUNCTION()
	void OnRep_TwoHandAbilityHaste(const FGameplayAttributeData& OldTwoHandAbilityHaste);

	UPROPERTY(BlueprintReadOnly, Category="Stats|Combat|Offensive", ReplicatedUsing=OnRep_DualWield)
	FGameplayAttributeData DualWield;
	ATTRIBUTE_ACCESSORS(USignetCombatAttributeSet, DualWield);
	UFUNCTION()
	void OnRep_DualWield(const FGameplayAttributeData& OldDualWield);

	UPROPERTY(BlueprintReadOnly, Category="Stats|Combat|Offensive", ReplicatedUsing=OnRep_MartialArts)
	FGameplayAttributeData MartialArts;
	ATTRIBUTE_ACCESSORS(USignetCombatAttributeSet, MartialArts);
	UFUNCTION()
	void OnRep_MartialArts(const FGameplayAttributeData& OldMartialArts);

	// =========================
	// Attack Mods
	// =========================
	UPROPERTY(BlueprintReadOnly, Category="Stats|Combat|Offensive", ReplicatedUsing=OnRep_Smite)
	FGameplayAttributeData Smite;
	ATTRIBUTE_ACCESSORS(USignetCombatAttributeSet, Smite);
	UFUNCTION()
	void OnRep_Smite(const FGameplayAttributeData& OldSmite);

	UPROPERTY(BlueprintReadOnly, Category="Stats|Combat|Offensive", ReplicatedUsing=OnRep_FencerCritHitRate)
	FGameplayAttributeData FencerCritHitRate;
	ATTRIBUTE_ACCESSORS(USignetCombatAttributeSet, FencerCritHitRate);
	UFUNCTION()
	void OnRep_FencerCritHitRate(const FGameplayAttributeData& OldFencerCritHitRate);

	UPROPERTY(BlueprintReadOnly, Category="Stats|Combat|Offensive", ReplicatedUsing=OnRep_CriticalHitRate)
	FGameplayAttributeData CriticalHitRate;
	ATTRIBUTE_ACCESSORS(USignetCombatAttributeSet, CriticalHitRate);
	UFUNCTION()
	void OnRep_CriticalHitRate(const FGameplayAttributeData& OldCriticalHitRate);

	UPROPERTY(BlueprintReadOnly, Category="Stats|Combat|Offensive", ReplicatedUsing=OnRep_CriticalHitDamage)
	FGameplayAttributeData CriticalHitDamage;
	ATTRIBUTE_ACCESSORS(USignetCombatAttributeSet, CriticalHitDamage);
	UFUNCTION()
	void OnRep_CriticalHitDamage(const FGameplayAttributeData& OldCriticalHitDamage);

	UPROPERTY(BlueprintReadOnly, Category="Stats|Combat|Offensive", ReplicatedUsing=OnRep_RangedCritHitDamage)
	FGameplayAttributeData RangedCritHitDamage;
	ATTRIBUTE_ACCESSORS(USignetCombatAttributeSet, RangedCritHitDamage);
	UFUNCTION()
	void OnRep_RangedCritHitDamage(const FGameplayAttributeData& OldRangedCritHitDamage);

	UPROPERTY(BlueprintReadOnly, Category="Stats|Combat|Offensive", ReplicatedUsing=OnRep_CritHitEvasion)
	FGameplayAttributeData CritHitEvasion;
	ATTRIBUTE_ACCESSORS(USignetCombatAttributeSet, CritHitEvasion);
	UFUNCTION()
	void OnRep_CritHitEvasion(const FGameplayAttributeData& OldCritHitEvasion);

	UPROPERTY(BlueprintReadOnly, Category="Stats|Combat|Offensive", ReplicatedUsing=OnRep_CritHitDefense)
	FGameplayAttributeData CritHitDefense;
	ATTRIBUTE_ACCESSORS(USignetCombatAttributeSet, CritHitDefense);
	UFUNCTION()
	void OnRep_CritHitDefense(const FGameplayAttributeData& OldCritHitDefense);

	UPROPERTY(BlueprintReadOnly, Category="Stats|Combat|Offensive", ReplicatedUsing=OnRep_MagicCritHitRate)
	FGameplayAttributeData MagicCritHitRate;
	ATTRIBUTE_ACCESSORS(USignetCombatAttributeSet, MagicCritHitRate);
	UFUNCTION()
	void OnRep_MagicCritHitRate(const FGameplayAttributeData& OldMagicCritHitRate);

	UPROPERTY(BlueprintReadOnly, Category="Stats|Combat|Offensive", ReplicatedUsing=OnRep_MagicCritHitDamage)
	FGameplayAttributeData MagicCritHitDamage;
	ATTRIBUTE_ACCESSORS(USignetCombatAttributeSet, MagicCritHitDamage);
	UFUNCTION()
	void OnRep_MagicCritHitDamage(const FGameplayAttributeData& OldMagicCritHitDamage);

	UPROPERTY(BlueprintReadOnly, Category="Stats|Combat|Offensive", ReplicatedUsing=OnRep_MagicDamage)
	FGameplayAttributeData MagicDamage;
	ATTRIBUTE_ACCESSORS(USignetCombatAttributeSet, MagicDamage);
	UFUNCTION()
	void OnRep_MagicDamage(const FGameplayAttributeData& OldMagicDamage);

	UPROPERTY(BlueprintReadOnly, Category="Stats|Combat|Offensive", ReplicatedUsing=OnRep_KickDamage)
	FGameplayAttributeData KickDamage;
	ATTRIBUTE_ACCESSORS(USignetCombatAttributeSet, KickDamage);
	UFUNCTION()
	void OnRep_KickDamage(const FGameplayAttributeData& OldKickDamage);

	// =========================
	// Cure Mods
	// =========================
	UPROPERTY(BlueprintReadOnly, Category="Stats|Combat|Healing", ReplicatedUsing=OnRep_CurePotency)
	FGameplayAttributeData CurePotency;
	ATTRIBUTE_ACCESSORS(USignetCombatAttributeSet, CurePotency);
	UFUNCTION()
	void OnRep_CurePotency(const FGameplayAttributeData& OldCurePotency);

	UPROPERTY(BlueprintReadOnly, Category="Stats|Combat|Healing", ReplicatedUsing=OnRep_CurePotencyII)
	FGameplayAttributeData CurePotencyII;
	ATTRIBUTE_ACCESSORS(USignetCombatAttributeSet, CurePotencyII);
	UFUNCTION()
	void OnRep_CurePotencyII(const FGameplayAttributeData& OldCurePotencyII);

	UPROPERTY(BlueprintReadOnly, Category="Stats|Combat|Healing", ReplicatedUsing=OnRep_CurePotencyRecieved)
	FGameplayAttributeData CurePotencyRecieved;
	ATTRIBUTE_ACCESSORS(USignetCombatAttributeSet, CurePotencyRecieved);
	UFUNCTION()
	void OnRep_CurePotencyRecieved(const FGameplayAttributeData& OldCurePotencyRecieved);

	UPROPERTY(BlueprintReadOnly, Category="Stats|Combat|Healing", ReplicatedUsing=OnRep_CurePotencyBonus)
	FGameplayAttributeData CurePotencyBonus;
	ATTRIBUTE_ACCESSORS(USignetCombatAttributeSet, CurePotencyBonus);
	UFUNCTION()
	void OnRep_CurePotencyBonus(const FGameplayAttributeData& OldCurePotencyBonus);

	// =========================
	// Multi Attack
	// =========================
	UPROPERTY(BlueprintReadOnly, Category="Stats|Combat|Offensive", ReplicatedUsing=OnRep_MaxSwings)
	FGameplayAttributeData MaxSwings;
	ATTRIBUTE_ACCESSORS(USignetCombatAttributeSet, MaxSwings);
	UFUNCTION()
	void OnRep_MaxSwings(const FGameplayAttributeData& OldMaxSwings);

	UPROPERTY(BlueprintReadOnly, Category="Stats|Combat|Offensive", ReplicatedUsing=OnRep_AdditionalSwingChance)
	FGameplayAttributeData AdditionalSwingChance;
	ATTRIBUTE_ACCESSORS(USignetCombatAttributeSet, AdditionalSwingChance);
	UFUNCTION()
	void OnRep_AdditionalSwingChance(const FGameplayAttributeData& OldAdditionalSwingChance);

	UPROPERTY(BlueprintReadOnly, Category="Stats|Combat|Offensive", ReplicatedUsing=OnRep_DoubleAttackRate)
	FGameplayAttributeData DoubleAttackRate;
	ATTRIBUTE_ACCESSORS(USignetCombatAttributeSet, DoubleAttackRate);
	UFUNCTION()
	void OnRep_DoubleAttackRate(const FGameplayAttributeData& OldDoubleAttackRate);

	UPROPERTY(BlueprintReadOnly, Category="Stats|Combat|Offensive", ReplicatedUsing=OnRep_DoubleAttackDamage)
	FGameplayAttributeData DoubleAttackDamage;
	ATTRIBUTE_ACCESSORS(USignetCombatAttributeSet, DoubleAttackDamage);
	UFUNCTION()
	void OnRep_DoubleAttackDamage(const FGameplayAttributeData& OldDoubleAttackDamage);

	UPROPERTY(BlueprintReadOnly, Category="Stats|Combat|Offensive", ReplicatedUsing=OnRep_TripleAttackRate)
	FGameplayAttributeData TripleAttackRate;
	ATTRIBUTE_ACCESSORS(USignetCombatAttributeSet, TripleAttackRate);
	UFUNCTION()
	void OnRep_TripleAttackRate(const FGameplayAttributeData& OldTripleAttackRate);

	UPROPERTY(BlueprintReadOnly, Category="Stats|Combat|Offensive", ReplicatedUsing=OnRep_QuadAttackRate)
	FGameplayAttributeData QuadAttackRate;
	ATTRIBUTE_ACCESSORS(USignetCombatAttributeSet, QuadAttackRate);
	UFUNCTION()
	void OnRep_QuadAttackRate(const FGameplayAttributeData& OldQuadAttackRate);

	UPROPERTY(BlueprintReadOnly, Category="Stats|Combat|Offensive", ReplicatedUsing=OnRep_ExtraKickAttack)
	FGameplayAttributeData ExtraKickAttack;
	ATTRIBUTE_ACCESSORS(USignetCombatAttributeSet, ExtraKickAttack);
	UFUNCTION()
	void OnRep_ExtraKickAttack(const FGameplayAttributeData& OldExtraKickAttack);

	UPROPERTY(BlueprintReadOnly, Category="Stats|Combat|Offensive", ReplicatedUsing=OnRep_ExtraDualWieldAttack)
	FGameplayAttributeData ExtraDualWieldAttack;
	ATTRIBUTE_ACCESSORS(USignetCombatAttributeSet, ExtraDualWieldAttack);
	UFUNCTION()
	void OnRep_ExtraDualWieldAttack(const FGameplayAttributeData& OldExtraDualWieldAttack);

	// =========================
	// Spell Stuff
	// =========================
	UPROPERTY(BlueprintReadOnly, Category="Stats|Combat|Magic", ReplicatedUsing=OnRep_ClearMind)
	FGameplayAttributeData ClearMind;
	ATTRIBUTE_ACCESSORS(USignetCombatAttributeSet, ClearMind);
	UFUNCTION()
	void OnRep_ClearMind(const FGameplayAttributeData& OldClearMind);

	UPROPERTY(BlueprintReadOnly, Category="Stats|Combat|Magic", ReplicatedUsing=OnRep_ConserveMP)
	FGameplayAttributeData ConserveMP;
	ATTRIBUTE_ACCESSORS(USignetCombatAttributeSet, ConserveMP);
	UFUNCTION()
	void OnRep_ConserveMP(const FGameplayAttributeData& OldConserveMP);

	UPROPERTY(BlueprintReadOnly, Category="Stats|Combat|Magic", ReplicatedUsing=OnRep_Blink)
	FGameplayAttributeData Blink;
	ATTRIBUTE_ACCESSORS(USignetCombatAttributeSet, Blink);
	UFUNCTION()
	void OnRep_Blink(const FGameplayAttributeData& OldBlink);

	UPROPERTY(BlueprintReadOnly, Category="Stats|Combat|Magic", ReplicatedUsing=OnRep_Stoneskin)
	FGameplayAttributeData Stoneskin;
	ATTRIBUTE_ACCESSORS(USignetCombatAttributeSet, Stoneskin);
	UFUNCTION()
	void OnRep_Stoneskin(const FGameplayAttributeData& OldStoneskin);

	UPROPERTY(BlueprintReadOnly, Category="Stats|Combat|Magic", ReplicatedUsing=OnRep_EnfeeblingMagicPotency)
	FGameplayAttributeData EnfeeblingMagicPotency;
	ATTRIBUTE_ACCESSORS(USignetCombatAttributeSet, EnfeeblingMagicPotency);
	UFUNCTION()
	void OnRep_EnfeeblingMagicPotency(const FGameplayAttributeData& OldEnfeeblingMagicPotency);

	UPROPERTY(BlueprintReadOnly, Category="Stats|Combat|Magic", ReplicatedUsing=OnRep_EnfeeblingMagicDuration)
	FGameplayAttributeData EnfeeblingMagicDuration;
	ATTRIBUTE_ACCESSORS(USignetCombatAttributeSet, EnfeeblingMagicDuration);
	UFUNCTION()
	void OnRep_EnfeeblingMagicDuration(const FGameplayAttributeData& OldEnfeeblingMagicDuration);

	UPROPERTY(BlueprintReadOnly, Category="Stats|Combat|Magic", ReplicatedUsing=OnRep_EnhancingMagicPotency)
	FGameplayAttributeData EnhancingMagicPotency;
	ATTRIBUTE_ACCESSORS(USignetCombatAttributeSet, EnhancingMagicPotency);
	UFUNCTION()
	void OnRep_EnhancingMagicPotency(const FGameplayAttributeData& OldEnhancingMagicPotency);

	UPROPERTY(BlueprintReadOnly, Category="Stats|Combat|Magic", ReplicatedUsing=OnRep_EnhancingMagicDuration)
	FGameplayAttributeData EnhancingMagicDuration;
	ATTRIBUTE_ACCESSORS(USignetCombatAttributeSet, EnhancingMagicDuration);
	UFUNCTION()
	void OnRep_EnhancingMagicDuration(const FGameplayAttributeData& OldEnhancingMagicDuration);

	// =========================
	// Ninjutsu
	// =========================
	UPROPERTY(BlueprintReadOnly, Category="Stats|Combat|Ninjutsu", ReplicatedUsing=OnRep_Utsusemi)
	FGameplayAttributeData Utsusemi;
	ATTRIBUTE_ACCESSORS(USignetCombatAttributeSet, Utsusemi);
	UFUNCTION()
	void OnRep_Utsusemi(const FGameplayAttributeData& OldUtsusemi);

	UPROPERTY(BlueprintReadOnly, Category="Stats|Combat|Ninjutsu", ReplicatedUsing=OnRep_UtsusemiBonus)
	FGameplayAttributeData UtsusemiBonus;
	ATTRIBUTE_ACCESSORS(USignetCombatAttributeSet, UtsusemiBonus);
	UFUNCTION()
	void OnRep_UtsusemiBonus(const FGameplayAttributeData& OldUtsusemiBonus);

	UPROPERTY(BlueprintReadOnly, Category="Stats|Combat|Ninjutsu", ReplicatedUsing=OnRep_NinjaTool)
	FGameplayAttributeData NinjaTool;
	ATTRIBUTE_ACCESSORS(USignetCombatAttributeSet, NinjaTool);
	UFUNCTION()
	void OnRep_NinjaTool(const FGameplayAttributeData& OldNinjaTool);

	UPROPERTY(BlueprintReadOnly, Category="Stats|Combat|Ninjutsu", ReplicatedUsing=OnRep_NinjutsuBonus)
	FGameplayAttributeData NinjutsuBonus;
	ATTRIBUTE_ACCESSORS(USignetCombatAttributeSet, NinjutsuBonus);
	UFUNCTION()
	void OnRep_NinjutsuBonus(const FGameplayAttributeData& OldNinjutsuBonus);

	UPROPERTY(BlueprintReadOnly, Category="Stats|Combat|Ninjutsu", ReplicatedUsing=OnRep_NinjutsuBonusInnin)
	FGameplayAttributeData NinjutsuBonusInnin;
	ATTRIBUTE_ACCESSORS(USignetCombatAttributeSet, NinjutsuBonusInnin);
	UFUNCTION()
	void OnRep_NinjutsuBonusInnin(const FGameplayAttributeData& OldNinjutsuBonusInnin);

	UPROPERTY(BlueprintReadOnly, Category="Stats|Combat|Ninjutsu", ReplicatedUsing=OnRep_Daken)
	FGameplayAttributeData Daken;
	ATTRIBUTE_ACCESSORS(USignetCombatAttributeSet, Daken);
	UFUNCTION()
	void OnRep_Daken(const FGameplayAttributeData& OldDaken);

	UPROPERTY(BlueprintReadOnly, Category="Stats|Combat|Ninjutsu", ReplicatedUsing=OnRep_NinjutsuDuration)
	FGameplayAttributeData NinjutsuDuration;
	ATTRIBUTE_ACCESSORS(USignetCombatAttributeSet, NinjutsuDuration);
	UFUNCTION()
	void OnRep_NinjutsuDuration(const FGameplayAttributeData& OldNinjutsuDuration);

	// =========================
	// Casting Times
	// =========================
	UPROPERTY(BlueprintReadOnly, Category="Stats|Combat|Offensive", ReplicatedUsing=OnRep_QuickMagic)
	FGameplayAttributeData QuickMagic;
	ATTRIBUTE_ACCESSORS(USignetCombatAttributeSet, QuickMagic);
	UFUNCTION()
	void OnRep_QuickMagic(const FGameplayAttributeData& OldQuickMagic);

	UPROPERTY(BlueprintReadOnly, Category="Stats|Combat|Offensive", ReplicatedUsing=OnRep_FastCast)
	FGameplayAttributeData FastCast;
	ATTRIBUTE_ACCESSORS(USignetCombatAttributeSet, FastCast);
	UFUNCTION()
	void OnRep_FastCast(const FGameplayAttributeData& OldFastCast);

	UPROPERTY(BlueprintReadOnly, Category="Stats|Combat|Offensive", ReplicatedUsing=OnRep_UncappedFastCast)
	FGameplayAttributeData UncappedFastCast;
	ATTRIBUTE_ACCESSORS(USignetCombatAttributeSet, UncappedFastCast);
	UFUNCTION()
	void OnRep_UncappedFastCast(const FGameplayAttributeData& OldUncappedFastCast);

	UPROPERTY(BlueprintReadOnly, Category="Stats|Combat|Offensive", ReplicatedUsing=OnRep_GrimoireCast)
	FGameplayAttributeData GrimoireCast;
	ATTRIBUTE_ACCESSORS(USignetCombatAttributeSet, GrimoireCast);
	UFUNCTION()
	void OnRep_GrimoireCast(const FGameplayAttributeData& OldGrimoireCast);

	UPROPERTY(BlueprintReadOnly, Category="Stats|Combat|Offensive", ReplicatedUsing=OnRep_CureCastTime)
	FGameplayAttributeData CureCastTime;
	ATTRIBUTE_ACCESSORS(USignetCombatAttributeSet, CureCastTime);
	UFUNCTION()
	void OnRep_CureCastTime(const FGameplayAttributeData& OldCureCastTime);

	UPROPERTY(BlueprintReadOnly, Category="Stats|Combat|Offensive", ReplicatedUsing=OnRep_ElementalCastTime)
	FGameplayAttributeData ElementalCastTime;
	ATTRIBUTE_ACCESSORS(USignetCombatAttributeSet, ElementalCastTime);
	UFUNCTION()
	void OnRep_ElementalCastTime(const FGameplayAttributeData& OldElementalCastTime);

	UPROPERTY(BlueprintReadOnly, Category="Stats|Combat|Offensive", ReplicatedUsing=OnRep_SongCastTime)
	FGameplayAttributeData SongCastTime;
	ATTRIBUTE_ACCESSORS(USignetCombatAttributeSet, SongCastTime);
	UFUNCTION()
	void OnRep_SongCastTime(const FGameplayAttributeData& OldSongCastTime);

	UPROPERTY(BlueprintReadOnly, Category="Stats|Combat|Offensive", ReplicatedUsing=OnRep_SpellInterruptionRate)
	FGameplayAttributeData SpellInterruptionRate;
	ATTRIBUTE_ACCESSORS(USignetCombatAttributeSet, SpellInterruptionRate);
	UFUNCTION()
	void OnRep_SpellInterruptionRate(const FGameplayAttributeData& OldSpellInterruptionRate);

	// =========================
	// Skill Chains
	// =========================
	UPROPERTY(BlueprintReadOnly, Category="Stats|Combat|Offensive", ReplicatedUsing=OnRep_SKillChainBonus)
	FGameplayAttributeData SKillChainBonus;
	ATTRIBUTE_ACCESSORS(USignetCombatAttributeSet, SKillChainBonus);
	UFUNCTION()
	void OnRep_SKillChainBonus(const FGameplayAttributeData& OldSKillChainBonus);

	UPROPERTY(BlueprintReadOnly, Category="Stats|Combat|Offensive", ReplicatedUsing=OnRep_SkillChainDamage)
	FGameplayAttributeData SkillChainDamage;
	ATTRIBUTE_ACCESSORS(USignetCombatAttributeSet, SkillChainDamage);
	UFUNCTION()
	void OnRep_SkillChainDamage(const FGameplayAttributeData& OldSkillChainDamage);

	UPROPERTY(BlueprintReadOnly, Category="Stats|Combat|Offensive", ReplicatedUsing=OnRep_SKillChainDuration)
	FGameplayAttributeData SKillChainDuration;
	ATTRIBUTE_ACCESSORS(USignetCombatAttributeSet, SKillChainDuration);
	UFUNCTION()
	void OnRep_SKillChainDuration(const FGameplayAttributeData& OldSKillChainDuration);

	// =========================
	// Killer Effects
	// =========================
	UPROPERTY(BlueprintReadOnly, Category="Stats|Combat|Offensive", ReplicatedUsing=OnRep_VerminKiller)
	FGameplayAttributeData VerminKiller;
	ATTRIBUTE_ACCESSORS(USignetCombatAttributeSet, VerminKiller);
	UFUNCTION()
	void OnRep_VerminKiller(const FGameplayAttributeData& OldVerminKiller);

	UPROPERTY(BlueprintReadOnly, Category="Stats|Combat|Offensive", ReplicatedUsing=OnRep_BirdKiller)
	FGameplayAttributeData BirdKiller;
	ATTRIBUTE_ACCESSORS(USignetCombatAttributeSet, BirdKiller);
	UFUNCTION()
	void OnRep_BirdKiller(const FGameplayAttributeData& OldBirdKiller);

	UPROPERTY(BlueprintReadOnly, Category="Stats|Combat|Offensive", ReplicatedUsing=OnRep_AmorphKiller)
	FGameplayAttributeData AmorphKiller;
	ATTRIBUTE_ACCESSORS(USignetCombatAttributeSet, AmorphKiller);
	UFUNCTION()
	void OnRep_AmorphKiller(const FGameplayAttributeData& OldAmorphKiller);

	UPROPERTY(BlueprintReadOnly, Category="Stats|Combat|Offensive", ReplicatedUsing=OnRep_LizardKiller)
	FGameplayAttributeData LizardKiller;
	ATTRIBUTE_ACCESSORS(USignetCombatAttributeSet, LizardKiller);
	UFUNCTION()
	void OnRep_LizardKiller(const FGameplayAttributeData& OldLizardKiller);

	UPROPERTY(BlueprintReadOnly, Category="Stats|Combat|Offensive", ReplicatedUsing=OnRep_AquanKiller)
	FGameplayAttributeData AquanKiller;
	ATTRIBUTE_ACCESSORS(USignetCombatAttributeSet, AquanKiller);
	UFUNCTION()
	void OnRep_AquanKiller(const FGameplayAttributeData& OldAquanKiller);

	UPROPERTY(BlueprintReadOnly, Category="Stats|Combat|Offensive", ReplicatedUsing=OnRep_PlantoidKiller)
	FGameplayAttributeData PlantoidKiller;
	ATTRIBUTE_ACCESSORS(USignetCombatAttributeSet, PlantoidKiller);
	UFUNCTION()
	void OnRep_PlantoidKiller(const FGameplayAttributeData& OldPlantoidKiller);

	UPROPERTY(BlueprintReadOnly, Category="Stats|Combat|Offensive", ReplicatedUsing=OnRep_BeastKiller)
	FGameplayAttributeData BeastKiller;
	ATTRIBUTE_ACCESSORS(USignetCombatAttributeSet, BeastKiller);
	UFUNCTION()
	void OnRep_BeastKiller(const FGameplayAttributeData& OldBeastKiller);

	UPROPERTY(BlueprintReadOnly, Category="Stats|Combat|Offensive", ReplicatedUsing=OnRep_UndeadKiller)
	FGameplayAttributeData UndeadKiller;
	ATTRIBUTE_ACCESSORS(USignetCombatAttributeSet, UndeadKiller);
	UFUNCTION()
	void OnRep_UndeadKiller(const FGameplayAttributeData& OldUndeadKiller);

	UPROPERTY(BlueprintReadOnly, Category="Stats|Combat|Offensive", ReplicatedUsing=OnRep_ArcanaKiller)
	FGameplayAttributeData ArcanaKiller;
	ATTRIBUTE_ACCESSORS(USignetCombatAttributeSet, ArcanaKiller);
	UFUNCTION()
	void OnRep_ArcanaKiller(const FGameplayAttributeData& OldArcanaKiller);

	UPROPERTY(BlueprintReadOnly, Category="Stats|Combat|Offensive", ReplicatedUsing=OnRep_DragonKiller)
	FGameplayAttributeData DragonKiller;
	ATTRIBUTE_ACCESSORS(USignetCombatAttributeSet, DragonKiller);
	UFUNCTION()
	void OnRep_DragonKiller(const FGameplayAttributeData& OldDragonKiller);

	UPROPERTY(BlueprintReadOnly, Category="Stats|Combat|Offensive", ReplicatedUsing=OnRep_DemonKiller)
	FGameplayAttributeData DemonKiller;
	ATTRIBUTE_ACCESSORS(USignetCombatAttributeSet, DemonKiller);
	UFUNCTION()
	void OnRep_DemonKiller(const FGameplayAttributeData& OldDemonKiller);

	UPROPERTY(BlueprintReadOnly, Category="Stats|Combat|Offensive", ReplicatedUsing=OnRep_EmptyKiller)
	FGameplayAttributeData EmptyKiller;
	ATTRIBUTE_ACCESSORS(USignetCombatAttributeSet, EmptyKiller);
	UFUNCTION()
	void OnRep_EmptyKiller(const FGameplayAttributeData& OldEmptyKiller);

	UPROPERTY(BlueprintReadOnly, Category="Stats|Combat|Offensive", ReplicatedUsing=OnRep_HumanoidKiller)
	FGameplayAttributeData HumanoidKiller;
	ATTRIBUTE_ACCESSORS(USignetCombatAttributeSet, HumanoidKiller);
	UFUNCTION()
	void OnRep_HumanoidKiller(const FGameplayAttributeData& OldHumanoidKiller);

	UPROPERTY(BlueprintReadOnly, Category="Stats|Combat|Offensive", ReplicatedUsing=OnRep_LuminianKiller)
	FGameplayAttributeData LuminianKiller;
	ATTRIBUTE_ACCESSORS(USignetCombatAttributeSet, LuminianKiller);
	UFUNCTION()
	void OnRep_LuminianKiller(const FGameplayAttributeData& OldLuminianKiller);

	UPROPERTY(BlueprintReadOnly, Category="Stats|Combat|Offensive", ReplicatedUsing=OnRep_LuminionKiller)
	FGameplayAttributeData LuminionKiller;
	ATTRIBUTE_ACCESSORS(USignetCombatAttributeSet, LuminionKiller);
	UFUNCTION()
	void OnRep_LuminionKiller(const FGameplayAttributeData& OldLuminionKiller);

	// =========================
	// Resistance Effects
	// =========================
	UPROPERTY(BlueprintReadOnly, Category="Stats|Combat|Resistances", ReplicatedUsing=OnRep_SleepResistance)
	FGameplayAttributeData SleepResistance;
	ATTRIBUTE_ACCESSORS(USignetCombatAttributeSet, SleepResistance);
	UFUNCTION()
	void OnRep_SleepResistance(const FGameplayAttributeData& OldSleepResistance);

	UPROPERTY(BlueprintReadOnly, Category="Stats|Combat|Resistances", ReplicatedUsing=OnRep_PoisonResistance)
	FGameplayAttributeData PoisonResistance;
	ATTRIBUTE_ACCESSORS(USignetCombatAttributeSet, PoisonResistance);
	UFUNCTION()
	void OnRep_PoisonResistance(const FGameplayAttributeData& OldPoisonResistance);

	UPROPERTY(BlueprintReadOnly, Category="Stats|Combat|Resistances", ReplicatedUsing=OnRep_ParalyzeResistance)
	FGameplayAttributeData ParalyzeResistance;
	ATTRIBUTE_ACCESSORS(USignetCombatAttributeSet, ParalyzeResistance);
	UFUNCTION()
	void OnRep_ParalyzeResistance(const FGameplayAttributeData& OldParalyzeResistance);

	UPROPERTY(BlueprintReadOnly, Category="Stats|Combat|Resistances", ReplicatedUsing=OnRep_BlindResistance)
	FGameplayAttributeData BlindResistance;
	ATTRIBUTE_ACCESSORS(USignetCombatAttributeSet, BlindResistance);
	UFUNCTION()
	void OnRep_BlindResistance(const FGameplayAttributeData& OldBlindResistance);

	UPROPERTY(BlueprintReadOnly, Category="Stats|Combat|Resistances", ReplicatedUsing=OnRep_SilenceResistance)
	FGameplayAttributeData SilenceResistance;
	ATTRIBUTE_ACCESSORS(USignetCombatAttributeSet, SilenceResistance);
	UFUNCTION()
	void OnRep_SilenceResistance(const FGameplayAttributeData& OldSilenceResistance);

	UPROPERTY(BlueprintReadOnly, Category="Stats|Combat|Resistances", ReplicatedUsing=OnRep_VirusResistance)
	FGameplayAttributeData VirusResistance;
	ATTRIBUTE_ACCESSORS(USignetCombatAttributeSet, VirusResistance);
	UFUNCTION()
	void OnRep_VirusResistance(const FGameplayAttributeData& OldVirusResistance);

	UPROPERTY(BlueprintReadOnly, Category="Stats|Combat|Resistances", ReplicatedUsing=OnRep_PetrifyResistance)
	FGameplayAttributeData PetrifyResistance;
	ATTRIBUTE_ACCESSORS(USignetCombatAttributeSet, PetrifyResistance);
	UFUNCTION()
	void OnRep_PetrifyResistance(const FGameplayAttributeData& OldPetrifyResistance);

	UPROPERTY(BlueprintReadOnly, Category="Stats|Combat|Resistances", ReplicatedUsing=OnRep_BindResistance)
	FGameplayAttributeData BindResistance;
	ATTRIBUTE_ACCESSORS(USignetCombatAttributeSet, BindResistance);
	UFUNCTION()
	void OnRep_BindResistance(const FGameplayAttributeData& OldBindResistance);

	UPROPERTY(BlueprintReadOnly, Category="Stats|Combat|Resistances", ReplicatedUsing=OnRep_CurseResistance)
	FGameplayAttributeData CurseResistance;
	ATTRIBUTE_ACCESSORS(USignetCombatAttributeSet, CurseResistance);
	UFUNCTION()
	void OnRep_CurseResistance(const FGameplayAttributeData& OldCurseResistance);

	UPROPERTY(BlueprintReadOnly, Category="Stats|Combat|Resistances", ReplicatedUsing=OnRep_GravityResistance)
	FGameplayAttributeData GravityResistance;
	ATTRIBUTE_ACCESSORS(USignetCombatAttributeSet, GravityResistance);
	UFUNCTION()
	void OnRep_GravityResistance(const FGameplayAttributeData& OldGravityResistance);

	UPROPERTY(BlueprintReadOnly, Category="Stats|Combat|Resistances", ReplicatedUsing=OnRep_SlowResistance)
	FGameplayAttributeData SlowResistance;
	ATTRIBUTE_ACCESSORS(USignetCombatAttributeSet, SlowResistance);
	UFUNCTION()
	void OnRep_SlowResistance(const FGameplayAttributeData& OldSlowResistance);

	UPROPERTY(BlueprintReadOnly, Category="Stats|Combat|Resistances", ReplicatedUsing=OnRep_StunResistance)
	FGameplayAttributeData StunResistance;
	ATTRIBUTE_ACCESSORS(USignetCombatAttributeSet, StunResistance);
	UFUNCTION()
	void OnRep_StunResistance(const FGameplayAttributeData& OldStunResistance);

	UPROPERTY(BlueprintReadOnly, Category="Stats|Combat|Resistances", ReplicatedUsing=OnRep_CharmResistance)
	FGameplayAttributeData CharmResistance;
	ATTRIBUTE_ACCESSORS(USignetCombatAttributeSet, CharmResistance);
	UFUNCTION()
	void OnRep_CharmResistance(const FGameplayAttributeData& OldCharmResistance);

	UPROPERTY(BlueprintReadOnly, Category="Stats|Combat|Resistances", ReplicatedUsing=OnRep_AmnesiaResistance)
	FGameplayAttributeData AmnesiaResistance;
	ATTRIBUTE_ACCESSORS(USignetCombatAttributeSet, AmnesiaResistance);
	UFUNCTION()
	void OnRep_AmnesiaResistance(const FGameplayAttributeData& OldAmnesiaResistance);

	UPROPERTY(BlueprintReadOnly, Category="Stats|Combat|Resistances", ReplicatedUsing=OnRep_LullabyResistance)
	FGameplayAttributeData LullabyResistance;
	ATTRIBUTE_ACCESSORS(USignetCombatAttributeSet, LullabyResistance);
	UFUNCTION()
	void OnRep_LullabyResistance(const FGameplayAttributeData& OldLullabyResistance);

	UPROPERTY(BlueprintReadOnly, Category="Stats|Combat|Resistances", ReplicatedUsing=OnRep_DeathResistance)
	FGameplayAttributeData DeathResistance;
	ATTRIBUTE_ACCESSORS(USignetCombatAttributeSet, DeathResistance);
	UFUNCTION()
	void OnRep_DeathResistance(const FGameplayAttributeData& OldDeathResistance);

	UPROPERTY(BlueprintReadOnly, Category="Stats|Combat|Resistances", ReplicatedUsing=OnRep_StatusResistance)
	FGameplayAttributeData StatusResistance;
	ATTRIBUTE_ACCESSORS(USignetCombatAttributeSet, StatusResistance);
	UFUNCTION()
	void OnRep_StatusResistance(const FGameplayAttributeData& OldStatusResistance);

	// =========================
	// Enspells
	// =========================
	UPROPERTY(BlueprintReadOnly, Category="Stats|Combat|Enspells", ReplicatedUsing=OnRep_Enspell)
	FGameplayAttributeData Enspell;
	ATTRIBUTE_ACCESSORS(USignetCombatAttributeSet, Enspell);
	UFUNCTION()
	void OnRep_Enspell(const FGameplayAttributeData& OldEnspell);

	UPROPERTY(BlueprintReadOnly, Category="Stats|Combat|Enspells", ReplicatedUsing=OnRep_EnspellDamage)
	FGameplayAttributeData EnspellDamage;
	ATTRIBUTE_ACCESSORS(USignetCombatAttributeSet, EnspellDamage);
	UFUNCTION()
	void OnRep_EnspellDamage(const FGameplayAttributeData& OldEnspellDamage);

	UPROPERTY(BlueprintReadOnly, Category="Stats|Combat|Enspells", ReplicatedUsing=OnRep_EnspellDamageBonus)
	FGameplayAttributeData EnspellDamageBonus;
	ATTRIBUTE_ACCESSORS(USignetCombatAttributeSet, EnspellDamageBonus);
	UFUNCTION()
	void OnRep_EnspellDamageBonus(const FGameplayAttributeData& OldEnspellDamageBonus);

	UPROPERTY(BlueprintReadOnly, Category="Stats|Combat|Enspells", ReplicatedUsing=OnRep_EnspellChance)
	FGameplayAttributeData EnspellChance;
	ATTRIBUTE_ACCESSORS(USignetCombatAttributeSet, EnspellChance);
	UFUNCTION()
	void OnRep_EnspellChance(const FGameplayAttributeData& OldEnspellChance);

	// =========================
	// Spikes
	// =========================
	UPROPERTY(BlueprintReadOnly, Category="Stats|Combat|Spikes", ReplicatedUsing=OnRep_Spikes)
	FGameplayAttributeData Spikes;
	ATTRIBUTE_ACCESSORS(USignetCombatAttributeSet, Spikes);
	UFUNCTION()
	void OnRep_Spikes(const FGameplayAttributeData& OldSpikes);

	UPROPERTY(BlueprintReadOnly, Category="Stats|Combat|Spikes", ReplicatedUsing=OnRep_SpikesDamage)
	FGameplayAttributeData SpikesDamage;
	ATTRIBUTE_ACCESSORS(USignetCombatAttributeSet, SpikesDamage);
	UFUNCTION()
	void OnRep_SpikesDamage(const FGameplayAttributeData& OldSpikesDamage);

	UPROPERTY(BlueprintReadOnly, Category="Stats|Combat|Spikes", ReplicatedUsing=OnRep_SpikesDamageBonus)
	FGameplayAttributeData SpikesDamageBonus;
	ATTRIBUTE_ACCESSORS(USignetCombatAttributeSet, SpikesDamageBonus);
	UFUNCTION()
	void OnRep_SpikesDamageBonus(const FGameplayAttributeData& OldSpikesDamageBonus);

	// =========================
	// Songs
	// =========================
	UPROPERTY(BlueprintReadOnly, Category="Stats|Combat|Songs", ReplicatedUsing=OnRep_MinneEffect)
	FGameplayAttributeData MinneEffect;
	ATTRIBUTE_ACCESSORS(USignetCombatAttributeSet, MinneEffect);
	UFUNCTION()
	void OnRep_MinneEffect(const FGameplayAttributeData& OldMinneEffect);

	UPROPERTY(BlueprintReadOnly, Category="Stats|Combat|Songs", ReplicatedUsing=OnRep_MinuetEffect)
	FGameplayAttributeData MinuetEffect;
	ATTRIBUTE_ACCESSORS(USignetCombatAttributeSet, MinuetEffect);
	UFUNCTION()
	void OnRep_MinuetEffect(const FGameplayAttributeData& OldMinuetEffect);

	UPROPERTY(BlueprintReadOnly, Category="Stats|Combat|Songs", ReplicatedUsing=OnRep_PaeonEffect)
	FGameplayAttributeData PaeonEffect;
	ATTRIBUTE_ACCESSORS(USignetCombatAttributeSet, PaeonEffect);
	UFUNCTION()
	void OnRep_PaeonEffect(const FGameplayAttributeData& OldPaeonEffect);

	UPROPERTY(BlueprintReadOnly, Category="Stats|Combat|Songs", ReplicatedUsing=OnRep_RequiemEffect)
	FGameplayAttributeData RequiemEffect;
	ATTRIBUTE_ACCESSORS(USignetCombatAttributeSet, RequiemEffect);
	UFUNCTION()
	void OnRep_RequiemEffect(const FGameplayAttributeData& OldRequiemEffect);

	UPROPERTY(BlueprintReadOnly, Category="Stats|Combat|Songs", ReplicatedUsing=OnRep_ThrenodyEffect)
	FGameplayAttributeData ThrenodyEffect;
	ATTRIBUTE_ACCESSORS(USignetCombatAttributeSet, ThrenodyEffect);
	UFUNCTION()
	void OnRep_ThrenodyEffect(const FGameplayAttributeData& OldThrenodyEffect);

	UPROPERTY(BlueprintReadOnly, Category="Stats|Combat|Songs", ReplicatedUsing=OnRep_MadrigalEffect)
	FGameplayAttributeData MadrigalEffect;
	ATTRIBUTE_ACCESSORS(USignetCombatAttributeSet, MadrigalEffect);
	UFUNCTION()
	void OnRep_MadrigalEffect(const FGameplayAttributeData& OldMadrigalEffect);

	UPROPERTY(BlueprintReadOnly, Category="Stats|Combat|Songs", ReplicatedUsing=OnRep_MamboEffect)
	FGameplayAttributeData MamboEffect;
	ATTRIBUTE_ACCESSORS(USignetCombatAttributeSet, MamboEffect);
	UFUNCTION()
	void OnRep_MamboEffect(const FGameplayAttributeData& OldMamboEffect);

	UPROPERTY(BlueprintReadOnly, Category="Stats|Combat|Songs", ReplicatedUsing=OnRep_LullabyEffect)
	FGameplayAttributeData LullabyEffect;
	ATTRIBUTE_ACCESSORS(USignetCombatAttributeSet, LullabyEffect);
	UFUNCTION()
	void OnRep_LullabyEffect(const FGameplayAttributeData& OldLullabyEffect);

	UPROPERTY(BlueprintReadOnly, Category="Stats|Combat|Songs", ReplicatedUsing=OnRep_EtudeEffect)
	FGameplayAttributeData EtudeEffect;
	ATTRIBUTE_ACCESSORS(USignetCombatAttributeSet, EtudeEffect);
	UFUNCTION()
	void OnRep_EtudeEffect(const FGameplayAttributeData& OldEtudeEffect);

	UPROPERTY(BlueprintReadOnly, Category="Stats|Combat|Songs", ReplicatedUsing=OnRep_BalladEffect)
	FGameplayAttributeData BalladEffect;
	ATTRIBUTE_ACCESSORS(USignetCombatAttributeSet, BalladEffect);
	UFUNCTION()
	void OnRep_BalladEffect(const FGameplayAttributeData& OldBalladEffect);

	UPROPERTY(BlueprintReadOnly, Category="Stats|Combat|Songs", ReplicatedUsing=OnRep_MarchEffect)
	FGameplayAttributeData MarchEffect;
	ATTRIBUTE_ACCESSORS(USignetCombatAttributeSet, MarchEffect);
	UFUNCTION()
	void OnRep_MarchEffect(const FGameplayAttributeData& OldMarchEffect);

	UPROPERTY(BlueprintReadOnly, Category="Stats|Combat|Songs", ReplicatedUsing=OnRep_FinaleEffect)
	FGameplayAttributeData FinaleEffect;
	ATTRIBUTE_ACCESSORS(USignetCombatAttributeSet, FinaleEffect);
	UFUNCTION()
	void OnRep_FinaleEffect(const FGameplayAttributeData& OldFinaleEffect);

	UPROPERTY(BlueprintReadOnly, Category="Stats|Combat|Songs", ReplicatedUsing=OnRep_CarolEffect)
	FGameplayAttributeData CarolEffect;
	ATTRIBUTE_ACCESSORS(USignetCombatAttributeSet, CarolEffect);
	UFUNCTION()
	void OnRep_CarolEffect(const FGameplayAttributeData& OldCarolEffect);

	UPROPERTY(BlueprintReadOnly, Category="Stats|Combat|Songs", ReplicatedUsing=OnRep_MazurkaEffect)
	FGameplayAttributeData MazurkaEffect;
	ATTRIBUTE_ACCESSORS(USignetCombatAttributeSet, MazurkaEffect);
	UFUNCTION()
	void OnRep_MazurkaEffect(const FGameplayAttributeData& OldMazurkaEffect);

	UPROPERTY(BlueprintReadOnly, Category="Stats|Combat|Songs", ReplicatedUsing=OnRep_ElegyEffect)
	FGameplayAttributeData ElegyEffect;
	ATTRIBUTE_ACCESSORS(USignetCombatAttributeSet, ElegyEffect);
	UFUNCTION()
	void OnRep_ElegyEffect(const FGameplayAttributeData& OldElegyEffect);

	UPROPERTY(BlueprintReadOnly, Category="Stats|Combat|Songs", ReplicatedUsing=OnRep_PreludeEffect)
	FGameplayAttributeData PreludeEffect;
	ATTRIBUTE_ACCESSORS(USignetCombatAttributeSet, PreludeEffect);
	UFUNCTION()
	void OnRep_PreludeEffect(const FGameplayAttributeData& OldPreludeEffect);

	UPROPERTY(BlueprintReadOnly, Category="Stats|Combat|Songs", ReplicatedUsing=OnRep_HymnusEffect)
	FGameplayAttributeData HymnusEffect;
	ATTRIBUTE_ACCESSORS(USignetCombatAttributeSet, HymnusEffect);
	UFUNCTION()
	void OnRep_HymnusEffect(const FGameplayAttributeData& OldHymnusEffect);

	UPROPERTY(BlueprintReadOnly, Category="Stats|Combat|Songs", ReplicatedUsing=OnRep_VirelaiEffect)
	FGameplayAttributeData VirelaiEffect;
	ATTRIBUTE_ACCESSORS(USignetCombatAttributeSet, VirelaiEffect);
	UFUNCTION()
	void OnRep_VirelaiEffect(const FGameplayAttributeData& OldVirelaiEffect);

	UPROPERTY(BlueprintReadOnly, Category="Stats|Combat|Songs", ReplicatedUsing=OnRep_ScherzoEffect)
	FGameplayAttributeData ScherzoEffect;
	ATTRIBUTE_ACCESSORS(USignetCombatAttributeSet, ScherzoEffect);
	UFUNCTION()
	void OnRep_ScherzoEffect(const FGameplayAttributeData& OldScherzoEffect);

	UPROPERTY(BlueprintReadOnly, Category="Stats|Combat|Songs", ReplicatedUsing=OnRep_AllSongsEffect)
	FGameplayAttributeData AllSongsEffect;
	ATTRIBUTE_ACCESSORS(USignetCombatAttributeSet, AllSongsEffect);
	UFUNCTION()
	void OnRep_AllSongsEffect(const FGameplayAttributeData& OldAllSongsEffect);

	UPROPERTY(BlueprintReadOnly, Category="Stats|Combat|Songs", ReplicatedUsing=OnRep_MaximumSongsBonus)
	FGameplayAttributeData MaximumSongsBonus;
	ATTRIBUTE_ACCESSORS(USignetCombatAttributeSet, MaximumSongsBonus);
	UFUNCTION()
	void OnRep_MaximumSongsBonus(const FGameplayAttributeData& OldMaximumSongsBonus);

	UPROPERTY(BlueprintReadOnly, Category="Stats|Combat|Songs", ReplicatedUsing=OnRep_SongDurationBonus)
	FGameplayAttributeData SongDurationBonus;
	ATTRIBUTE_ACCESSORS(USignetCombatAttributeSet, SongDurationBonus);
	UFUNCTION()
	void OnRep_SongDurationBonus(const FGameplayAttributeData& OldSongDurationBonus);

	UPROPERTY(BlueprintReadOnly, Category="Stats|Combat|Songs", ReplicatedUsing=OnRep_SongRecastDelay)
	FGameplayAttributeData SongRecastDelay;
	ATTRIBUTE_ACCESSORS(USignetCombatAttributeSet, SongRecastDelay);
	UFUNCTION()
	void OnRep_SongRecastDelay(const FGameplayAttributeData& OldSongRecastDelay);

	// ---------------- Affinities ----------------
	UPROPERTY(BlueprintReadOnly, Category="Stats|Combat|Affinity", ReplicatedUsing=OnRep_FireAffinityDamage)
	FGameplayAttributeData FireAffinityDamage;
	ATTRIBUTE_ACCESSORS(USignetCombatAttributeSet, FireAffinityDamage);
	UFUNCTION()
	void OnRep_FireAffinityDamage(const FGameplayAttributeData& OldFireAffinityDamage);

	UPROPERTY(BlueprintReadOnly, Category="Stats|Combat|Affinity", ReplicatedUsing=OnRep_FireAffinityAccuracy)
	FGameplayAttributeData FireAffinityAccuracy;
	ATTRIBUTE_ACCESSORS(USignetCombatAttributeSet, FireAffinityAccuracy);
	UFUNCTION()
	void OnRep_FireAffinityAccuracy(const FGameplayAttributeData& OldFireAffinityAccuracy);

	UPROPERTY(BlueprintReadOnly, Category="Stats|Combat|Affinity", ReplicatedUsing=OnRep_FireAffinityPerpetuation)
	FGameplayAttributeData FireAffinityPerpetuation;
	ATTRIBUTE_ACCESSORS(USignetCombatAttributeSet, FireAffinityPerpetuation);
	UFUNCTION()
	void OnRep_FireAffinityPerpetuation(const FGameplayAttributeData& OldFireAffinityPerpetuation);

	UPROPERTY(BlueprintReadOnly, Category="Stats|Combat|Affinity", ReplicatedUsing=OnRep_IceAffinityDamage)
	FGameplayAttributeData IceAffinityDamage;
	ATTRIBUTE_ACCESSORS(USignetCombatAttributeSet, IceAffinityDamage);
	UFUNCTION()
	void OnRep_IceAffinityDamage(const FGameplayAttributeData& OldIceAffinityDamage);

	UPROPERTY(BlueprintReadOnly, Category="Stats|Combat|Affinity", ReplicatedUsing=OnRep_IceAffinityAccuracy)
	FGameplayAttributeData IceAffinityAccuracy;
	ATTRIBUTE_ACCESSORS(USignetCombatAttributeSet, IceAffinityAccuracy);
	UFUNCTION()
	void OnRep_IceAffinityAccuracy(const FGameplayAttributeData& OldIceAffinityAccuracy);

	UPROPERTY(BlueprintReadOnly, Category="Stats|Combat|Affinity", ReplicatedUsing=OnRep_IceAffinityPerpetuation)
	FGameplayAttributeData IceAffinityPerpetuation;
	ATTRIBUTE_ACCESSORS(USignetCombatAttributeSet, IceAffinityPerpetuation);
	UFUNCTION()
	void OnRep_IceAffinityPerpetuation(const FGameplayAttributeData& OldIceAffinityPerpetuation);

	UPROPERTY(BlueprintReadOnly, Category="Stats|Combat|Affinity", ReplicatedUsing=OnRep_WindAffinityDamage)
	FGameplayAttributeData WindAffinityDamage;
	ATTRIBUTE_ACCESSORS(USignetCombatAttributeSet, WindAffinityDamage);
	UFUNCTION()
	void OnRep_WindAffinityDamage(const FGameplayAttributeData& OldWindAffinityDamage);

	UPROPERTY(BlueprintReadOnly, Category="Stats|Combat|Affinity", ReplicatedUsing=OnRep_WindAffinityAccuracy)
	FGameplayAttributeData WindAffinityAccuracy;
	ATTRIBUTE_ACCESSORS(USignetCombatAttributeSet, WindAffinityAccuracy);
	UFUNCTION()
	void OnRep_WindAffinityAccuracy(const FGameplayAttributeData& OldWindAffinityAccuracy);

	UPROPERTY(BlueprintReadOnly, Category="Stats|Combat|Affinity", ReplicatedUsing=OnRep_WindAffinityPerpetuation)
	FGameplayAttributeData WindAffinityPerpetuation;
	ATTRIBUTE_ACCESSORS(USignetCombatAttributeSet, WindAffinityPerpetuation);
	UFUNCTION()
	void OnRep_WindAffinityPerpetuation(const FGameplayAttributeData& OldWindAffinityPerpetuation);

	UPROPERTY(BlueprintReadOnly, Category="Stats|Combat|Affinity", ReplicatedUsing=OnRep_EarthAffinityDamage)
	FGameplayAttributeData EarthAffinityDamage;
	ATTRIBUTE_ACCESSORS(USignetCombatAttributeSet, EarthAffinityDamage);
	UFUNCTION()
	void OnRep_EarthAffinityDamage(const FGameplayAttributeData& OldEarthAffinityDamage);

	UPROPERTY(BlueprintReadOnly, Category="Stats|Combat|Affinity", ReplicatedUsing=OnRep_EarthAffinityAccuracy)
	FGameplayAttributeData EarthAffinityAccuracy;
	ATTRIBUTE_ACCESSORS(USignetCombatAttributeSet, EarthAffinityAccuracy);
	UFUNCTION()
	void OnRep_EarthAffinityAccuracy(const FGameplayAttributeData& OldEarthAffinityAccuracy);

	UPROPERTY(BlueprintReadOnly, Category="Stats|Combat|Affinity", ReplicatedUsing=OnRep_EarthAffinityPerpetuation)
	FGameplayAttributeData EarthAffinityPerpetuation;
	ATTRIBUTE_ACCESSORS(USignetCombatAttributeSet, EarthAffinityPerpetuation);
	UFUNCTION()
	void OnRep_EarthAffinityPerpetuation(const FGameplayAttributeData& OldEarthAffinityPerpetuation);

	UPROPERTY(BlueprintReadOnly, Category="Stats|Combat|Affinity", ReplicatedUsing=OnRep_LightningAffinityDamage)
	FGameplayAttributeData LightningAffinityDamage;
	ATTRIBUTE_ACCESSORS(USignetCombatAttributeSet, LightningAffinityDamage);
	UFUNCTION()
	void OnRep_LightningAffinityDamage(const FGameplayAttributeData& OldLightningAffinityDamage);

	UPROPERTY(BlueprintReadOnly, Category="Stats|Combat|Affinity", ReplicatedUsing=OnRep_LightningAffinityAccuracy)
	FGameplayAttributeData LightningAffinityAccuracy;
	ATTRIBUTE_ACCESSORS(USignetCombatAttributeSet, LightningAffinityAccuracy);
	UFUNCTION()
	void OnRep_LightningAffinityAccuracy(const FGameplayAttributeData& OldLightningAffinityAccuracy);

	UPROPERTY(BlueprintReadOnly, Category="Stats|Combat|Affinity", ReplicatedUsing=OnRep_LightningAffinityPerpetuation)
	FGameplayAttributeData LightningAffinityPerpetuation;
	ATTRIBUTE_ACCESSORS(USignetCombatAttributeSet, LightningAffinityPerpetuation);
	UFUNCTION()
	void OnRep_LightningAffinityPerpetuation(const FGameplayAttributeData& OldLightningAffinityPerpetuation);

	UPROPERTY(BlueprintReadOnly, Category="Stats|Combat|Affinity", ReplicatedUsing=OnRep_WaterAffinityDamage)
	FGameplayAttributeData WaterAffinityDamage;
	ATTRIBUTE_ACCESSORS(USignetCombatAttributeSet, WaterAffinityDamage);
	UFUNCTION()
	void OnRep_WaterAffinityDamage(const FGameplayAttributeData& OldWaterAffinityDamage);

	UPROPERTY(BlueprintReadOnly, Category="Stats|Combat|Affinity", ReplicatedUsing=OnRep_WaterAffinityAccuracy)
	FGameplayAttributeData WaterAffinityAccuracy;
	ATTRIBUTE_ACCESSORS(USignetCombatAttributeSet, WaterAffinityAccuracy);
	UFUNCTION()
	void OnRep_WaterAffinityAccuracy(const FGameplayAttributeData& OldWaterAffinityAccuracy);

	UPROPERTY(BlueprintReadOnly, Category="Stats|Combat|Affinity", ReplicatedUsing=OnRep_WaterAffinityPerpetuation)
	FGameplayAttributeData WaterAffinityPerpetuation;
	ATTRIBUTE_ACCESSORS(USignetCombatAttributeSet, WaterAffinityPerpetuation);
	UFUNCTION()
	void OnRep_WaterAffinityPerpetuation(const FGameplayAttributeData& OldWaterAffinityPerpetuation);

	UPROPERTY(BlueprintReadOnly, Category="Stats|Combat|Affinity", ReplicatedUsing=OnRep_LightAffinityDamage)
	FGameplayAttributeData LightAffinityDamage;
	ATTRIBUTE_ACCESSORS(USignetCombatAttributeSet, LightAffinityDamage);
	UFUNCTION()
	void OnRep_LightAffinityDamage(const FGameplayAttributeData& OldLightAffinityDamage);

	UPROPERTY(BlueprintReadOnly, Category="Stats|Combat|Affinity", ReplicatedUsing=OnRep_LightAffinityAccuracy)
	FGameplayAttributeData LightAffinityAccuracy;
	ATTRIBUTE_ACCESSORS(USignetCombatAttributeSet, LightAffinityAccuracy);
	UFUNCTION()
	void OnRep_LightAffinityAccuracy(const FGameplayAttributeData& OldLightAffinityAccuracy);

	UPROPERTY(BlueprintReadOnly, Category="Stats|Combat|Affinity", ReplicatedUsing=OnRep_LightAffinityPerpetuation)
	FGameplayAttributeData LightAffinityPerpetuation;
	ATTRIBUTE_ACCESSORS(USignetCombatAttributeSet, LightAffinityPerpetuation);
	UFUNCTION()
	void OnRep_LightAffinityPerpetuation(const FGameplayAttributeData& OldLightAffinityPerpetuation);

	UPROPERTY(BlueprintReadOnly, Category="Stats|Combat|Affinity", ReplicatedUsing=OnRep_DarkAffinityDamage)
	FGameplayAttributeData DarkAffinityDamage;
	ATTRIBUTE_ACCESSORS(USignetCombatAttributeSet, DarkAffinityDamage);
	UFUNCTION()
	void OnRep_DarkAffinityDamage(const FGameplayAttributeData& OldDarkAffinityDamage);

	UPROPERTY(BlueprintReadOnly, Category="Stats|Combat|Affinity", ReplicatedUsing=OnRep_DarkAffinityAccuracy)
	FGameplayAttributeData DarkAffinityAccuracy;
	ATTRIBUTE_ACCESSORS(USignetCombatAttributeSet, DarkAffinityAccuracy);
	UFUNCTION()
	void OnRep_DarkAffinityAccuracy(const FGameplayAttributeData& OldDarkAffinityAccuracy);

	UPROPERTY(BlueprintReadOnly, Category="Stats|Combat|Affinity", ReplicatedUsing=OnRep_DarkAffinityPerpetuation)
	FGameplayAttributeData DarkAffinityPerpetuation;
	ATTRIBUTE_ACCESSORS(USignetCombatAttributeSet, DarkAffinityPerpetuation);
	UFUNCTION()
	void OnRep_DarkAffinityPerpetuation(const FGameplayAttributeData& OldDarkAffinityPerpetuation);

	// ---------------- Points ----------------
	UPROPERTY(BlueprintReadOnly, Category="Stats|Combat|Points", ReplicatedUsing=OnRep_ExpBonus)
	FGameplayAttributeData ExpBonus;
	ATTRIBUTE_ACCESSORS(USignetCombatAttributeSet, ExpBonus);
	UFUNCTION()
	void OnRep_ExpBonus(const FGameplayAttributeData& OldExpBonus);

	UPROPERTY(BlueprintReadOnly, Category="Stats|Combat|Points", ReplicatedUsing=OnRep_CapacityBonus)
	FGameplayAttributeData CapacityBonus;
	ATTRIBUTE_ACCESSORS(USignetCombatAttributeSet, CapacityBonus);
	UFUNCTION()
	void OnRep_CapacityBonus(const FGameplayAttributeData& OldCapacityBonus);

	UPROPERTY(BlueprintReadOnly, Category="Stats|Combat|Points", ReplicatedUsing=OnRep_MasterBonus)
	FGameplayAttributeData MasterBonus;
	ATTRIBUTE_ACCESSORS(USignetCombatAttributeSet, MasterBonus);
	UFUNCTION()
	void OnRep_MasterBonus(const FGameplayAttributeData& OldMasterBonus);



	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};

