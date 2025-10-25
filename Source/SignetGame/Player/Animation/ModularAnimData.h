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

	UPROPERTY(EditDefaultsOnly) TObjectPtr<UAnimSequence> Lower;
	UPROPERTY(EditDefaultsOnly) TObjectPtr<UAnimSequence> Upper;
	UPROPERTY(EditDefaultsOnly) TObjectPtr<UAnimSequence> Right;
};

USTRUCT(BlueprintType)
struct SIGNETGAME_API FWeaponOverlay
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, Category = "State")
	FLayeredSequence Idle;
	UPROPERTY(EditDefaultsOnly, Category = "State")
	FLayeredSequence Dead;
	UPROPERTY(EditDefaultsOnly, Category = "State")
	FLayeredSequence Walking;
	UPROPERTY(EditDefaultsOnly, Category = "State")
	FLayeredSequence Running;
};

USTRUCT(BlueprintType)
struct SIGNETGAME_API FModularAnimData
{
	GENERATED_BODY()

// Begin Core State Animations
	
	UPROPERTY(EditDefaultsOnly, Category = "State")
	FLayeredSequence Idle;
	UPROPERTY(EditDefaultsOnly, Category = "State")
	FLayeredSequence Dead;
	UPROPERTY(EditDefaultsOnly, Category = "State")
	FLayeredSequence Walking;
	UPROPERTY(EditDefaultsOnly, Category = "State")
	FLayeredSequence Running;
	UPROPERTY(EditDefaultsOnly, Category = "State")
	FLayeredSequence Resting;
	UPROPERTY(EditDefaultsOnly, Category = "State")
	FLayeredSequence Sitting;
	UPROPERTY(EditDefaultsOnly, Category = "State")
	FLayeredSequence Crafting;
	UPROPERTY(EditDefaultsOnly, Category = "State")
	FLayeredSequence Falling;
	
};