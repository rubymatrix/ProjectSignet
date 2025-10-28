// Copyright Red Lotus Games, All Rights Reserved.

#include "SignetLobbyGameMode.h"
#include "SignetLobbyGameState.h"
#include "Engine/World.h"
#include "GameFramework/CheatManager.h"
#include "GameFramework/GameStateBase.h"
#include "SignetGame/Debug/SignetDebugHUD.h"
#include "SignetGame/Inventory/SignetInventoryComponent.h"
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

	if (IsValid(NewPlayer) && NewPlayer->CheatClass)
	{
		// Ensure the CheatManager exists and is ready on both listen server & clients
		NewPlayer->EnableCheats(); // creates and InitCheatManager() internally
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
	
	if (ASignetPlayerState* Current = FindCurrentLeader())
	{
		Current->bIsHost = false;
	}
	NewLeaderPS->bIsHost = true;
}

APlayerController* ASignetLobbyGameMode::ResolveTargetPC(const FString& Target) const
{
    UWorld* World = GetWorld();
    if (!World) return nullptr;
	
    if (Target.Len() > 1 && Target[0] == TCHAR('#'))
    {
        int32 Index = FCString::Atoi(*Target.Mid(1));
        int32 i = 0;
        for (FConstPlayerControllerIterator It = World->GetPlayerControllerIterator(); It; ++It)
        {
            if (i++ == Index) return It->Get();
        }
        return nullptr;
    }

    for (FConstPlayerControllerIterator It = World->GetPlayerControllerIterator(); It; ++It)
    {
        APlayerController* PC = It->Get();
        if (!PC || !PC->PlayerState) continue;
        const FString Name = PC->PlayerState->GetPlayerName();
        if (Name.Equals(Target, ESearchCase::IgnoreCase) ||
            Name.StartsWith(Target, ESearchCase::IgnoreCase))
        {
            return PC;
        }
    }
    return nullptr;
}

void ASignetLobbyGameMode::PrintInventoryForPC(APlayerController* PC) const
{
    if (!PC) return;
    APawn* P = PC->GetPawn();
    if (!P) { UE_LOG(LogTemp, Warning, TEXT("No pawn for %s"), *PC->GetName()); return; }
    auto* Inv = P->FindComponentByClass<USignetInventoryComponent>();
    if (!Inv) { UE_LOG(LogTemp, Warning, TEXT("No inventory on %s"), *PC->GetName()); return; }

    UE_LOG(LogTemp, Log, TEXT("=== INVENTORY for %s ==="), *PC->GetName());
    for (const auto& E : Inv->GetBag().Entries)
    {
        UE_LOG(LogTemp, Log, TEXT("ItemID=%d Qty=%d Inst=%s"),
            E.ItemID, (int32)E.Quantity, *E.InstanceId.ToString());
    }
    UE_LOG(LogTemp, Log, TEXT("=== EQUIPPED for %s ==="), *PC->GetName());
    for (int32 i = 0; i < USignetInventoryComponent::SlotCount(); ++i)
    {
        const int32 ID = Inv->GetEquippedItemID(static_cast<EGearSlot>(i));
        const FGuid G = Inv->GetEquippedInstance(static_cast<EGearSlot>(i));
        UE_LOG(LogTemp, Log, TEXT("Slot %d -> ItemID=%d Inst=%s"), i, ID, *G.ToString());
    }
	
    PC->ClientMessage(TEXT("Inventory printed to server log"));
}

void ASignetLobbyGameMode::InvPrintTarget(const FString& Target)
{
    if (Target.Equals(TEXT("all"), ESearchCase::IgnoreCase))
    {
        for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
        {
            PrintInventoryForPC(It->Get());
        }
        return;
    }

    if (Target.Equals(TEXT("me"), ESearchCase::IgnoreCase))
    {
        APlayerController* Local = GetWorld()->GetFirstPlayerController();
        PrintInventoryForPC(Local);
        return;
    }

    if (APlayerController* PC = ResolveTargetPC(Target))
    {
        PrintInventoryForPC(PC);
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("InvPrintTarget: player '%s' not found"), *Target);
    }
}

void ASignetLobbyGameMode::GiveItemTarget(const FString& Target, int32 ItemID, int32 Qty)
{
	if (APlayerController* PC = ResolveTargetPC(Target))
	{
		if (APawn* P = PC->GetPawn())
		{
			if (auto* Inv = P->FindComponentByClass<USignetInventoryComponent>())
			{
				Inv->ServerTryAddItem(ItemID, Qty);
				UE_LOG(LogTemp, Log, TEXT("Gave %d of %d to %s"), Qty, ItemID, *PC->GetName());
			}
		}
	}
}

void ASignetLobbyGameMode::EquipIDTarget(const FString& Target, int32 Slot, int32 ItemID)
{
	if (APlayerController* PC = ResolveTargetPC(Target))
	{
		if (APawn* P = PC->GetPawn())
		{
			if (auto* Inv = P->FindComponentByClass<USignetInventoryComponent>())
			{
				Inv->ServerEquipItemByItemID(static_cast<EGearSlot>(Slot), ItemID);
				UE_LOG(LogTemp, Log, TEXT("EquipID %d in slot %d for %s"), ItemID, Slot, *PC->GetName());
			}
		}
	}
}

void ASignetLobbyGameMode::UnequipTarget(const FString& Target, int32 Slot)
{
	if (APlayerController* PC = ResolveTargetPC(Target))
	{
		if (APawn* P = PC->GetPawn())
		{
			if (auto* Inv = P->FindComponentByClass<USignetInventoryComponent>())
			{
				Inv->ServerUnequip(static_cast<EGearSlot>(Slot));
				UE_LOG(LogTemp, Log, TEXT("Unequip slot %d for %s"), Slot, *PC->GetName());
			}
		}
	}
}