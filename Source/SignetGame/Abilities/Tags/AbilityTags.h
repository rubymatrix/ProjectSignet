#pragma once
#include "GameplayTagContainer.h"

struct FAbilityTags
{
	FGameplayTag Engage;
	FGameplayTag Disengage;
	FGameplayTag Cast;
	FGameplayTag Craft;
	FGameplayTag Rest;
	FGameplayTag StopRest;
	FGameplayTag Sit;
	FGameplayTag StopSit;
	FGameplayTag Jump;
	FGameplayTag Die;
	FGameplayTag Raise;
	FGameplayTag Attack;
	
	void Initialize()
	{
		Engage = FGameplayTag::RequestGameplayTag(TEXT("Ability.Engage"));
		Disengage = FGameplayTag::RequestGameplayTag(TEXT("Ability.Disengage"));
		Cast = FGameplayTag::RequestGameplayTag(TEXT("Ability.Cast"));
		Craft = FGameplayTag::RequestGameplayTag(TEXT("Ability.Craft"));
		Rest = FGameplayTag::RequestGameplayTag(TEXT("Ability.Rest"));
		StopRest = FGameplayTag::RequestGameplayTag(TEXT("Ability.StopRest"));
		Sit = FGameplayTag::RequestGameplayTag(TEXT("Ability.Sit"));
		StopSit = FGameplayTag::RequestGameplayTag(TEXT("Ability.StopSit"));
		Jump = FGameplayTag::RequestGameplayTag(TEXT("Ability.Jump"));
		Die = FGameplayTag::RequestGameplayTag(TEXT("Ability.Die"));
		Raise = FGameplayTag::RequestGameplayTag(TEXT("Ability.Raise"));
		Attack = FGameplayTag::RequestGameplayTag(TEXT("Ability.Attack"));
	}
};