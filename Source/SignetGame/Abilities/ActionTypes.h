#pragma once
#include "CoreMinimal.h"
#include "ActionTypes.generated.h"

UENUM(BlueprintType)
enum class EMontageType : uint8
{
	None,
	Engage,
	Disengage,
	StartResting,
	StopResting,
	Sit,
	StopSitting,
	Jump,
	Die,
	Raise,
	Attack,
	SubAttack,
	KickAttack
};