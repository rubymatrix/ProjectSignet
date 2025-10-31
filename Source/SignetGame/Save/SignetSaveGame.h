// Copyright Red Lotus Games, All Rights Reserved.

#pragma once
#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "SignetSaveTypes.h"
#include "SignetSaveGame.generated.h"

UCLASS(ClassGroup=(Signet))
class SIGNETGAME_API USignetSaveGame : public USaveGame
{
	GENERATED_BODY()
	
public:
	
	UPROPERTY() 
	int SaveVersion = 1;
	UPROPERTY() 
	FString SaveGuid;
	UPROPERTY()
	FString CreatedAtUtc;
	UPROPERTY()
	FString UpdatedAtUtc;
	UPROPERTY()
	int BuildChangelist = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite) 
	FSignetPlayerProfile Profile;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite) 
	FSignetCurrencyWallet Wallet;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<EJob, FSignetJobProgress> Jobs;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FSignetSavedInventory Inventory;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<FString, FString> UserSettings;

	UPROPERTY()
	int Checksum = 0;

public:
	
	static constexpr int CURRENT_VERSION = 1;
	
};