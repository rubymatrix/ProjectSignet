// Copyright Red Lotus Games, All Rights Reserved.


#include "GameDataSubsystem.h"

#include "DataHelpers.h"
#include "SignetGame/Util/Logging.h"

void UGameDataSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	if (CharacterPartsTableRef.IsNull())
	{
		UE_LOG(LogSignetGameData, Error, TEXT("Missing reference to Character Parts Table.  GameDataSubsystem will not funciton."));
		return;
	}

	CharacterPartsTable = CharacterPartsTableRef.LoadSynchronous();
	if (CharacterPartsTable == nullptr)
	{
		UE_LOG(LogSignetGameData, Error, TEXT("CharacterPartsTable failed to load and is NULL."));
	}
	else
	{
		bCharacterPartsLoaded = true;
	}
}

void UGameDataSubsystem::Deinitialize()
{
	Super::Deinitialize();
}

bool UGameDataSubsystem::GetCharacterParts(const ERace& TargetRace, FCharacterPartsRow& OutRow)
{
	if (!bCharacterPartsLoaded)
	{
		UE_LOG(LogSignetGameData, Error, TEXT("Request made to the GameDataSubsystem for %s Character Parts before character parts were loaded."), *Data::GetRowNameFromEnum(TargetRace).ToString());
		return false;
	}

	if (CharacterPartsTable == nullptr)
	{
		UE_LOG(LogSignetGameData, Error, TEXT("Request made to the GameDataSubsystem for %s Character Parts, but CharacterPartsTable is NULL"), *Data::GetRowNameFromEnum(TargetRace).ToString());
		return false;
	}

	if (const auto FoundPartsCache = CharacterPartsCache.Find(TargetRace))
	{
		OutRow = **FoundPartsCache;
		return true;
	}

	if (const auto* Row = CharacterPartsTable->FindRow<FCharacterPartsRow>(Data::GetRowNameFromEnum(TargetRace), TEXT("PartsRowLookup")))
	{
		OutRow = *Row;
		return true;
	}

	UE_LOG(LogSignetGameData, Error, TEXT("Requested %s but no row found matching."), *Data::GetRowNameFromEnum(TargetRace).ToString());
	return false;
}
