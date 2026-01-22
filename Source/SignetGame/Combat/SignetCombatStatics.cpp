// Copyright Red Lotus Games, All Rights Reserved.

#include "SignetCombatStatics.h"
#include "SignetGame/Abilities/SignetAbilitySystemComponent.h"
#include "SignetGame/Abilities/Attributes/SignetCombatAttributeSet.h"
#include "SignetGame/Abilities/Attributes/SignetPrimaryAttributeSet.h"
#include "SignetGame/Abilities/Attributes/SignetSkillAttributeSet.h"
#include "SignetGame/Abilities/TagCache.h"

float USignetCombatStatics::CalculateHitRate(const USignetAbilitySystemComponent* AttackerAsc, const USignetAbilitySystemComponent* TargetAsc, bool bIsRanged, int32 AccuracyBonus)
{
	if (!AttackerAsc || !TargetAsc) return 0.0f;

	float Accuracy = bIsRanged ? AttackerAsc->GetNumericAttribute(USignetCombatAttributeSet::GetRangedAccuracyAttribute()) : AttackerAsc->GetNumericAttribute(USignetCombatAttributeSet::GetAccuracyAttribute());
	float Evasion = bIsRanged ? TargetAsc->GetNumericAttribute(USignetCombatAttributeSet::GetRangedEvasionAttribute()) : TargetAsc->GetNumericAttribute(USignetCombatAttributeSet::GetEvasionAttribute());

	Accuracy += AccuracyBonus;

	// FFXI Hit Rate formula: 75% + (Acc - Eva) / 2
	float HitRate = 75.0f + (Accuracy - Evasion) * 0.5f;

	// Critical check for Seigan counter
	bool bIsCountered = false;
	bool bIsCounterCritical = false;
	if (CheckSeiganCounter(AttackerAsc, TargetAsc, bIsCountered, bIsCounterCritical))
	{
		// If countered, we don't hit the target
		return 0.0f;
	}

	// Caps
	float MaxHitRate = 95.0f;
	float MinHitRate = 5.0f;

	return FMath::Clamp(HitRate, MinHitRate, MaxHitRate);
}

float USignetCombatStatics::CalculateDamageRatio(const USignetAbilitySystemComponent* AttackerAsc, const USignetAbilitySystemComponent* TargetAsc, bool bIsCritical, float AttackBonus, FGameplayTag SkillTag, EGearSlot WeaponSlot)
{
	if (!AttackerAsc || !TargetAsc) return 1.0f;

	float Attack = AttackerAsc->GetNumericAttribute(USignetCombatAttributeSet::GetAttackAttribute());
	float Defense = TargetAsc->GetNumericAttribute(USignetCombatAttributeSet::GetDefenseAttribute());

	Attack *= AttackBonus;
	if (Defense <= 0) Defense = 1.0f;

	float Ratio = Attack / Defense;

	// pDIF calculation (Simplified for now, mirroring legacy battleutils::GetDamageRatio)
	float pDIF = Ratio; // Placeholder for more complex pDIF curves

	if (bIsCritical)
	{
		pDIF += 1.0f;
	}

	return FMath::Max(pDIF, 0.0f);
}

int32 USignetCombatStatics::CalculateFSTR(const USignetAbilitySystemComponent* AttackerAsc, const USignetAbilitySystemComponent* TargetAsc, EGearSlot WeaponSlot)
{
	if (!AttackerAsc || !TargetAsc) return 0;

	float AttackerSTR = AttackerAsc->GetNumericAttribute(USignetPrimaryAttributeSet::GetSTRAttribute());
	float TargetVIT = TargetAsc->GetNumericAttribute(USignetPrimaryAttributeSet::GetVITAttribute());

	// FFXI fSTR formula: (STR - VIT + 4) / 4
	int32 fSTR = FMath::FloorToInt((AttackerSTR - TargetVIT + 4.0f) / 4.0f);
	
	return fSTR;
}

