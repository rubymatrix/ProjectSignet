// Copyright Red Lotus Games, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "GameplayTagContainer.h"
#include "InventoryTypes.generated.h"

UENUM(BlueprintType)
enum class EGearSlot : uint8
{
	None,
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
	Equipment
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

USTRUCT(BlueprintType)
struct SIGNETGAME_API FInventoryItem
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int ItemID = 0;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FString ItemDescription;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Properties")
	EItemType ItemType = EItemType::None;

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
	TSoftObjectPtr<USkeletalMesh> Mesh;
};


USTRUCT(BlueprintType)
struct SIGNETGAME_API FItemRow : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FInventoryItem Item;
};