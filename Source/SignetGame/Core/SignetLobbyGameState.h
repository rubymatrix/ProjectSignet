// Copyright Red Lotus Games, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "ModularGameplayActors/GSCModularGameState.h"
#include "SignetLobbyGameState.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnLobbyPlayerJoined, ASignetPlayerState*, Player);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnLobbyPlayerLeft,   ASignetPlayerState*, Player);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnLobbyPlayerUpdated, ASignetPlayerState*, Player);

class ASignetPlayerState;
/**
 * 
 */
UCLASS()
class SIGNETGAME_API ASignetLobbyGameState : public AGSCModularGameState
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintAssignable, Category="Lobby")
	FOnLobbyPlayerJoined OnLobbyPlayerJoined;
	
	UPROPERTY(BlueprintAssignable, Category="Lobby")
	FOnLobbyPlayerLeft OnLobbyPlayerLeft;

	UPROPERTY(BlueprintAssignable, Category="Lobby")
	FOnLobbyPlayerUpdated OnLobbyPlayerUpdated;

	UFUNCTION(BlueprintPure, Category="Lobby")
	TArray<ASignetPlayerState*> GetLobbyPlayers(bool bSortByJoinOrder = true) const;
	
	UFUNCTION(BlueprintPure, Category="Lobby")
	ASignetPlayerState* GetLobbyLeader() const;

protected:

	virtual void AddPlayerState(APlayerState* PlayerState) override;
	virtual void RemovePlayerState(APlayerState* PlayerState) override;

private:

	UFUNCTION() void PlayerUpdateReceived(ASignetPlayerState* Player);
};
