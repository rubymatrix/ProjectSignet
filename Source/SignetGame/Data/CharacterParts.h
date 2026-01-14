// Copyright Red Lotus Games, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "SignetGame/Abilities/ActionTypes.h"
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
struct SIGNETGAME_API FActionData
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TMap<EMontageType, UAnimMontage*> ActionMontages;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<UAnimMontage*> AttackMontages;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<UAnimMontage*> SubAttackMontages;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<UAnimMontage*> KickAttackMontages;
};

USTRUCT(BlueprintType)
struct SIGNETGAME_API FCharacterPartsRow : public FTableRowBase
{
	GENERATED_BODY()

// Begin Data Properties
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	ERace RowNameAsEnum = ERace::HumeMale;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UAnimInstance> AnimInstanceClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<USkeletalMesh> RootMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TMap<EFace, FFaceMesh> Faces;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TMap<EGearSlot, TObjectPtr<USkeletalMesh>> SlotDefaults;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TMap<EGearSlot, int32> StartingEquipment;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FActionData Actions;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<FVoiceBank> Voices;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FFootstepBank Footsteps;


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


UENUM(BlueprintType)
enum class EActionDirection : uint8
{
	Front,
	Back,
	Left,
	Right
};


USTRUCT(BlueprintType)
struct SIGNETGAME_API FCharacterWeaponTypeData
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)	
	TMap<EMontageType, UAnimMontage*> ActionMontages;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<UAnimMontage*> MainAttackMontages;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<UAnimMontage*> SubAttackMontages;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<UAnimMontage*> KickAttackMontages;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TMap<EActionDirection, UAnimMontage*> DirectionalAttackMontages;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TMap<EActionDirection, UAnimMontage*> DirectionalReactMontages;
};


USTRUCT(BlueprintType)
struct SIGNETGAME_API FCharacterWeaponTypeRow : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TMap<FGameplayTag, FCharacterWeaponTypeData> WeaponTypes;
};