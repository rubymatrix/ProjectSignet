// Copyright Red Lotus Games, All Rights Reserved.


#include "SignetPlayerState.h"
#include "SignetPlayerCharacter.h"
#include "Net/UnrealNetwork.h"
#include "SignetGame/Save/SignetSaveSubsystem.h"
#include "SignetGame/Util/Logging.h"
#include "SignetGame/Inventory/SignetInventoryComponent.h"

void ASignetPlayerState::OnRep_IsLobbyHost()
{
}

void ASignetPlayerState::OnRep_DisplayName() { DebouncePersist(); }
void ASignetPlayerState::OnRep_Nation() { DebouncePersist(); }
void ASignetPlayerState::OnRep_Race()
{
	DebouncePersist();
}
void ASignetPlayerState::OnRep_Face()
{
	DebouncePersist();
}
void ASignetPlayerState::OnRep_Job()
{
	DebouncePersist();
	if (HasAuthority()) DebounceRecalculateStats();
}
void ASignetPlayerState::OnRep_Level()
{
	DebouncePersist();
	if (HasAuthority()) DebounceRecalculateStats();
}
void ASignetPlayerState::OnRep_CurrentExp() { DebouncePersist(); }

void ASignetPlayerState::SetRace_Implementation(const ERace& NewRace)
{
	Race = NewRace;
}

void ASignetPlayerState::SetFace_Implementation(const EFace& NewFace)
{
	Face = NewFace;
}

void ASignetPlayerState::ReceivedPlayerProfile()
{
	if (HasAuthority())
	{
		DebounceUpdate();
		DebounceRecalculateStats();
	}
}

void ASignetPlayerState::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ASignetPlayerState, bIsHost);
	DOREPLIFETIME(ASignetPlayerState, PlayerName);
	DOREPLIFETIME(ASignetPlayerState, Nation);
	DOREPLIFETIME(ASignetPlayerState, Race);
	DOREPLIFETIME(ASignetPlayerState, Face);
	DOREPLIFETIME(ASignetPlayerState, Job);
	DOREPLIFETIME(ASignetPlayerState, Level);
	DOREPLIFETIME(ASignetPlayerState, CurrentExp);
}

class ASignetPlayerCharacter* ASignetPlayerState::GetSignetPawn()
{
	return Cast<ASignetPlayerCharacter>(GetPawn());
}

bool ASignetPlayerState::IsOwningClient() const
{
#if WITH_EDITOR
#endif
	if (const auto World = GetWorld())
	{
		if (const auto LocalPC = World->GetFirstPlayerController())
		{
			return GetOwner() == LocalPC;
		}
	}
	return false;
}

void ASignetPlayerState::DebounceUpdate()
{
	const auto W = GetWorld();
	if (!W) return;

	if (!W->GetTimerManager().IsTimerActive(UpdateDebounceTimer))
	{
		W->GetTimerManager().SetTimer(UpdateDebounceTimer, [this]()
		{
			OnPlayerUpdated.Broadcast(this);
		}, 0.15f, false);
	}
}

void ASignetPlayerState::DebouncePersist()
{
	DebounceUpdate(); // Broadcast that some properties were updated
	
	if (!IsOwningClient()) return;
	
	if (UWorld* W = GetWorld())
	{
		if (!W->GetTimerManager().IsTimerActive(PersistDebounceTimer))
		{
			W->GetTimerManager().SetTimer(PersistDebounceTimer, [this]()
			{
				if (auto* GI = GetGameInstance())
				{
					if (auto* Save = GI->GetSubsystem<USignetSaveSubsystem>())
					{
						// Profile block
						{
							FSignetPlayerProfile& P = Save->GetPlayerProfile();
							P.DisplayName = PlayerName;
							P.HomeNation  = Nation;
							P.SelectedRace= Race;
							P.SelectedFace= Face;
							P.CurrentJob  = Job;
						}
						// Progress block
						if (auto* JP = Save->GetOrAddJob(Job))
						{
							JP->Level      = Level;
							JP->CurrentExp = CurrentExp;
						}
						Save->MarkDirty(); // autosave will handle timing
					}
				}
			}, 0.15f, false);
		}
	}
}

void ASignetPlayerState::DebounceRecalculateStats()
{
	if (!HasAuthority()) return;
	
	const auto W = GetWorld();
	if (!W) return;

	if (!W->GetTimerManager().IsTimerActive(StatsDebounceTimer))
	{
		W->GetTimerManager().SetTimer(StatsDebounceTimer, [this]()
		{
			if (const auto P = GetSignetPawn())
			{
				P->ResetBaseStats();
			}
			else
			{
				UE_LOG(LogSignet, Error, TEXT("Unable to recalc base stats, pawn was NULL"));
			}
		}, 0.15f, false);
	}
}


void ASignetPlayerState::TriggerFaceVisualUpdate()
{
}

void ASignetPlayerState::TriggerRaceVisualUpdate()
{
}

void ASignetPlayerState::ValidateEquipment() const
{
	// TODO: Make sure equipment is valid for new class
}
