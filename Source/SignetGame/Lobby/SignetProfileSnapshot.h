// Copyright Red Lotus Games, All Rights Reserved.

#pragma once
#include "CoreMinimal.h"
#include "SignetGame/Save/SignetSaveTypes.h"
#include "SignetProfileSnapshot.generated.h"

USTRUCT(BlueprintType)
struct SIGNETGAME_API FSignetProfileSnapshot
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere) FString PlayerName;
	UPROPERTY(BlueprintReadWrite, EditAnywhere) ENation Nation = ENation::Bastok;
	UPROPERTY(BlueprintReadWrite, EditAnywhere) ERace   Race   = ERace::HumeMale;
	UPROPERTY(BlueprintReadWrite, EditAnywhere) EFace   Face   = EFace::F1A;
	UPROPERTY(BlueprintReadWrite, EditAnywhere) EJob    Job    = EJob::Warrior;
	UPROPERTY(BlueprintReadWrite, EditAnywhere) int32   Level  = 1;
	UPROPERTY(BlueprintReadWrite, EditAnywhere) int64   CurrentExp = 0;
};