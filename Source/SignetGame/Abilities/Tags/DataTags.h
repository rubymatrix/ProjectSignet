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

	void Initialize()
	{
		Job = FGameplayTag::RequestGameplayTag(TEXT("Data.Job"));
		SubJob = FGameplayTag::RequestGameplayTag(TEXT("Data.SubJob"));
		JobLevel = FGameplayTag::RequestGameplayTag(TEXT("Data.JobLevel"));
		SubJobLevel = FGameplayTag::RequestGameplayTag(TEXT("Data.SubJobLevel"));
		Race = FGameplayTag::RequestGameplayTag(TEXT("Data.Race"));
		EntityType = FGameplayTag::RequestGameplayTag(TEXT("Data.EntityType"));
		PlayerHpMultiplier = FGameplayTag::RequestGameplayTag(TEXT("Data.PlayerHpMultiplier"));
		PlayerMpMultiplier = FGameplayTag::RequestGameplayTag(TEXT("Data.PlayerMpMultiplier"));
		PlayerStatMultiplier = FGameplayTag::RequestGameplayTag(TEXT("Data.PlayerStatMultiplier"));
		SubJobDivisor = FGameplayTag::RequestGameplayTag(TEXT("Data.SubJobDivisor"));
		HealAfterInit = FGameplayTag::RequestGameplayTag(TEXT("Data.HealAfterInit"));
	}
};