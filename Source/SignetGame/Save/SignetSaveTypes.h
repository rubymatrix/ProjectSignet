// Copyright Red Lotus Games, All Rights Reserved.

#pragma once
#include "CoreMinimal.h"
#include "SignetSaveTypes.generated.h"


UENUM(BlueprintType)
enum class ENation : uint8
{
	Bastok		UMETA(DisplayName = "Republic of Bastok"),
	Sandoria	UMETA(DisplayName = "Kingdom of San d'Oria"),
	Windurst	UMETA(DisplayName = "Federation of Windurst"),
	Jeuno		UMETA(DisplayName = "Grand Duchy of Jeuno"),
	Tavnazia    UMETA(DisplayName = "Grand Duchy of Tavnazia"),
	Whitegate   UMETA(DisplayName = "Empire of Aht Urghan"),
	Adoulin     UMETA(DisplayName = "Kingdom of Adoulin")
};

UENUM(BlueprintType)
enum class EJob : uint8
{
	Warrior, Monk, Thief, RedMage, BlackMage, WhiteMage
};

UENUM(BlueprintType)
enum class ERace : uint8
{
	HumeMale, HumeFemale, ElvaanMale, ElvaanFemale, TarutaruMale, TarutaruFemale, Mithra, Galka	
};

UENUM(BlueprintType)
enum class EFace : uint8
{
	F1A, F1B,
	F2A, F2B,
	F3A, F3B,
	F4A, F4B,
	F5A, F5B,
	F6A, F6B,
	F7A, F7B,
	F8A, F8B
};


UENUM(BlueprintType)
enum class ECurrencyType : uint8
{
	EXP				UMETA(DisplayName = "Experience"),
	AP				UMETA(DisplayName = "Ability Points"),
	SP				UMETA(DisplayName = "Skill Points"),
	BeastmenSeals	UMETA(DisplayName = "Beastmen Seals"),
	KindredSeals	UMETA(DisplayName = "Kindred Seals"),
	Gil				UMETA(DisplayName = "Gil")
};

USTRUCT(BlueprintType)
struct SIGNETGAME_API FSignetCurrencyWallet
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite) 
	int64 Experience = 0;
	 
	UPROPERTY(EditAnywhere, BlueprintReadWrite) 
	int64 AbilityPoints = 0;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite) 
	int64 SkillPoints   = 0;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite) 
	int64 BeastmenSeals = 0;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite) 
	int64 Gil           = 0;
	

	int64& GetRef(ECurrencyType Type)
	{
		switch (Type)
		{
		case ECurrencyType::EXP:            return Experience;
		case ECurrencyType::AP:             return AbilityPoints;
		case ECurrencyType::SP:             return SkillPoints;
		case ECurrencyType::BeastmenSeals:  return BeastmenSeals;
		default:                            return Gil;
		}
	}
};

USTRUCT(BlueprintType)
struct SIGNETGAME_API FSkillNodeProgress
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite) 
	FName NodeId;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite) 
	int CurrentTier = 0; 
	
};

USTRUCT(BlueprintType)
struct SIGNETGAME_API FSignetInventoryItem
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite) 
	FName ItemId;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite) 
	int Quantity = 1;
};

USTRUCT(BlueprintType)
struct SIGNETGAME_API FSignetJobProgress
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite) 
	EJob JobType = EJob::Warrior;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite) 
	int Level = 1;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite) 
	int64 CurrentExp = 0;
};

USTRUCT(BlueprintType)
struct SIGNETGAME_API FSignetPlayerProfile
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite) 
	FString DisplayName = TEXT("Soldier");

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ENation HomeNation = ENation::Bastok;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite) 
	EJob CurrentJob = EJob::Warrior;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite) 
	ERace SelectedRace = ERace::HumeMale;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite) 
	EFace SelectedFace = EFace::F1A;
};