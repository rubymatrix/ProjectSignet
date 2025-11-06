// Copyright Red Lotus Games, All Rights Reserved.


#include "GameDataSubsystem.h"

#include "DataHelpers.h"
#include "SignetGame/Util/Logging.h"

void UGameDataSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	if (!CharacterPartsTableRef.IsNull())
	{
		CharacterPartsTable = CharacterPartsTableRef.LoadSynchronous();
		UE_LOG(LogTemp, Display, TEXT("Loaded CharacterPartsTable: %s"),
			*GetNameSafe(CharacterPartsTable));

		bCharacterPartsLoaded = true;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("CharacterPartsTableRef is null (check INI)."));
	}

	if (!ItemTableRef.IsNull())
	{
		ItemTable = ItemTableRef.LoadSynchronous();
		UE_LOG(LogTemp, Display, TEXT("Loaded ItemTable: %s"),
			*GetNameSafe(ItemTable));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("ItemTableRef is null (check INI)."));
	}

	if (!WeaponSoundTableRef.IsNull())
	{
		WeaponSoundTable = WeaponSoundTableRef.LoadSynchronous();
	}
	

	// Hydrate the item cache
	for (const auto& Pair : ItemTable->GetRowMap())
	{
		const auto* Row = reinterpret_cast<const FItemRow*>(Pair.Value);
		if (Row && Row->Item.ItemID != 0)
		{
			ItemCache.Add(Row->Item.ItemID, Row->Item);
		}
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

const FCharacterPartsRow* UGameDataSubsystem::GetCharacterPartsRow(const ERace InRace)
{
	if (!bCharacterPartsLoaded)
	{
		UE_LOG(LogSignetGameData, Error, TEXT("Request made to the GameDataSubsystem for %s Character Parts before character parts were loaded."), *Data::GetRowNameFromEnum(InRace).ToString());
		return nullptr;
	}

	if (CharacterPartsTable == nullptr)
	{
		UE_LOG(LogSignetGameData, Error, TEXT("Request made to the GameDataSubsystem for %s Character Parts, but CharacterPartsTable is NULL"), *Data::GetRowNameFromEnum(InRace).ToString());
		return nullptr;
	}

	if (const auto Result = CharacterPartsCache.Find(InRace))
	{
		return *Result;
	}

	if (const auto* Row = CharacterPartsTable->FindRow<FCharacterPartsRow>(Data::GetRowNameFromEnum(InRace), TEXT("PartsRowLookup")))
	{
		return Row;
	}

	return nullptr;
}

const FInventoryItem* UGameDataSubsystem::GetItem(const int32 ItemId)
{
	if (ItemTable == nullptr || ItemCache.IsEmpty()) return nullptr;
	return ItemCache.Find(ItemId);
}

const FWeaponSoundBankTableRow* UGameDataSubsystem::GetWeaponSound(const FGameplayTag& WeaponTypeTag)
{
	return nullptr;
}
