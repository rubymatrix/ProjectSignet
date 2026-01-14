// Copyright Red Lotus Games, All Rights Reserved.


#include "SignetPlayerController.h"

#include "SignetCheats.h"
#include "SignetPlayerCharacter.h"
#include "SignetPlayerState.h"
#include "Blueprint/UserWidget.h"
#include "SignetGame/Core/SignetLobbyGameState.h"
#include "SignetGame/Inventory/SignetInventoryComponent.h"
#include "SignetGame/Save/SignetSaveSubsystem.h"
#include "SignetGame/UI/Admin/SGProfileDisplayWidget.h"
#include "SignetGame/UI/Admin/SGProfileEditorWidget.h"
#include "SignetGame/Util/Logging.h"


static FString BuildTravelURL(const FString& MapPath, const FString& Options)
{
	// MapPath examples:
	//   "/Game/Signet/Maps/L_Dungeon01"  (cooked asset path)  OR
	//   "L_Dungeon01"                    (short name in editor)
	// Options examples:
	//   "" or "Game=/Game/Signet/Core/BP_SignetGameMode.BP_SignetGameMode_C&MatchId=42"

	FString Url = MapPath;

	// If the Options string is empty, just return the map path.
	if (Options.IsEmpty())
	{
		return Url;
	}

	// If Options already contains '?' treat it as a full query, else prepend '?'.
	if (Options.Contains(TEXT("?")))
	{
		Url += Options;
	}
	else
	{
		Url += TEXT("?") + Options;
	}
	return Url;
}

ASignetPlayerController::ASignetPlayerController()
{
	static ConstructorHelpers::FClassFinder<UUserWidget> WidgetBPClass(
		TEXT("/Game/Signet/UI/Admin/WBP_SGAdmin")
	);

	if (WidgetBPClass.Succeeded())
	{
		AdminPanelWidgetClass = WidgetBPClass.Class;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to find WBP_SGProfileEditor class at the given path."));
	}

	CheatClass = USignetCheatManager::StaticClass();
}

void ASignetPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (IsLocalController())
	{
		InitializeAdminUI();
		SetInputMode(FInputModeGameOnly());
		bShowMouseCursor = false;
	}
}

void ASignetPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	ClientRequestProfileSnapshot();
}


void ASignetPlayerController::ClientRequestProfileSnapshot_Implementation()
{
	// Build from the local save subsystem
	if (auto* GI = GetGameInstance())
	{
		if (auto* Save = GI->GetSubsystem<USignetSaveSubsystem>())
		{
			const auto& P = Save->GetPlayerProfile();
			FSignetProfileSnapshot S;
			S.PlayerName = Save->GetPlayerName();
			S.Nation     = P.HomeNation;
			S.Race       = P.SelectedRace;
			S.Face       = P.SelectedFace;
			S.Job        = P.CurrentJob;
			S.Level      = Save->GetCurrentJobLevel();
			S.CurrentExp = Save->GetCurrentJobExp();
			ServerSubmitProfileSnapshot(S);
		}
	}
}

void ASignetPlayerController::ServerSubmitProfileSnapshot_Implementation(const FSignetProfileSnapshot Snapshot)
{
	auto* PS = GetPlayerState<ASignetPlayerState>();
	if (!PS) return;

	const FString SanitizedName = Snapshot.PlayerName.Left(50);

	UE_LOG(LogSignetNet, Log, TEXT("Received New Client Profile Snapshot from %s"), *SanitizedName);

	PS->PlayerName = SanitizedName; // Local
	PS->SetPlayerName(SanitizedName); // Engine-level
	PS->Nation = Snapshot.Nation;
	PS->Race = Snapshot.Race;
	PS->Face = Snapshot.Face;
	PS->Job = Snapshot.Job;
	PS->Level = Snapshot.Level;
	PS->CurrentExp = Snapshot.CurrentExp;
	PS->ReceivedPlayerProfile();

	ClientRequestInventorySnapshot();
}

void ASignetPlayerController::ClientRequestInventorySnapshot_Implementation()
{
	if (const auto* GI = GetGameInstance())
	{
		if (const auto* SaveSys = GI->GetSubsystem<USignetSaveSubsystem>())
		{
			FSignetSavedInventory InvSnapshot = SaveSys->GetSave()->Inventory;
			ServerSubmitInventorySnapshot(InvSnapshot);
		}
	}
}

