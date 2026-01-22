// Copyright Red Lotus Games, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "SignetGame/Save/SignetSaveTypes.h"
#include "SkillRankStatics.generated.h"

UENUM(BlueprintType)
enum class ESkillRank : uint8
{
	None = 0 UMETA(DisplayName = "None"),
	APlus = 1 UMETA(DisplayName = "A+"),
	A = 2 UMETA(DisplayName = "A"),
	BPlus = 3 UMETA(DisplayName = "B+"),
	B = 4 UMETA(DisplayName = "B"),
	BMinus = 5 UMETA(DisplayName = "B-"),
	CPlus = 6 UMETA(DisplayName = "C+"),
	C = 7 UMETA(DisplayName = "C"),
	CMinus = 8 UMETA(DisplayName = "C-"),
	D = 9 UMETA(DisplayName = "D"),
	E = 10 UMETA(DisplayName = "E"),
	F = 11 UMETA(DisplayName = "F"),
	G = 12 UMETA(DisplayName = "G"),
};

UCLASS()
class SIGNETGAME_API USkillRankStatics : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	/** Returns the skill rank for a job/skill pair. */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Skills")
	static ESkillRank GetSkillRankForJob(EJob Job, FGameplayTag SkillTag);

	/** Returns the skill cap for a level/rank pair (mirrors skill_ranks.lua). */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Skills")
	static int32 GetSkillCapForLevelAndRank(int32 Level, ESkillRank Rank);

	/** Returns the skill cap for a job/skill at a given level. */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Skills")
	static int32 GetSkillCapForJobAndSkill(int32 Level, EJob Job, FGameplayTag SkillTag);

	/** Attempts to skill up a player's weapon skill based on victim level. */
	UFUNCTION(BlueprintCallable, Category = "Skills")
	static void TrySkillUp(ASignetPlayerCharacter* Player, FGameplayTag SkillTag, int32 VictimLevel, bool bForceSkillUp = false);

private:
	static const TArray<TArray<int32>> SkillCapsByLevel;
};
