// Copyright 2025, Red Lotus Games

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AttributeMacros.h"
#include "AbilitySystemComponent.h"
#include "Abilities/Attributes/GSCAttributeSetBase.h"
#include "SignetSkillAttributeSet.generated.h"

UCLASS()
class SIGNETGAME_API USignetSkillAttributeSet : public UGSCAttributeSetBase
{
	GENERATED_BODY()

public:

	USignetSkillAttributeSet();

	/*
	* Skill Types
	* Axe
	* Archery
	* Club
	* Dagger
	* Evasion
	* GreatAxe
	* GreatSword
	* GreatKatana
	* Guard
	* HandToHand
	* Katana
	* Marksmanship
	* Polearm
	* Parrying
	* Scythe
	* Shield
	* Staff
	* Sword
	* Throwing
	*/
	
	DRAGON_ATTRIBUTE(USignetSkillAttributeSet, Axe, "Stats|Skills")
	DRAGON_ATTRIBUTE(USignetSkillAttributeSet, Archery, "Stats|Skills")
	DRAGON_ATTRIBUTE(USignetSkillAttributeSet, Club, "Stats|Skills")
	DRAGON_ATTRIBUTE(USignetSkillAttributeSet, Dagger, "Stats|Skills")
	DRAGON_ATTRIBUTE(USignetSkillAttributeSet, Evasion, "Stats|Skills")
	DRAGON_ATTRIBUTE(USignetSkillAttributeSet, GreatAxe, "Stats|Skills")
	DRAGON_ATTRIBUTE(USignetSkillAttributeSet, GreatSword, "Stats|Skills")
	DRAGON_ATTRIBUTE(USignetSkillAttributeSet, GreatKatana, "Stats|Skills")
	DRAGON_ATTRIBUTE(USignetSkillAttributeSet, Guard, "Stats|Skills")
	DRAGON_ATTRIBUTE(USignetSkillAttributeSet, HandToHand, "Stats|Skills")
	DRAGON_ATTRIBUTE(USignetSkillAttributeSet, Katana, "Stats|Skills")
	DRAGON_ATTRIBUTE(USignetSkillAttributeSet, Marksmanship, "Stats|Skills")
	DRAGON_ATTRIBUTE(USignetSkillAttributeSet, Polearm, "Stats|Skills")
	DRAGON_ATTRIBUTE(USignetSkillAttributeSet, Parrying, "Stats|Skills")
	DRAGON_ATTRIBUTE(USignetSkillAttributeSet, Scythe, "Stats|Skills")
	DRAGON_ATTRIBUTE(USignetSkillAttributeSet, Shield, "Stats|Skills")
	DRAGON_ATTRIBUTE(USignetSkillAttributeSet, Staff, "Stats|Skills")
	DRAGON_ATTRIBUTE(USignetSkillAttributeSet, Sword, "Stats|Skills")
	DRAGON_ATTRIBUTE(USignetSkillAttributeSet, Throwing, "Stats|Skills")

	/*
	* Magic Skills
	* Blue
	* Dark
	* Divine
	* Elemental
	* Enfeebling
	* Enhancing
	* Geomancy
	* Handbell
	* Healing
	* Summoning
	* Ninjutsu
	* Singing
	* StringInstrument
	* WindInstrument
	*/

	DRAGON_ATTRIBUTE(USignetSkillAttributeSet, Blue, "Stats|Skills|Magic")
	DRAGON_ATTRIBUTE(USignetSkillAttributeSet, Dark, "Stats|Skills|Magic")
	DRAGON_ATTRIBUTE(USignetSkillAttributeSet, Divine, "Stats|Skills|Magic")
	DRAGON_ATTRIBUTE(USignetSkillAttributeSet, Elemental, "Stats|Skills|Magic")
	DRAGON_ATTRIBUTE(USignetSkillAttributeSet, Enfeebling, "Stats|Skills|Magic")
	DRAGON_ATTRIBUTE(USignetSkillAttributeSet, Enhancing, "Stats|Skills|Magic")
	DRAGON_ATTRIBUTE(USignetSkillAttributeSet, Geomancy, "Stats|Skills|Magic")
	DRAGON_ATTRIBUTE(USignetSkillAttributeSet, Handbell, "Stats|Skills|Magic")
	DRAGON_ATTRIBUTE(USignetSkillAttributeSet, Healing, "Stats|Skills|Magic")
	DRAGON_ATTRIBUTE(USignetSkillAttributeSet, Summoning, "Stats|Skills|Magic")
	DRAGON_ATTRIBUTE(USignetSkillAttributeSet, Ninjutsu, "Stats|Skills|Magic")
	DRAGON_ATTRIBUTE(USignetSkillAttributeSet, Singing, "Stats|Skills|Magic")
	DRAGON_ATTRIBUTE(USignetSkillAttributeSet, StringInstrument, "Stats|Skills|Magic")
	DRAGON_ATTRIBUTE(USignetSkillAttributeSet, WindInstrument, "Stats|Skills|Magic")

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};
