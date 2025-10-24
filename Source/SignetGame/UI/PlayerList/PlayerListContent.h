#pragma once
#include "CoreMinimal.h"
#include "PlayerListContent.generated.h"

USTRUCT(BlueprintType)
struct SIGNETGAME_API FPlayerListContent
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere) int32 PlayerId = 0;
	UPROPERTY(EditAnywhere) bool bIsLeader = false;
	UPROPERTY(EditAnywhere) FText PlayerName;
	UPROPERTY(EditAnywhere) FText Job;
	UPROPERTY(EditAnywhere) int Level = 1;
};
