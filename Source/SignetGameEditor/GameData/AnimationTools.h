// Copyright Red Lotus Games, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "SignetGame/Abilities/TagCache.h"
#include "AnimationTools.generated.h"

class USignetPlayerAnimInstance;


struct FCharacterAnimData
{
	FString UnarmedKey;
	FString H2HKey;
	FString ClubKey;
	FString AxeKey;
	FString SwordKey;
	FString DaggerKey;
	FString KatanaKey;
	FString StaffKey;
	FString GreatSwordKey;
	FString GreatAxeKey;
	FString ScytheKey;
	FString PolearmKey;
	FString GreatKatanaKey;
	FString DWCategory1Key;
	FString DWCategory2Key;
	FString DWCategory3Key;
	FString DWCategory4Key;
	FString DWSubCategory1Key;
	FString DWSubCategory2Key;
	FString DWSubCategory3Key;
	FString DWSubCategory4Key;

	FString GetKey(const FGameplayTag& WeaponTag)
	{
		TMap<FGameplayTag, FString> KeyMap = {
			{ FTagCache::Get().Skill.None, UnarmedKey },
			{ FTagCache::Get().Skill.H2H, H2HKey },
			{ FTagCache::Get().Skill.Club, ClubKey },
			{ FTagCache::Get().Skill.Axe, AxeKey },
			{ FTagCache::Get().Skill.Sword, SwordKey },
			{ FTagCache::Get().Skill.Dagger, DaggerKey },
			{ FTagCache::Get().Skill.Katana, KatanaKey },
			{ FTagCache::Get().Skill.Staff, StaffKey },
			{ FTagCache::Get().Skill.GreatSword, GreatSwordKey },
			{ FTagCache::Get().Skill.GreatAxe, GreatAxeKey },
			{ FTagCache::Get().Skill.Scythe, ScytheKey },
			{ FTagCache::Get().Skill.Polearm, PolearmKey },
			{ FTagCache::Get().Skill.GreatKatana, GreatKatanaKey },
			{ FTagCache::Get().Skill.DWCategory1, DWCategory1Key },
			{ FTagCache::Get().Skill.DWCategory2, DWCategory2Key },
			{ FTagCache::Get().Skill.DWCategory3, DWCategory3Key },
			{ FTagCache::Get().Skill.DWCategory4, DWCategory4Key },
			{ FTagCache::Get().Skill.DWSubCategory1, DWSubCategory1Key },
			{ FTagCache::Get().Skill.DWSubCategory2, DWSubCategory2Key },
			{ FTagCache::Get().Skill.DWSubCategory3, DWSubCategory3Key },
			{ FTagCache::Get().Skill.DWSubCategory4, DWSubCategory4Key }
		};

		if (KeyMap.Find(WeaponTag))
		{
			return *KeyMap.Find(WeaponTag);
		}
		
		return TEXT("");
	}
};

UCLASS()
class SIGNETGAMEEDITOR_API UAnimationTools : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, CallInEditor)
	static void AssignAnimationsByRace(UObject* Object, const FGameplayTag& RaceTag);

	UFUNCTION(BlueprintCallable, CallInEditor)
	static void AssignBasicAnimations(USignetPlayerAnimInstance* AnimInstance, const FGameplayTag& RaceTag);

	UFUNCTION(BlueprintCallable, CallInEditor)
	static void AssignCastingScheduleAnimations(USignetPlayerAnimInstance* AnimInstance, const FGameplayTag& RaceTag);
	
	UFUNCTION(BlueprintCallable, CallInEditor)
	static void AssignAnimationsByWeaponType(USignetPlayerAnimInstance* AnimInstance, const FGameplayTag& RaceTag, const FGameplayTag& WeaponType);


private:

	static FString GetRacePathKey(const FGameplayTag& RaceTag);
	static FCharacterAnimData GetAnimDataForRace(const FGameplayTag& RaceTag);
};
