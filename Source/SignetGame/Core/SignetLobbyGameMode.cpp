// Copyright Red Lotus Games, All Rights Reserved.

#include "SignetLobbyGameMode.h"
#include "SignetLobbyGameState.h"
#include "Engine/World.h"
#include "GameFramework/GameStateBase.h"
#include "SignetGame/Debug/SignetDebugHUD.h"
#include "SignetGame/Player/SignetPlayerController.h"
#include "SignetGame/Player/SignetPlayerState.h"

ASignetLobbyGameMode::ASignetLobbyGameMode()
{
	bUseSeamlessTravel = true; // keep connections when traveling to dungeon
	
	PlayerStateClass = ASignetPlayerState::StaticClass();
	PlayerControllerClass = ASignetPlayerController::StaticClass();
	HUDClass = ASignetDebugHUD::StaticClass();
	GameStateClass = ASignetLobbyGameState::StaticClass();
}

void ASignetLobbyGameMode::PreLogin(const FString& Options, const FString& Address, const FUniqueNetIdRepl& UniqueId, FString& ErrorMessage)
{
	Super::PreLogin(Options, Address, UniqueId, ErrorMessage);

	// const auto Current = GameState ? GameState->PlayerArray.Num() : 0;
	// if (constexpr auto MaxPlayers = 6; Current >= MaxPlayers)
	// {
	// 	ErrorMessage = TEXT("ServerFull");
	// }
}

void ASignetLobbyGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);
	
	if (auto* PC = Cast<ASignetPlayerController>(NewPlayer))
	{
		PC->ClientRequestProfileSnapshot();
	}

	ElectLeaderIfNone();
}

void ASignetLobbyGameMode::ElectLeaderIfNone()
{
	if (!HasAuthority() || !GameState) return;

	if (!FindCurrentLeader())
	{
		// Pick the lowest PlayerId (first joiner)
		APlayerState* Best = nullptr;
		for (APlayerState* PS : GameState->PlayerArray)
		{
			if (!Best || PS->GetPlayerId() < Best->GetPlayerId())
			{
				Best = PS;
			}
		}
		if (auto* S = Cast<ASignetPlayerState>(Best))
		{
			S->bIsHost = true;
		}
	}
}

ASignetPlayerState* ASignetLobbyGameMode::FindCurrentLeader() const
{
	for (APlayerState* PS : GameState->PlayerArray)
	{
		if (auto* S = Cast<ASignetPlayerState>(PS))
		{
			if (S->bIsHost) return S;
		}
	}
	return nullptr;
}

void ASignetLobbyGameMode::StartDungeon(const FString& MapAssetPath)
{
	if (!HasAuthority()) return;
	const FString Url = FString::Printf(TEXT("%s?listen"), *MapAssetPath);
	GetWorld()->ServerTravel(Url, true);
}

void ASignetLobbyGameMode::SetLobbyLeader(ASignetPlayerState* NewLeaderPS)
{
	if (!HasAuthority() || !NewLeaderPS) return;

	// Clear any existing leader
	if (ASignetPlayerState* Current = FindCurrentLeader())
	{
		Current->bIsHost = false;
	}
	NewLeaderPS->bIsHost = true;
}