int32 USignetCombatStatics::CalculateBaseDamage(const USignetAbilitySystemComponent* AttackerAsc, const USignetAbilitySystemComponent* TargetAsc, EGearSlot WeaponSlot, EPhysicalAttackType AttackType, bool bIsH2H, bool bIsFirstSwing, bool bIsSA, bool bIsTA, float DamageRatio)
{
	if (!AttackerAsc || !TargetAsc) return 0;

	float WeaponDamage = 0.0f;
	if (WeaponSlot == EGearSlot::Main)
		WeaponDamage = AttackerAsc->GetNumericAttribute(USignetCombatAttributeSet::GetWeaponDamageAttribute());
	else if (WeaponSlot == EGearSlot::Sub)
		WeaponDamage = AttackerAsc->GetNumericAttribute(USignetCombatAttributeSet::GetSubWeaponDamageAttribute());

	int32 fSTR = CalculateFSTR(AttackerAsc, TargetAsc, WeaponSlot);
	float BaseDamage = WeaponDamage + fSTR;

	// Ported logic from xi.combat.physical.calculateAttackDamage
	float BonusDamage = 0.0f;

	if (bIsSA && bIsFirstSwing)
	{
		float DEX = AttackerAsc->GetNumericAttribute(USignetPrimaryAttributeSet::GetDEXAttribute());
		BonusDamage += DEX; // SA adds DEX to base damage
	}

	if (bIsTA && bIsFirstSwing)
	{
		float AGI = AttackerAsc->GetNumericAttribute(USignetPrimaryAttributeSet::GetAGIAttribute());
		BonusDamage += AGI; // TA adds AGI to base damage
	}

	BaseDamage += BonusDamage;

	// Apply Ratio
	int32 FinalDamage = FMath::FloorToInt(BaseDamage * DamageRatio);

	// Apply multipliers
	FinalDamage = ApplyDamageMultipliers(AttackerAsc, FinalDamage, AttackType, WeaponSlot, bIsFirstSwing);

	return FMath::Max(FinalDamage, 0);
}

bool USignetCombatStatics::CheckAnticipated(const USignetAbilitySystemComponent* AttackerAsc, const USignetAbilitySystemComponent* TargetAsc)
{
	if (!TargetAsc) return false;
	
	// Check for Third Eye (EFFECT_THIRD_EYE)
	static const FGameplayTag ThirdEyeTag = FGameplayTag::RequestGameplayTag(TEXT("Data.Effect.ThirdEye"));
	if (TargetAsc->HasMatchingGameplayTag(ThirdEyeTag))
	{
		// Logic from xi.combat.physicalHitRate.checkAnticipated
		// In a real implementation, we would consume a shadow or decrease proc chance
		return true;
	}

	return false;
}

bool USignetCombatStatics::CheckSeiganCounter(const USignetAbilitySystemComponent* AttackerAsc, const USignetAbilitySystemComponent* TargetAsc, bool& bOutIsCountered, bool& bOutIsCritical)
{
	bOutIsCountered = false;
	bOutIsCritical = false;

	if (!TargetAsc) return false;

	static const FGameplayTag SeiganTag = FGameplayTag::RequestGameplayTag(TEXT("Data.Effect.Seigan"));
	if (TargetAsc->HasMatchingGameplayTag(SeiganTag))
	{
		// Simplified Seigan counter logic
		float CounterRate = 0.25f; // Placeholder
		if (FMath::FRand() < CounterRate)
		{
			bOutIsCountered = true;
			bOutIsCritical = (FMath::FRand() < 0.2f); // 20% crit chance on counter
			return true;
		}
	}

	return false;
}

float USignetCombatStatics::CalculateKickAttackRate(const USignetAbilitySystemComponent* AttackerAsc)
{
	if (!AttackerAsc) return 0.0f;
	return AttackerAsc->GetNumericAttribute(USignetCombatAttributeSet::GetExtraKickAttackAttribute());
}

