// Copyright Red Lotus Games, All Rights Reserved.

#include "SignetNetSubsystem.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "SignetGame/Util/Logging.h"

void USignetNetSubsystem::HostLocalLobby(int32 Port)
{
	if (!GetWorld()) return;

	// If we’re already a listen server in Lobby, skip
	if (IsAlreadyHosting()) return;

	if (IsRunningDedicatedServer()) return;

	// Force open Lobby map as listen server
	UE_LOG(LogSignetNet, Display, TEXT("Opening Lobby Listen Server on port %d"), Port);
	
	const auto Options = FString::Printf(TEXT("?listen?Port=%d"), Port);
	UGameplayStatics::OpenLevel(GetWorld(), FName(TEXT("L_Lobby")), true, Options);
}

void USignetNetSubsystem::ConnectToHost(const FString& Address)
{
	if (!GetWorld()) return;
	
	const auto URL = Address;
	if (const auto PC = UGameplayStatics::GetPlayerController(GetWorld(), 0))
	{
		UE_LOG(LogSignetNet, Display, TEXT("Client is connecting to host at %s"), *URL);
		PC->ClientTravel(URL, ETravelType::TRAVEL_Absolute);
	}
}

bool USignetNetSubsystem::IsAlreadyHosting() const
{
	const auto World = GetWorld();
	if (!World) return false;
	return World->GetNetMode() == NM_ListenServer;
}
