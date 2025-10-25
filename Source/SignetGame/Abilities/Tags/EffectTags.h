#pragma once
#include "GameplayTagContainer.h"

struct FEffectTags
{
	FGameplayTag KO;
	FGameplayTag Bound;
	FGameplayTag Stunned;
	FGameplayTag Sleep;
	FGameplayTag Gravity;
	
	void Initialize()
	{
		KO = FGameplayTag::RequestGameplayTag(TEXT("Data.Effect.KO"));
		Bound = FGameplayTag::RequestGameplayTag(TEXT("Data.Effect.Bound"));
		Stunned = FGameplayTag::RequestGameplayTag(TEXT("Data.Effect.Stunned"));
		Sleep = FGameplayTag::RequestGameplayTag(TEXT("Data.Effect.Sleep"));
		Gravity = FGameplayTag::RequestGameplayTag(TEXT("Data.Effect.Gravity"));
	}
};