float USignetCombatStatics::CalculateDefense(const USignetAbilitySystemComponent* Asc)
{
	if (!Asc) return 0.0f;

	float VIT = Asc->GetNumericAttribute(USignetPrimaryAttributeSet::GetVITAttribute());
	float Level = Asc->GetNumericAttribute(USignetPrimaryAttributeSet::GetJobLevelAttribute());

	// Defense = floor( VIT × 1.5 ) + Level + 8
	return FMath::FloorToFloat(VIT * 1.5f) + Level + 8.0f;
}

float USignetCombatStatics::CalculateAttack(const USignetAbilitySystemComponent* Asc, FGameplayTag SkillTag)
{
	if (!Asc) return 0.0f;

	float STR = Asc->GetNumericAttribute(USignetPrimaryAttributeSet::GetSTRAttribute());
	
	// Get Skill Value
	float SkillValue = GetSkillValue(Asc, SkillTag);

	// Attack = 8 + Combat Skill + STR
	return 8.0f + SkillValue + STR;
}

float USignetCombatStatics::CalculateAccuracy(const USignetAbilitySystemComponent* Asc, FGameplayTag SkillTag)
{
	if (!Asc) return 0.0f;

	float DEX = Asc->GetNumericAttribute(USignetPrimaryAttributeSet::GetDEXAttribute());
	
	// Accuracy from DEX (FFXI formula: floor(DEX * 0.75))
	float AccFromDEX = FMath::FloorToFloat(DEX * 0.75f);

	// Get Skill Value
	float SkillValue = GetSkillValue(Asc, SkillTag);

	// Accuracy from Skill (FFXI piecewise formula)
	float AccFromSkill = 0.0f;
	if (SkillValue <= 200.0f)
	{
		AccFromSkill = SkillValue;
	}
	else if (SkillValue <= 400.0f)
	{
		AccFromSkill = FMath::FloorToFloat((SkillValue - 200.0f) * 0.9f) + 200.0f;
	}
	else if (SkillValue <= 600.0f)
	{
		AccFromSkill = FMath::FloorToFloat((SkillValue - 400.0f) * 0.8f) + 380.0f;
	}
	else
	{
		AccFromSkill = FMath::FloorToFloat((SkillValue - 600.0f) * 0.9f) + 540.0f;
	}

	// Accuracy from Traits/Gear/etc. (The Accuracy attribute itself)
	float AccFromGear = Asc->GetNumericAttribute(USignetCombatAttributeSet::GetAccuracyAttribute());

	// AccuracyTotal = Accuracy from DEX + Accuracy from Skill + Accuracy from Traits/Gear/etc.
	return AccFromDEX + AccFromSkill + AccFromGear;
}

