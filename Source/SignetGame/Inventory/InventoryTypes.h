// Copyright Red Lotus Games, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "AttributeSet.h"
#include "GameplayEffect.h"
#include "GameplayTagContainer.h"
#include "SignetGame/Data/AudioTypes.h"
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

UENUM(BlueprintType, meta = (Bitflags, UseEnumValuesAsMaskValuesInEditor = "true"))
enum class EBodyPart : uint8
{
	None  = 0b00000000 UMETA(Hidden),
	Head  = 0b00000001,
	Body  = 0b00000010,
	Hands = 0b00000100,
	Legs  = 0b00001000,
	Feet  = 0b00010000
};
ENUM_CLASS_FLAGS(EBodyPart)


// 0001 1100

USTRUCT(BlueprintType)
struct SIGNETGAME_API FItemStatModifier
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Stats")
	FGameplayAttribute Attribute;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Stats")
	bool bIsPercent = false;

	// Percent modifiers use 0.20 for +20%.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Stats")
	float Magnitude = 0.0f;
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

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Properties")
	FGameplayTagContainer EquippableRaces;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Properties")
	FGameplayTagContainer EquippableJobs;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Properties")
	FGameplayTag WeaponSkill = FGameplayTag::EmptyTag;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Properties")
	TSubclassOf<UGameplayEffect> EffectClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Stats")
	TArray<FItemStatModifier> StatModifiers;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category ="Masking")
	EFaceClipStage FaceClippingStage = EFaceClipStage::None;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (Bitmask, BitmaskEnum = EHideParts), Category = "Masking")
	int32 HiddenPartsMask = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (Bitmask, BitmaskEnum = EHideParts), Category = "Masking")
	int32 RemoveSlotsMask = 0;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Visual")
	TObjectPtr<UTexture> Icon;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Visual")
	FString MeshPath;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Visual")
	FString OffhandMeshPath;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Sound/FX")
	EWeaponSoundSlot WeaponSoundType = EWeaponSoundSlot::Slot1;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Sound/FX")
	EArmorSoundClass ArmorSoundClass = EArmorSoundClass::Cloth;
};


USTRUCT(BlueprintType)
struct SIGNETGAME_API FItemRow : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FInventoryItem Item;
};


static bool HidesPart(const FInventoryItem* ItemDef, EBodyPart Part)
{
	if (!ItemDef) return false;

	if (const EBodyPart Flags = static_cast<EBodyPart>(ItemDef->HiddenPartsMask); EnumHasAnyFlags(Flags, Part))
	{
		return true;
	}
	
	return false;
}
