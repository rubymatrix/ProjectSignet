// Copyright Red Lotus Games, All Rights Reserved.


#include "SignetPlayerState.h"
#include "Net/UnrealNetwork.h"
#include "SignetGame/Save/SignetSaveSubsystem.h"

void ASignetPlayerState::OnRep_IsLobbyHost()
{
}

void ASignetPlayerState::OnRep_DisplayName() { DebouncePersist(); }
void ASignetPlayerState::OnRep_Nation() { DebouncePersist(); }
void ASignetPlayerState::OnRep_Race() { DebouncePersist(); }
void ASignetPlayerState::OnRep_Face() { DebouncePersist(); }
void ASignetPlayerState::OnRep_Job() { DebouncePersist(); }
void ASignetPlayerState::OnRep_Level() { DebouncePersist(); }
void ASignetPlayerState::OnRep_CurrentExp() { DebouncePersist(); }

void ASignetPlayerState::ReceivedPlayerProfile()
{
	if (HasAuthority())
	{
		DebounceUpdate();
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
