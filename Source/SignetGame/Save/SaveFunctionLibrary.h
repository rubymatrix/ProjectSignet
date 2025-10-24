// Copyright Red Lotus Games, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "SaveFunctionLibrary.generated.h"

enum class ECurrencyType : uint8;
enum class EJob : uint8;
class USignetSaveGame;

/**
 * Collection of functions to assist with interacting with the player's profile
 */
UCLASS(ClassGroup=(Signet))
class SIGNETGAME_API USaveFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, BlueprintPure, CallInEditor)
	static USignetSaveGame* GetPlayerData(UWorld* WorldContext);

	UFUNCTION(BlueprintCallable, CallInEditor)
	static void SetPlayerName(UWorld* WorldContext, const FString& NewName);

	UFUNCTION(BlueprintCallable, CallInEditor)
	static int AddCurrency(UWorld* WorldContext, const ECurrencyType& CurrencyType, int Delta);

	UFUNCTION(BlueprintCallable, CallInEditor)
	static void SetJobLevel(UWorld* WorldContext, const EJob& JobType, int NewLevel);
};
