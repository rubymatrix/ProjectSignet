// Copyright Red Lotus Games, All Rights Reserved.

#pragma once
#include "CoreMinimal.h"
#include "ModularGameplayActors/GSCModularGameMode.h"
#include "SignetLobbyGameMode.generated.h"

class ASignetPlayerState;

UCLASS()
class SIGNETGAME_API ASignetLobbyGameMode : public AGSCModularGameMode
{
	GENERATED_BODY()
public:
	ASignetLobbyGameMode();
	
	UFUNCTION(BlueprintCallable)
	void StartDungeon(const FString& MapAssetPath);

	UFUNCTION(BlueprintCallable)
	void SetLobbyLeader(ASignetPlayerState* NewLeaderPS);

protected:

	virtual void PreLogin(const FString& Options, const FString& Address, const FUniqueNetIdRepl& UniqueId, FString& ErrorMessage) override;

	virtual void PostLogin(APlayerController* NewPlayer) override;
	
	virtual bool CanServerTravel(const FString& URL, bool bAbsolute) override { return true; }

private:

	void ElectLeaderIfNone();

	ASignetPlayerState* FindCurrentLeader() const;

public:
	
	UFUNCTION(Exec) void InvPrintTarget(const FString& Target);
	UFUNCTION(Exec) void GiveItemTarget(const FString& Target, int32 ItemID, int32 Qty);
	UFUNCTION(Exec) void EquipIDTarget(const FString& Target, int32 Slot, int32 ItemID);
	UFUNCTION(Exec) void UnequipTarget(const FString& Target, int32 Slot);

private:
	APlayerController* ResolveTargetPC(const FString& Target) const; // nullptr means not found
	void PrintInventoryForPC(APlayerController* PC) const;
};


