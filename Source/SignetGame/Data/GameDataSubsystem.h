// Copyright Red Lotus Games, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "CharacterParts.h"
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
	TSoftObjectPtr<UDataTable> ItemTableRef;

	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
	
	bool GetCharacterParts(const ERace& TargetRace, FCharacterPartsRow& OutRow);

	const FInventoryItem* GetItem(const int32 ItemId);

private:

// Begin Character Parts Data
	
	bool bCharacterPartsLoaded = false;
	
	UPROPERTY(Transient)
	TObjectPtr<UDataTable> CharacterPartsTable = nullptr;

	// Row Key Caches
	TMap<ERace, FCharacterPartsRow*> CharacterPartsCache;
	
	void EnsureCharacterPartsLoaded();

// Begin Item Data

	UPROPERTY(Transient)
	TObjectPtr<UDataTable> ItemTable = nullptr;
	TMap<int, FInventoryItem> ItemCache;
	
};
