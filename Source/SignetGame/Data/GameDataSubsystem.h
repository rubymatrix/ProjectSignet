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
UCLASS()
class SIGNETGAME_API UGameDataSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:

	UPROPERTY(EditDefaultsOnly, Category="Game Data|Tables")
	TSoftObjectPtr<UDataTable> CharacterPartsTableRef;

	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
	
	bool GetCharacterParts(const ERace& TargetRace, FCharacterPartsRow& OutRow);

private:

// Begin Character Parts Data
	
	bool bCharacterPartsLoaded = false;
	
	UPROPERTY(Transient)
	TObjectPtr<UDataTable> CharacterPartsTable = nullptr;

	// Row Key Caches
	TMap<ERace, FCharacterPartsRow*> CharacterPartsCache;
	
	void EnsureCharacterPartsLoaded();
	
};
