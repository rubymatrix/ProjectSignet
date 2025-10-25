#pragma once
#include "GameplayTagContainer.h"

struct FRaceTags
{
	FGameplayTag HumeMale;
	FGameplayTag HumeFemale;
	FGameplayTag ElvaanMale;
	FGameplayTag ElvaanFemale;
	FGameplayTag TarutaruMale;
	FGameplayTag TarutaruFemale;
	FGameplayTag Mithra;
	FGameplayTag Galka;

	void Initialize()
	{
		HumeMale = FGameplayTag::RequestGameplayTag(TEXT("Race.HumeMale"));
		HumeFemale = FGameplayTag::RequestGameplayTag(TEXT("Race.HumeFemale"));
		ElvaanMale = FGameplayTag::RequestGameplayTag(TEXT("Race.ElvaanMale"));
		ElvaanFemale = FGameplayTag::RequestGameplayTag(TEXT("Race.ElvaanFemale"));
		TarutaruMale = FGameplayTag::RequestGameplayTag(TEXT("Race.TarutaruMale"));
		TarutaruFemale = FGameplayTag::RequestGameplayTag(TEXT("Race.TarutaruFemale"));
		Mithra = FGameplayTag::RequestGameplayTag(TEXT("Race.Mithra"));
		Galka = FGameplayTag::RequestGameplayTag(TEXT("Race.Galka"));
	}
};