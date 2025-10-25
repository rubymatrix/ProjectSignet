// Copyright Red Lotus Games, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "SignetGame/Save/SignetSaveTypes.h"
#include "VisualState.generated.h"

USTRUCT(BlueprintType)
struct SIGNETGAME_API FVisualState
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	ERace Race = ERace::HumeMale;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	EFace Face = EFace::F1A;

	UPROPERTY(VisibleAnywhere) uint16 MainID = 0;
	UPROPERTY(VisibleAnywhere) uint16 SubID = 0;
	UPROPERTY(VisibleAnywhere) uint16 RangeID = 0;
	UPROPERTY(VisibleAnywhere) uint16 HeadID = 0;
	UPROPERTY(VisibleAnywhere) uint16 BodyID = 0;
	UPROPERTY(VisibleAnywhere) uint16 HandsID = 0;
	UPROPERTY(VisibleAnywhere) uint16 LegsID = 0;
	UPROPERTY(VisibleAnywhere) uint16 FeetID = 0;
};