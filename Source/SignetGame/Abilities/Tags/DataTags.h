#pragma once
#include "GameplayTagContainer.h"

struct FDataTags
{
	FGameplayTag Job;
	FGameplayTag SubJob;
	FGameplayTag JobLevel;
	FGameplayTag SubJobLevel;
	FGameplayTag Race;
	FGameplayTag EntityType;
	FGameplayTag PlayerHpMultiplier;
	FGameplayTag PlayerMpMultiplier;
	FGameplayTag PlayerStatMultiplier;
	FGameplayTag SubJobDivisor;
	FGameplayTag HealAfterInit;
	FGameplayTag MoveSpeedMultiplier;
	FGameplayTag HP;
	FGameplayTag MaxHP;
	FGameplayTag MP;
	FGameplayTag MaxMP;
	FGameplayTag STR;
	FGameplayTag DEX;
	FGameplayTag VIT;
	FGameplayTag AGI;
	FGameplayTag INT;
	FGameplayTag MND;
	FGameplayTag CHR;

	void Initialize()
	{
		Job = FGameplayTag::RequestGameplayTag(TEXT("Data.SBC.Job"));
		SubJob = FGameplayTag::RequestGameplayTag(TEXT("Data.SBC.SubJob"));
		JobLevel = FGameplayTag::RequestGameplayTag(TEXT("Data.SBC.JobLevel"));
		SubJobLevel = FGameplayTag::RequestGameplayTag(TEXT("Data.SBC.SubJobLevel"));
		Race = FGameplayTag::RequestGameplayTag(TEXT("Data.SBC.Race"));
		EntityType = FGameplayTag::RequestGameplayTag(TEXT("Data.EntityType"));
		PlayerHpMultiplier = FGameplayTag::RequestGameplayTag(TEXT("Data.PlayerHpMultiplier"));
		PlayerMpMultiplier = FGameplayTag::RequestGameplayTag(TEXT("Data.PlayerMpMultiplier"));
		PlayerStatMultiplier = FGameplayTag::RequestGameplayTag(TEXT("Data.PlayerStatMultiplier"));
		SubJobDivisor = FGameplayTag::RequestGameplayTag(TEXT("Data.SubJobDivisor"));
		HealAfterInit = FGameplayTag::RequestGameplayTag(TEXT("Data.HealAfterInit"));
		HP = FGameplayTag::RequestGameplayTag(TEXT("Data.SBC.HP"));
		MaxHP = FGameplayTag::RequestGameplayTag(TEXT("Data.SBC.MaxHP"));
		MaxHP = FGameplayTag::RequestGameplayTag(TEXT("Data.SBC.MaxHP"));
		MaxMP = FGameplayTag::RequestGameplayTag(TEXT("Data.SBC.MaxMP"));
		STR = FGameplayTag::RequestGameplayTag(TEXT("Data.SBC.STR"));
		DEX = FGameplayTag::RequestGameplayTag(TEXT("Data.SBC.DEX"));
		VIT = FGameplayTag::RequestGameplayTag(TEXT("Data.SBC.VIT"));
		AGI = FGameplayTag::RequestGameplayTag(TEXT("Data.SBC.AGI"));
		INT = FGameplayTag::RequestGameplayTag(TEXT("Data.SBC.INT"));
		MND = FGameplayTag::RequestGameplayTag(TEXT("Data.SBC.MND"));
		CHR = FGameplayTag::RequestGameplayTag(TEXT("Data.SBC.CHR"));
	}
};