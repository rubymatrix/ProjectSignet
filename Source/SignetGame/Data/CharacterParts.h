// Copyright Red Lotus Games, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "SignetGame/Inventory/InventoryTypes.h"
#include "SignetGame/Save/SignetSaveTypes.h"
#include "CharacterParts.generated.h"


USTRUCT(BlueprintType)
struct SIGNETGAME_API FCharacterPartsRow : public FTableRowBase
{
	GENERATED_BODY()

// Begin Data Properties
	
	UPROPERTY(EditDefaultsOnly)
	ERace RowNameAsEnum = ERace::HumeMale;
	
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UAnimInstance> AnimInstanceClass;

	UPROPERTY(EditDefaultsOnly)
	TSoftObjectPtr<USkeletalMesh> RootMesh;

	UPROPERTY(EditDefaultsOnly)
	TMap<EFace, TSoftObjectPtr<USkeletalMesh>> Faces;

	UPROPERTY(EditDefaultsOnly)
	TMap<EGearSlot, TSoftObjectPtr<USkeletalMesh>> SlotDefaults;


// Begin Access Helpers

	FORCEINLINE TSoftObjectPtr<USkeletalMesh> GetFace(const EFace& Face)
	{
		const TSoftObjectPtr<USkeletalMesh>* FoundFace = Faces.Find(Face);
		return FoundFace ? *FoundFace : nullptr;
	}

	FORCEINLINE TSoftObjectPtr<USkeletalMesh> GetDefaultSlot(const EGearSlot& Sot)
	{
		const TSoftObjectPtr<USkeletalMesh>* FoundSlot = SlotDefaults.Find(Sot);
		return FoundSlot ? *FoundSlot : nullptr;
	}
};