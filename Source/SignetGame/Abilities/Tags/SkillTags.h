#pragma once
#include "GameplayTagContainer.h"

struct FSkillTags
{
	FGameplayTag None;
	FGameplayTag Archery;
	FGameplayTag Axe;
	FGameplayTag Club;
	FGameplayTag Dagger;
	FGameplayTag GreatAxe;
	FGameplayTag GreatKatana;
	FGameplayTag GreatSword;
	FGameplayTag H2H;
	FGameplayTag Katana;
	FGameplayTag Marksmanship;
	FGameplayTag Polearm;
	FGameplayTag Scythe;
	FGameplayTag Staff;
	FGameplayTag Shield;
	FGameplayTag StringInstrument;
	FGameplayTag Sword;
	FGameplayTag Throwing;
	FGameplayTag WindInstrument;
	FGameplayTag Singing;
	FGameplayTag DWCategory1;
	FGameplayTag DWCategory2;
	FGameplayTag DWCategory3;
	FGameplayTag DWCategory4;
	FGameplayTag DWSubCategory1;
	FGameplayTag DWSubCategory2;
	FGameplayTag DWSubCategory3;
	FGameplayTag DWSubCategory4;
	
	void Initialize()
	{
		None = FGameplayTag::RequestGameplayTag(TEXT("Data.Skill.None"));
		Archery = FGameplayTag::RequestGameplayTag(TEXT("Data.Skill.Archery"));
		Axe = FGameplayTag::RequestGameplayTag(TEXT("Data.Skill.Axe"));
		Club = FGameplayTag::RequestGameplayTag(TEXT("Data.Skill.Club"));
		Dagger = FGameplayTag::RequestGameplayTag(TEXT("Data.Skill.Dagger"));
		GreatAxe = FGameplayTag::RequestGameplayTag(TEXT("Data.Skill.GreatAxe"));
		GreatKatana = FGameplayTag::RequestGameplayTag(TEXT("Data.Skill.GreatKatana"));
		GreatSword = FGameplayTag::RequestGameplayTag(TEXT("Data.Skill.GreatSword"));
		H2H = FGameplayTag::RequestGameplayTag(TEXT("Data.Skill.HandToHand"));
		Katana = FGameplayTag::RequestGameplayTag(TEXT("Data.Skill.Katana"));
		Marksmanship = FGameplayTag::RequestGameplayTag(TEXT("Data.Skill.Marksmanship"));
		Polearm = FGameplayTag::RequestGameplayTag(TEXT("Data.Skill.Polearm"));
		Scythe = FGameplayTag::RequestGameplayTag(TEXT("Data.Skill.Scythe"));
		Staff = FGameplayTag::RequestGameplayTag(TEXT("Data.Skill.Staff"));
		Shield = FGameplayTag::RequestGameplayTag(TEXT("Data.Skill.Shield"));
		Singing = FGameplayTag::RequestGameplayTag(TEXT("Data.Skill.Singing"));
		StringInstrument = FGameplayTag::RequestGameplayTag(TEXT("Data.Skill.StringInstrument"));
		Sword = FGameplayTag::RequestGameplayTag(TEXT("Data.Skill.Sword"));
		Throwing = FGameplayTag::RequestGameplayTag(TEXT("Data.Skill.Throwing"));
		WindInstrument = FGameplayTag::RequestGameplayTag(TEXT("Data.Skill.WindInstrument"));
		DWCategory1 = FGameplayTag::RequestGameplayTag(TEXT("Data.Skill.DualWield.Category1"));
		DWCategory2 = FGameplayTag::RequestGameplayTag(TEXT("Data.Skill.DualWield.Category2"));
		DWCategory3 = FGameplayTag::RequestGameplayTag(TEXT("Data.Skill.DualWield.Category3"));
		DWCategory4 = FGameplayTag::RequestGameplayTag(TEXT("Data.Skill.DualWield.Category4"));
		DWSubCategory1 = FGameplayTag::RequestGameplayTag(TEXT("Data.Skill.DualWield.SubCategory1"));
		DWSubCategory2 = FGameplayTag::RequestGameplayTag(TEXT("Data.Skill.DualWield.SubCategory2"));
		DWSubCategory3 = FGameplayTag::RequestGameplayTag(TEXT("Data.Skill.DualWield.SubCategory3"));
		DWSubCategory4 = FGameplayTag::RequestGameplayTag(TEXT("Data.Skill.DualWield.SubCategory4"));
		
	}
};