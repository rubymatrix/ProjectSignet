// Copyright Red Lotus Games, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "GameplayTagContainer.h"
#include "InventoryTypes.generated.h"

UENUM(BlueprintType)
enum class EGearSlot : uint8
{
	None = 0,
	Main,
	Sub,
	Ranged,
	Ammo,
	Head,
	Neck,
	LeftEar UMETA(DisplayName = "Left Ear"),
	RightEar UMETA(DisplayName = "Right Ear"),
	Body,
	Hands,
	LeftRing UMETA(DisplayName = "Left Ring"),
	RightRing UMETA(DisplayName = "Right Ring"),
	Back,
	Waist,
	Legs,
	Feet
};


UENUM(BlueprintType)
enum class EItemType : uint8
{
	None,
	Consumable,
	Equipment,
	Material
};

UENUM(BlueprintType)
enum class EEquipmentType : uint8
{
	None,
	Weapon,
	Ranged,
	Ammo,
	Armor,
	Accessory
};

UENUM(BlueprintType)
enum class ESoundSlot : uint8
{
	Default,
	Variant1,
	Variant2,
	Variant3
};

UENUM(BlueprintType)
enum class EFaceClipStage : uint8
{
	None,
	RemoveTop,
	RemoveAll
};

UENUM(BlueprintType)
enum class EItemQuality : uint8
{
	Normal,
	HQ,
	HQ2,
	HQ3
};

USTRUCT(BlueprintType)
struct SIGNETGAME_API FInventoryItem
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int ItemID = 0;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FString En;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FString Ja;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FString Enl;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FString Jal;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FString ItemDescription;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int MaxStackSize = 1;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Properties")
	EItemType ItemType = EItemType::None;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Properties")
	EItemQuality ItemQuality = EItemQuality::Normal;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Properties")
	EEquipmentType EquipmentType = EEquipmentType::None;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Properties")
	EGearSlot GearSlot = EGearSlot::None;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTag WeaponSkill = FGameplayTag::EmptyTag;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	ESoundSlot ImpactSoundType = ESoundSlot::Default;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	EFaceClipStage FaceClippingStage = EFaceClipStage::None;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Properties")
	TWeakObjectPtr<UDataTable> Stats;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Visual")
	TObjectPtr<UTexture> Icon;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Visual")
	TSoftObjectPtr<USkeletalMesh> Mesh;
};


USTRUCT(BlueprintType)
struct SIGNETGAME_API FItemRow : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FInventoryItem Item;
};