void ASignetPlayerController::ServerSubmitInventorySnapshot_Implementation(const FSignetSavedInventory& Snapshot)
{
	const auto P = Cast<ASignetPlayerCharacter>(GetPawn());
	if (!P)
	{
		return;
	}
	
	if (auto* Inv = P->FindComponentByClass<USignetInventoryComponent>())
	{
		Inv->LoadFromSave(Snapshot);

		auto PS = GetPlayerState<ASignetPlayerState>();
		if (!PS) return;

		Inv->ValidateEquipment(PS->Race, PS->Job);
		P->CaptureVisualState(PS->Race, PS->Face);
	}
}

void ASignetPlayerController::InitializeVisualComponents_Implementation()
{
	const auto PS = GetPlayerState<ASignetPlayerState>();
	if (!PS) return;
	const auto P = Cast<ASignetPlayerCharacter>(GetPawn());
	if (!P) return;

	P->UpdateFace(PS->Face);
	P->UpdateRace(PS->Race);
}

void ASignetPlayerController::ServerDebugLogConnections_Implementation()
{
	if (auto* GS = GetWorld()->GetGameState<ASignetLobbyGameState>())
	{
		for (auto LobbyPlayer : GS->GetLobbyPlayers())
		{
			UE_LOG(LogSignet, Log, TEXT("Player: %s - Lv %i %s"), *LobbyPlayer->PlayerName, LobbyPlayer->Level, *UEnum::GetValueAsString(LobbyPlayer->Job));
		}
	}
}

void ASignetPlayerController::ChangeJob_Implementation(const EJob NewJob)
{
	const auto W = GetWorld();
	if (!W) return;
	const auto GI = W->GetGameInstance();
	if (!GI) return;
	const auto Save = GI->GetSubsystem<USignetSaveSubsystem>();
	if (!Save) return;

	// Apply it to our local save
	Save->SetSelectedJob(NewJob);
}

void ASignetPlayerController::DoServerTravelAll(const FName& MapAssetPath, const FString& Options)
{
	UWorld* World = GetWorld();
	if (!World)
	{
		return; // Only the server can call ServerTravel
	}

	// Optional but handy: ensure seamless travel is on (also set in GameMode/INI).
	if (AGameModeBase* GM = World->GetAuthGameMode())
	{
		GM->bUseSeamlessTravel = true;
	}

	// Build URL and go
	const FString URL = BuildTravelURL(MapAssetPath.ToString(), Options);

	// true = absolute, but also enables seamless pathing when bUseSeamlessTravel is set
	const bool bAbsolute = true;
	World->ServerTravel(URL, bAbsolute);
}

void ASignetPlayerController::InitializeAdminUI()
{
	if (AdminPanelWidgetClass)
	{
		if (AdminPanelWidget = CreateWidget<UUserWidget>(this, AdminPanelWidgetClass); AdminPanelWidget)
		{
			const auto GameInstance = GetGameInstance();
			if (!GameInstance) return;

			const auto SaveSystem = GameInstance->GetSubsystem<USignetSaveSubsystem>();
			if (!SaveSystem) return;

			ProfileEditorWidget = Cast<USGProfileEditorWidget>(AdminPanelWidget->GetWidgetFromName(FName(TEXT("ProfileEditor"))));
			ProfileDisplayWidget = Cast<USGProfileDisplayWidget>(AdminPanelWidget->GetWidgetFromName(FName(TEXT("ProfileDisplay"))));

			if (ProfileEditorWidget)
			{
				ProfileEditorWidget->InitializeFromData(SaveSystem->GetSave());
				ProfileEditorWidget->OnProfileSaved.AddDynamic(this, &ASignetPlayerController::OnProfileSaved);
				ProfileEditorWidget->OnCurrencyAdded.AddDynamic(this, &ASignetPlayerController::OnCurrencyAdded);
			}

			if (ProfileDisplayWidget)
			{
				ProfileDisplayWidget->SetDataFromSave();
			}

			AdminPanelWidget->AddToViewport();
		}
		else
		{
			UE_LOG(LogSignet, Warning, TEXT("ASignetPlayerController::BeginPlay: Failed to create ProfileEditorWidget."));
		}
	}
	else
	{
		UE_LOG(LogSignet, Warning, TEXT("ASignetPlayerController::BeginPlay: ProfileEditorWidgetClass is null."));
	}
}

void ASignetPlayerController::OnProfileSaved(const FSignetPlayerProfile& Profile)
{
	if (ProfileDisplayWidget)
	{
		ProfileDisplayWidget->SetDataFromSave();
	}
}

void ASignetPlayerController::OnCurrencyAdded(const FCurrencyAdjustment& Data)
{
	if (ProfileDisplayWidget)
	{
		ProfileDisplayWidget->SetDataFromSave();
	}
}

