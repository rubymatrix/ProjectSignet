// Copyright Red Lotus Games, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType)
enum class EGearSlot : uint8
{
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