float USignetCombatStatics::GetSkillValue(const USignetAbilitySystemComponent* Asc, FGameplayTag SkillTag)
{
	if (!Asc || !SkillTag.IsValid()) return 0.0f;

	// This assumes we have a mapping from Tag to Attribute, or we can fetch it via attribute set
	// For now, let's try to find the attribute based on the tag name
	FString SkillName = SkillTag.GetTagName().ToString();
	SkillName.RemoveFromStart(TEXT("Data.Skill."));
	
	// In a real system, we might have a map, but here we can try to find it by name if GAS Companion supports it
	// or use a switch on common tags
	const USignetSkillAttributeSet* SkillSet = Asc->GetSet<USignetSkillAttributeSet>();
	if (SkillSet)
	{
		FGameplayAttribute SkillAttr;
		// Manual mapping for common skills to ensure correctness
		const FSkillTags& SkillTags = FTagCache::Get().Skill;
		if (SkillTag == SkillTags.Sword) SkillAttr = USignetSkillAttributeSet::GetSwordAttribute();
		else if (SkillTag == SkillTags.GreatSword) SkillAttr = USignetSkillAttributeSet::GetGreatSwordAttribute();
		else if (SkillTag == SkillTags.Axe) SkillAttr = USignetSkillAttributeSet::GetAxeAttribute();
		else if (SkillTag == SkillTags.GreatAxe) SkillAttr = USignetSkillAttributeSet::GetGreatAxeAttribute();
		else if (SkillTag == SkillTags.Dagger) SkillAttr = USignetSkillAttributeSet::GetDaggerAttribute();
		else if (SkillTag == SkillTags.Club) SkillAttr = USignetSkillAttributeSet::GetClubAttribute();
		else if (SkillTag == SkillTags.Staff) SkillAttr = USignetSkillAttributeSet::GetStaffAttribute();
		else if (SkillTag == SkillTags.Polearm) SkillAttr = USignetSkillAttributeSet::GetPolearmAttribute();
		else if (SkillTag == SkillTags.Scythe) SkillAttr = USignetSkillAttributeSet::GetScytheAttribute();
		else if (SkillTag == SkillTags.Katana) SkillAttr = USignetSkillAttributeSet::GetKatanaAttribute();
		else if (SkillTag == SkillTags.GreatKatana) SkillAttr = USignetSkillAttributeSet::GetGreatKatanaAttribute();
		else if (SkillTag == SkillTags.H2H) SkillAttr = USignetSkillAttributeSet::GetHandToHandAttribute();
		else if (SkillTag == SkillTags.Archery) SkillAttr = USignetSkillAttributeSet::GetArcheryAttribute();
		else if (SkillTag == SkillTags.Marksmanship) SkillAttr = USignetSkillAttributeSet::GetMarksmanshipAttribute();
		else if (SkillTag == SkillTags.Throwing) SkillAttr = USignetSkillAttributeSet::GetThrowingAttribute();

		if (SkillAttr.IsValid())
		{
			return Asc->GetNumericAttribute(SkillAttr);
		}
	}

	return 0.0f;
}

bool USignetCombatStatics::CheckShieldBlock(const USignetAbilitySystemComponent* AttackerAsc, const USignetAbilitySystemComponent* TargetAsc)
{
	if (!TargetAsc) return false;
	float BlockRate = TargetAsc->GetNumericAttribute(USignetCombatAttributeSet::GetShieldBlockRateAttribute());
	return (FMath::FRand() * 100.0f) < BlockRate;
}

bool USignetCombatStatics::CheckGuarded(const USignetAbilitySystemComponent* AttackerAsc, const USignetAbilitySystemComponent* TargetAsc)
{
	if (!TargetAsc) return false;
	// Guard logic placeholder
	return false;
}

int32 USignetCombatStatics::ApplyDamageMultipliers(const USignetAbilitySystemComponent* AttackerAsc, int32 CurrentDamage, EPhysicalAttackType AttackType, EGearSlot WeaponSlot, bool bIsFirstSwing)
{
	float Damage = (float)CurrentDamage;

	if (AttackType == EPhysicalAttackType::Double)
	{
		float DoubleAttackDmg = AttackerAsc->GetNumericAttribute(USignetCombatAttributeSet::GetDoubleAttackDamageAttribute());
		Damage *= (1.0f + DoubleAttackDmg / 100.0f);
	}
	
	// Soul Eater logic (EFFECT_SOUL_EATER)
	static const FGameplayTag SoulEaterTag = FGameplayTag::RequestGameplayTag(TEXT("Data.Effect.SoulEater"));
	if (AttackerAsc->HasMatchingGameplayTag(SoulEaterTag))
	{
		float MaxHP = AttackerAsc->GetNumericAttribute(USignetPrimaryAttributeSet::GetMaxHPAttribute());
		Damage += MaxHP * 0.1f; // 10% of Max HP added to damage
	}

	return FMath::FloorToInt(Damage);
}
