#pragma once
#include "GameplayTagContainer.h"

struct FStateTags
{
	FGameplayTag Root;
	FGameplayTag Idle;
	FGameplayTag Engaged;
	FGameplayTag Dead;
	FGameplayTag Casting;
	FGameplayTag Sitting;
	FGameplayTag Resting;
	FGameplayTag Crafting;
	FGameplayTag Mounted;
	FGameplayTag Cutscene;
	FGameplayTag Stunned;

	TArray<FGameplayTag> AllTags;

	void Initialize()
	{
		Root = FGameplayTag::RequestGameplayTag(TEXT("State"));
		Idle = FGameplayTag::RequestGameplayTag(TEXT("State.Idle"));
		Engaged = FGameplayTag::RequestGameplayTag(TEXT("State.Engaged"));
		Dead = FGameplayTag::RequestGameplayTag(TEXT("State.Dead"));
		Casting = FGameplayTag::RequestGameplayTag(TEXT("State.Casting"));
		Sitting = FGameplayTag::RequestGameplayTag(TEXT("State.Sitting"));
		Resting = FGameplayTag::RequestGameplayTag(TEXT("State.Resting"));
		Crafting = FGameplayTag::RequestGameplayTag(TEXT("State.Crafting"));
		Mounted = FGameplayTag::RequestGameplayTag(TEXT("State.Mounted"));
		Cutscene = FGameplayTag::RequestGameplayTag(TEXT("State.Cutscene"));
		Stunned = FGameplayTag::RequestGameplayTag(TEXT("State.Stunned"));

		AllTags = {
			Idle,
			Engaged,
			Dead,
			Casting,
			Sitting,
			Resting,
			Crafting,
			Mounted,
			Cutscene,
			Stunned
		};
	}
};
