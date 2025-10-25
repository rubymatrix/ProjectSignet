// Copyright Red Lotus Games, All Rights Reserved.

#pragma once
#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "SignetGame/Inventory/InventoryTypes.h"
#include "ModularAnimData.generated.h"


UENUM(BlueprintType)
enum class EAnimDirection : uint8
{
	None,
	Front,
	Back,
	Right,
	Left
};

USTRUCT(BlueprintType)
struct SIGNETGAME_API FLayeredSequence
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly) TObjectPtr<UAnimSequence> Lower;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly) TObjectPtr<UAnimSequence> Upper;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly) TObjectPtr<UAnimSequence> Waist;
};

USTRUCT(BlueprintType)
struct SIGNETGAME_API FWeaponOverlay
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "State")
	FLayeredSequence Idle;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "State")
	FLayeredSequence Dead;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly,Category = "State")
	FLayeredSequence Walking;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly,Category = "State")
	FLayeredSequence Running;
};

USTRUCT(BlueprintType)
struct SIGNETGAME_API FModularAnimData
{
	GENERATED_BODY()

// Begin Core State AnimData
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly,Category = "State")
	FLayeredSequence Idle;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly,Category = "State")
	FLayeredSequence Engaged;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly,Category = "State")
	FLayeredSequence Dead;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly,Category = "State")
	FLayeredSequence Walking;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly,Category = "State")
	FLayeredSequence Running;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly,Category = "State")
	FLayeredSequence Resting;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly,Category = "State")
	FLayeredSequence Sitting;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly,Category = "State")
	FLayeredSequence Crafting;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly,Category = "State")
	FLayeredSequence Falling;
	
};