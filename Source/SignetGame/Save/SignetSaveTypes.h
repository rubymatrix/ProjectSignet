// Copyright Red Lotus Games, All Rights Reserved.

#pragma once
#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
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
	None = 0 UMETA(DisplayName = "None"),
	Warrior = 1 UMETA(DisplayName = "Warrior"),
	Monk = 2 UMETA(DisplayName = "Monk"),
	WhiteMage = 3 UMETA(DisplayName = "White Mage"),
	BlackMage = 4 UMETA(DisplayName = "Black Mage"),
	RedMage = 5  UMETA(DisplayName = "Red Mage"),
	Thief = 6  UMETA(DisplayName = "Thief"),
	Paladin = 7  UMETA(DisplayName = "Paladin"),
	DarkKnight = 8  UMETA(DisplayName = "Dark Knight"),
	Beastmaster = 9  UMETA(DisplayName = "Beastmaster"),
	Bard = 10  UMETA(DisplayName = "Bard"),
	Ranger = 11  UMETA(DisplayName = "Ranger"),
	Samurai = 12  UMETA(DisplayName = "Samurai"),
	Ninja = 13  UMETA(DisplayName = "Ninja"),
	Dragoon = 14  UMETA(DisplayName = "Dragoon"),
	Summoner = 15  UMETA(DisplayName = "Summoner"),
	BlueMage = 16 UMETA(DisplayName = "Blue Mage"),
	Corsair = 17  UMETA(DisplayName = "Corsair"),
	Puppetmaster = 18  UMETA(DisplayName = "Puppetmaster"),
	Dancer = 19  UMETA(DisplayName = "Dancer"),
	Scholar = 20  UMETA(DisplayName = "Scholar"),
	Geomancer = 21 UMETA(DisplayName = "Geomancer"),
	Runefencer = 22  UMETA(DisplayName = "Rune Fencer"),
	Monstrosity = 23  UMETA(DisplayName = "Monstrosity"),
};

static TMap<EJob, FString> JobAbbrs = {
	{ EJob::None, "NON" },
	{ EJob::Warrior, "WAR"},
	{ EJob::Monk, "MNK" },
	{ EJob::WhiteMage, "WHM" },
	{ EJob::BlackMage, "BLM" },
	{ EJob::RedMage, "RDM" },
	{ EJob::Thief, "THF" },
	{ EJob::Paladin, "PLD" },
	{ EJob::DarkKnight, "DRK" },
	{ EJob::Beastmaster, "BST" },
	{ EJob::Bard, "BRD" },
	{ EJob::Ranger, "RNG" },
	{ EJob::Samurai, "SAM" },
	{ EJob::Ninja, "NIN" },
	{ EJob::Dragoon, "DRG" },
	{ EJob::Summoner, "SMN" },
	{ EJob::BlueMage, "BLU" },
	{ EJob::Corsair, "COR" },
	{ EJob::Puppetmaster, "PUP" },
	{ EJob::Dancer, "DNC" },
	{ EJob::Scholar, "SCH" },
	{ EJob::Geomancer, "GEO" },
	{ EJob::Runefencer, "RUN" },
	{ EJob::Monstrosity, "MON" }
};
static FString GetJobAbbreviation(const EJob Job)
{
	if (const auto* Abbr = JobAbbrs.Find(Job))
	{
		return *Abbr;
	}
	return TEXT("NON");
}

static TMap<EJob, FString> JobNames = {
	{ EJob::None, "None" },
	{ EJob::Warrior, "Warrior"},
	{ EJob::Monk, "Monk" },
	{ EJob::WhiteMage, "White Mage" },
	{ EJob::BlackMage, "Black Mage" },
	{ EJob::RedMage, "Red Mage" },
	{ EJob::Thief, "Thief" },
	{ EJob::Paladin, "Paladin" },
	{ EJob::DarkKnight, "Dark Knight" },
	{ EJob::Beastmaster, "Beastmaster" },
	{ EJob::Bard, "Bard" },
	{ EJob::Ranger, "Ranger" },
	{ EJob::Samurai, "Samurai" },
	{ EJob::Ninja, "Ninja" },
	{ EJob::Dragoon, "Dragoon" },
	{ EJob::Summoner, "Summoner" },
	{ EJob::BlueMage, "Blue Mage" },
	{ EJob::Corsair, "Corsair" },
	{ EJob::Puppetmaster, "Puppetmaster" },
	{ EJob::Dancer, "Dancer" },
	{ EJob::Scholar, "Scholar" },
	{ EJob::Geomancer, "Geomancer" },
	{ EJob::Runefencer, "Rune Fencer" },
	{ EJob::Monstrosity, "Monstrosity" }
};

static FString GetJobName(const EJob Job)
{
	if (const auto* Abbr = JobNames.Find(Job))
	{
		return *Abbr;
	}
	return TEXT("None");
}

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
	F8A, F8B,
	Invalid
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
struct SIGNETGAME_API FSignetSkillProgress
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGameplayTag SkillTag;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Rank = 0.0f; // Current skill level (e.g. 200.5)

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 SchemaVersion = 1;
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

USTRUCT(BlueprintType)
struct SIGNETGAME_API FSignetSavedBagEntry
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 ItemID = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Quantity = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGuid InstanceId; // valid for equipment, invalid for stack
};

USTRUCT(BlueprintType)
struct SIGNETGAME_API FSignetSavedInventory
{
	GENERATED_BODY()

	// Full bag, 1:1 with what we showed in the list widget
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FSignetSavedBagEntry> Bag;

	// Equipped by slot (index == (int32)EGearSlot)
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<int32> EquippedItemIDs;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FGuid> EquippedInstanceIds;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 SchemaVersion = 1;
};