// Copyright Red Lotus Games, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "EditorSubsystem.h"
#include "SignetGame/Data/CharacterParts.h"
#include "SignetEditorSubsystem.generated.h"

/**
 * 
 */
UCLASS()
class SIGNETGAMEEDITOR_API USignetEditorSubsystem : public UEditorSubsystem
{
	GENERATED_BODY()

public:

	UPROPERTY(EditDefaultsOnly, Category = "Editor|Game Data")
	TSoftObjectPtr<UDataTable> CharacterPartsTableRef;
	
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
	
	const FCharacterPartsRow* FindCharacterParts(ERace Race);

	const USoundBase* FindVocalization(EVocalizationType Vocalization, const ERace& Race = ERace::HumeMale, const EFace& Face = EFace::F1A);
	
private:
	
	UPROPERTY(Transient)
	TObjectPtr<UDataTable> CharacterPartsTable = nullptr;
};
