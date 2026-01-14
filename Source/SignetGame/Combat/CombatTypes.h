// Copyright Red Lotus Games, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "SignetGame/Inventory/InventoryTypes.h"
#include "CombatTypes.generated.h"

UENUM(BlueprintType)
enum class EPhysicalAttackType : uint8
{
	Normal,
	Double,
	Triple,
	Quad,
	Kick,
	Daken,
	Zanshin,
	Samba,
	FollowUp
};

UENUM(BlueprintType)
enum class EPhysicalAttackDirection : uint8
{
	Right,
	Left
};

USTRUCT(BlueprintType)
struct FAttackResult
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsHit = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsCritical = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsGuarded = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsParried = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsBlocked = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsEvaded = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsCountered = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsAnticipated = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsSA = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsTA = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsCovered = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EPhysicalAttackType AttackType = EPhysicalAttackType::Normal;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EPhysicalAttackDirection AttackDirection = EPhysicalAttackDirection::Right;
};
