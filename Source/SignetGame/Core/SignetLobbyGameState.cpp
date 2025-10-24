// Copyright Red Lotus Games, All Rights Reserved.

#include "SignetLobbyGameState.h"
#include "SignetGame/Player/SignetPlayerState.h"

TArray<ASignetPlayerState*> ASignetLobbyGameState::GetLobbyPlayers(bool bSortByJoinOrder) const
{
	TArray<ASignetPlayerState*> Out;
	for (APlayerState* PS : PlayerArray)
		if (auto* S = Cast<ASignetPlayerState>(PS)) Out.Add(S);

	if (bSortByJoinOrder)
	{
		Out.Sort([](const ASignetPlayerState& A, const ASignetPlayerState& B)
		{
			return A.GetPlayerId() < B.GetPlayerId();
		});
	}
	return Out;
}

ASignetPlayerState* ASignetLobbyGameState::GetLobbyLeader() const
{
	for (APlayerState* PS : PlayerArray)
		if (auto* S = Cast<ASignetPlayerState>(PS); S && S->IsLobbyHost()) return S;
	return nullptr;
}

void ASignetLobbyGameState::AddPlayerState(APlayerState* PS)
{
	Super::AddPlayerState(PS);
	if (ASignetPlayerState* S = Cast<ASignetPlayerState>(PS))
	{
		S->OnPlayerUpdated.AddDynamic(this, &ASignetLobbyGameState::PlayerUpdateReceived);
		OnLobbyPlayerJoined.Broadcast(S); 
	}
}

void ASignetLobbyGameState::RemovePlayerState(APlayerState* PS)
{
	if (ASignetPlayerState* S = Cast<ASignetPlayerState>(PS))
	{
		S->OnPlayerUpdated.RemoveAll(this);
		OnLobbyPlayerLeft.Broadcast(S);
	}
	Super::RemovePlayerState(PS);
}

void ASignetLobbyGameState::PlayerUpdateReceived(ASignetPlayerState* Player)
{
	OnLobbyPlayerUpdated.Broadcast(Player);	
}
