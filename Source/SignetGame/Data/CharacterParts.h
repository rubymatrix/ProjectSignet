// Copyright Red Lotus Games, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "SignetGame/Inventory/InventoryTypes.h"
#include "SignetGame/Save/SignetSaveTypes.h"
#include "CharacterParts.generated.h"


USTRUCT(BlueprintType)
struct SIGNETGAME_API FFaceMesh
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<USkeletalMesh> Mesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<USkeletalMesh> ClipStage1;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<USkeletalMesh> ClipStage2;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UMaterialInstance> Material;
};

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
	TObjectPtr<USkeletalMesh> RootMesh;

	UPROPERTY(EditDefaultsOnly)
	TMap<EFace, FFaceMesh> Faces;

	UPROPERTY(EditDefaultsOnly)
	TMap<EGearSlot, TObjectPtr<USkeletalMesh>> SlotDefaults;


// Begin Access Helpers

	const FFaceMesh* GetFace(const EFace& Face, const EFaceClipStage ClipStage = EFaceClipStage::None)
	{
		return Faces.Find(Face);
	}

	FORCEINLINE TSoftObjectPtr<USkeletalMesh> GetDefaultSlot(const EGearSlot& Sot)
	{
		const TObjectPtr<USkeletalMesh>* FoundSlot = SlotDefaults.Find(Sot);
		return FoundSlot ? *FoundSlot : nullptr;
	}
};