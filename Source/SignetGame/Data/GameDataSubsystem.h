// Copyright Red Lotus Games, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "CharacterParts.h"
#include "VFXTypes.h"
#include "GameplayTagContainer.h"
#include "AudioTypes.h"
#include "SignetGame/Save/SignetSaveTypes.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "GameDataSubsystem.generated.h"

/**
 * Responsible for Loading and Caching things like
 * Character Parts, Equipment, Talent Trees
 */
UCLASS(Config=Game, DefaultConfig)
class SIGNETGAME_API UGameDataSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	
	UPROPERTY(Config, EditDefaultsOnly, Category="Game Data|Tables")
	TSoftObjectPtr<UDataTable> CharacterPartsTableRef;

	UPROPERTY(Config, EditDefaultsOnly, Category="Game Data|Tables")
	TSoftObjectPtr<UDataTable> CharacterWeaponTypesTableRef;

	UPROPERTY(Config, EditDefaultsOnly, Category="Game Data|Tables")
	TSoftObjectPtr<UDataTable> ItemTableRef;	
	
	UPROPERTY(Config, EditDefaultsOnly, Category="Game Data|Tables")
	TSoftObjectPtr<UDataTable> WeaponSoundTableRef;

	UPROPERTY(Config, EditDefaultsOnly, Category="Game Data|Tables")
	TSoftObjectPtr<UFootstepBankDataAsset> FootstepDataAssetRef;

	UPROPERTY(Config, EditDefaultsOnly, Category="Game Data|Tables")
	TSoftObjectPtr<UUISoundAsset> UISoundDataAssetRef;

	UPROPERTY(Config, EditDefaultsOnly, Category="Game Data|Tables")
	TSoftObjectPtr<UVFXDataAsset> GlobalVFXDataAssetRef;

	UPROPERTY(Config, EditDefaultsOnly, Category="Game Data|Tables")
	TSoftObjectPtr<USoundBase> CriticalImpactRef;

	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
	
	bool GetCharacterParts(const ERace& TargetRace, FCharacterPartsRow& OutRow);

	const FCharacterPartsRow* GetCharacterPartsRow(ERace InRace);

	const FCharacterWeaponTypeData* GetWeaponTypeRow(ERace InRace, const FGameplayTag& WeaponTypeTag);

	const FInventoryItem* GetItem(const int32 ItemId);

	const FWeaponSoundBankTableRow* GetWeaponSound(const FGameplayTag& WeaponTypeTag);

	USoundBase* GetFootstep(const EPhysicalSurface SurfaceType, const EArmorSoundClass ArmorClass);

	USoundBase* GetUISound(const EUISound SoundType);

	FVFXSystem* GetGlobalVFX(const EVFXType VFXType);

	USoundBase* GetCriticalImpactSound() { return CriticalImpactSound; }

private:

// Begin Character Parts Data
	
	bool bCharacterPartsLoaded = false;
	
	UPROPERTY(Transient)
	TObjectPtr<UDataTable> CharacterPartsTable = nullptr;

	UPROPERTY(Transient)
	TObjectPtr<UDataTable> CharacterWeaponTypesTable = nullptr;

	// Row Key Caches
	TMap<ERace, FCharacterPartsRow*> CharacterPartsCache;
	
	void EnsureCharacterPartsLoaded();

// Begin Item Data

	UPROPERTY(Transient)
	TObjectPtr<UDataTable> ItemTable = nullptr;
	TMap<int, FInventoryItem> ItemCache;
	
// Begin Weapon Sound Table

	UPROPERTY(Transient)
	TObjectPtr<USoundBase> CriticalImpactSound = nullptr;
	
	UPROPERTY(Transient)
	TObjectPtr<UDataTable> WeaponSoundTable = nullptr;

	TMap<FGameplayTag, FWeaponSoundBankTableRow*> WeaponSoundCache;

// Begin Footsteps

	UPROPERTY(Transient)
	TObjectPtr<UFootstepBankDataAsset> FootstepBank = nullptr;

// Begin UI Sounds

	UPROPERTY(Transient)
	TObjectPtr<UUISoundAsset> UISoundBank = nullptr;
	UPROPERTY(Transient)
	TMap<EUISound, USoundBase*> UISoundCache;

// Begin VFX

	UPROPERTY(Transient)
	UVFXDataAsset* GlobalVFX = nullptr;
};
