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
};


