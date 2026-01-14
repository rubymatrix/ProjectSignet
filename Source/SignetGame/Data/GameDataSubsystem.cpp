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

	if (!CharacterWeaponTypesTableRef.IsNull())
	{
		CharacterWeaponTypesTable = CharacterWeaponTypesTableRef.LoadSynchronous();
		UE_LOG(LogTemp, Display, TEXT("Loaded CharacterWeaponTypeTable: %s"),
			*GetNameSafe(CharacterWeaponTypesTable));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("CharacterWeaponTypeTableRef is null (check INI)."));
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

	if (!FootstepDataAssetRef.IsNull())
	{
		FootstepBank = FootstepDataAssetRef.LoadSynchronous();
	}

	if (!UISoundDataAssetRef.IsNull())
	{
		UISoundBank = UISoundDataAssetRef.LoadSynchronous();
	}

	if (!GlobalVFXDataAssetRef.IsNull())
	{
		GlobalVFX = GlobalVFXDataAssetRef.LoadSynchronous();
	}

	if (!CriticalImpactRef.IsNull())
	{
		CriticalImpactSound = CriticalImpactRef.LoadSynchronous();
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

const FCharacterWeaponTypeData* UGameDataSubsystem::GetWeaponTypeRow(ERace InRace, const FGameplayTag& WeaponTypeTag)
{
	if (CharacterWeaponTypesTable == nullptr)
	{
		UE_LOG(LogSignetGameData, Error, TEXT("Request made to the GameDataSubsystem for %s WeaponType, but Weapon Type Table is NULL"), *Data::GetRowNameFromEnum(InRace).ToString());
	}

	if (const auto* Row = CharacterWeaponTypesTable->FindRow<FCharacterWeaponTypeRow>(Data::GetRowNameFromEnum(InRace), TEXT("WeaponTypeLookup")))
	{
		if (const auto* Res = Row->WeaponTypes.Find(WeaponTypeTag))
		{
			return Res;
		}
	}
	
	return nullptr;
}

const FInventoryItem* UGameDataSubsystem::GetItem(const int32 ItemId)
{
	if (ItemTable == nullptr || ItemCache.IsEmpty() || ItemId == 0) return nullptr;
	return ItemCache.Find(ItemId);
}

const FWeaponSoundBankTableRow* UGameDataSubsystem::GetWeaponSound(const FGameplayTag& WeaponTypeTag)
{
	if (!WeaponTypeTag.IsValid())
	{
		return nullptr;
	}

	if (const auto FoundRow = WeaponSoundCache.Find(WeaponTypeTag))
	{
		return *FoundRow;
	}

	if (WeaponSoundTable == nullptr && !WeaponSoundTableRef.IsNull())
	{
		WeaponSoundTable = WeaponSoundTableRef.LoadSynchronous();
	}

	if (WeaponSoundTable)
	{
		if (const auto Row = WeaponSoundTable->FindRow<FWeaponSoundBankTableRow>(WeaponTypeTag.GetTagName(), TEXT("WeaponSoundLookup")))
		{
			WeaponSoundCache.Add(WeaponTypeTag, const_cast<FWeaponSoundBankTableRow*>(Row));
			return Row;
		}
		
		UE_LOG(LogSignetGameData, Warning, TEXT("GetWeaponSound: Row not found for tag [%s]"), *WeaponTypeTag.ToString());
	}
	else
	{
		UE_LOG(LogSignetGameData, Error, TEXT("GetWeaponSound: WeaponSoundTable is null and could not be loaded."));
	}

	return nullptr;
}

USoundBase* UGameDataSubsystem::GetFootstep(const EPhysicalSurface SurfaceType, const EArmorSoundClass ArmorClass)
{
	if (FootstepBank)
	{
		if (const auto SoundBank = FootstepBank->FootstepSounds.Find(ArmorClass))
		{
			if (const auto Sound = SoundBank->FootstepMaterialMap.Find(SurfaceType))
			{
				return *Sound;
			}
		}

		if (const auto Default = FootstepBank->FootstepSounds.Find(EArmorSoundClass::Cloth))
		{
			if (const auto Sound = Default->FootstepMaterialMap.Find(SurfaceType1))
			{
				return *Sound;
			}
		}
	}
	return nullptr;
}

USoundBase* UGameDataSubsystem::GetUISound(const EUISound SoundType)
{
	if (UISoundCache.Contains(SoundType))
	{
		return *UISoundCache.Find(SoundType);
	}
	
	if (UISoundBank)
	{
		if (const auto FoundSound = UISoundBank->UISounds.Find(SoundType))
		{
			UISoundCache.Add(SoundType, *FoundSound);
			return *FoundSound;
		}
	}

	return nullptr;
}

FVFXSystem* UGameDataSubsystem::GetGlobalVFX(const EVFXType VFXType)
{
	if (GlobalVFX == nullptr)
	{
		return nullptr;
	}

	if (const auto System = GlobalVFX->VFXSystems.Find(VFXType))
	{
		return System;
	}

	return nullptr;
}

