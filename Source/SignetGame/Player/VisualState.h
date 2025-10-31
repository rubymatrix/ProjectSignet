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

	UPROPERTY(VisibleAnywhere) int32 MainID = 0;
	UPROPERTY(VisibleAnywhere) int32 SubID = 0;
	UPROPERTY(VisibleAnywhere) int32 RangeID = 0;
	UPROPERTY(VisibleAnywhere) int32 HeadID = 0;
	UPROPERTY(VisibleAnywhere) int32 BodyID = 0;
	UPROPERTY(VisibleAnywhere) int32 HandsID = 0;
	UPROPERTY(VisibleAnywhere) int32 LegsID = 0;
	UPROPERTY(VisibleAnywhere) int32 FeetID = 0;
};