// Copyright Red Lotus Games, All Rights Reserved.

#include "GE_PhysicalDamage.h"
#include "Abilities/Attributes/GSCAttributeSet.h"

UGE_PhysicalDamage::UGE_PhysicalDamage()
{
	DurationPolicy = EGameplayEffectDurationType::Instant;

	FGameplayModifierInfo Modifier;
	Modifier.Attribute = UGSCAttributeSet::GetDamageAttribute();
	Modifier.ModifierOp = EGameplayModOp::Additive;
	
	FSetByCallerFloat SetByCaller;
	SetByCaller.DataTag = FGameplayTag::RequestGameplayTag(TEXT("Data.Damage"));
	
	Modifier.ModifierMagnitude = FGameplayEffectModifierMagnitude(SetByCaller);
	
	Modifiers.Add(Modifier);
}
