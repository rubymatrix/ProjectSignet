// Copyright Red Lotus Games, All Rights Reserved.


#include "SignetPlayerState.h"
#include "SignetPlayerCharacter.h"
#include "Net/UnrealNetwork.h"
#include "SignetGame/Save/SignetSaveSubsystem.h"
#include "SignetGame/Util/Logging.h"
#include "SignetGame/Inventory/SignetInventoryComponent.h"

#include "SignetGame/Abilities/SignetAbilitySystemComponent.h"
#include "SignetGame/Abilities/Attributes/SignetSkillAttributeSet.h"
#include "SignetGame/Abilities/TagCache.h"
#include "SignetGame/Combat/SignetCombatStatics.h"

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
	TriggerRaceVisualUpdate();
	DebounceValidateEquipment();
}

void ASignetPlayerState::SetFace_Implementation(const EFace& NewFace)
{
	Face = NewFace;
	TriggerFaceVisualUpdate();
}

void ASignetPlayerState::SetJob_Implementation(const EJob& NewJob)
{
	Job = NewJob;
	DebounceRecalculateStats();
	DebounceValidateEquipment();
}

void ASignetPlayerState::ReceivedPlayerProfile()
{
	if (HasAuthority())
	{
		DebounceUpdate();
		DebounceRecalculateStats();
		InitializeSkillsFromSave();
	}
}

void ASignetPlayerState::InitializeSkillsFromSave()
{
	if (!HasAuthority()) return;

	const auto* GI = GetGameInstance();
	if (!GI) return;

	auto* SaveSubsystem = GI->GetSubsystem<USignetSaveSubsystem>();
	if (!SaveSubsystem) return;

	const auto* Save = SaveSubsystem->GetSave();
	if (!Save) return;

	const auto* Pawn = Cast<ASignetPlayerCharacter>(GetPawn());
	if (!Pawn) return;

	auto* Asc = Pawn->GetSignetAsc();
	if (!Asc) return;

	const auto* SkillSet = Asc->GetSet<USignetSkillAttributeSet>();
	if (!SkillSet) return;

	const auto& SkillTags = FTagCache::Get().Skill;

	// Helper to map Tag to Attribute
	auto GetSkillAttr = [&](const FGameplayTag& Tag) -> FGameplayAttribute
	{
		if (Tag == SkillTags.Sword) return USignetSkillAttributeSet::GetSwordAttribute();
		if (Tag == SkillTags.GreatSword) return USignetSkillAttributeSet::GetGreatSwordAttribute();
		if (Tag == SkillTags.Axe) return USignetSkillAttributeSet::GetAxeAttribute();
		if (Tag == SkillTags.GreatAxe) return USignetSkillAttributeSet::GetGreatAxeAttribute();
		if (Tag == SkillTags.Dagger) return USignetSkillAttributeSet::GetDaggerAttribute();
		if (Tag == SkillTags.Club) return USignetSkillAttributeSet::GetClubAttribute();
		if (Tag == SkillTags.Staff) return USignetSkillAttributeSet::GetStaffAttribute();
		if (Tag == SkillTags.Polearm) return USignetSkillAttributeSet::GetPolearmAttribute();
		if (Tag == SkillTags.Scythe) return USignetSkillAttributeSet::GetScytheAttribute();
		if (Tag == SkillTags.Katana) return USignetSkillAttributeSet::GetKatanaAttribute();
		if (Tag == SkillTags.GreatKatana) return USignetSkillAttributeSet::GetGreatKatanaAttribute();
		if (Tag == SkillTags.H2H) return USignetSkillAttributeSet::GetHandToHandAttribute();
		if (Tag == SkillTags.Archery) return USignetSkillAttributeSet::GetArcheryAttribute();
		if (Tag == SkillTags.Marksmanship) return USignetSkillAttributeSet::GetMarksmanshipAttribute();
		if (Tag == SkillTags.Throwing) return USignetSkillAttributeSet::GetThrowingAttribute();
		if (Tag == SkillTags.Shield) return USignetSkillAttributeSet::GetShieldAttribute();
		if (Tag == SkillTags.Singing) return USignetSkillAttributeSet::GetSingingAttribute();
		if (Tag == SkillTags.StringInstrument) return USignetSkillAttributeSet::GetStringInstrumentAttribute();
		if (Tag == SkillTags.WindInstrument) return USignetSkillAttributeSet::GetWindInstrumentAttribute();
		if (Tag == FGameplayTag::RequestGameplayTag(TEXT("Data.Skill.Evasion"))) return USignetSkillAttributeSet::GetEvasionAttribute();
		if (Tag == FGameplayTag::RequestGameplayTag(TEXT("Data.Skill.Parrying"))) return USignetSkillAttributeSet::GetParryingAttribute();
		if (Tag == FGameplayTag::RequestGameplayTag(TEXT("Data.Skill.Guard"))) return USignetSkillAttributeSet::GetGuardAttribute();
		if (Tag == FGameplayTag::RequestGameplayTag(TEXT("Data.Skill.Magic.Blue"))) return USignetSkillAttributeSet::GetBlueAttribute();
		if (Tag == FGameplayTag::RequestGameplayTag(TEXT("Data.Skill.Magic.Dark"))) return USignetSkillAttributeSet::GetDarkAttribute();
		if (Tag == FGameplayTag::RequestGameplayTag(TEXT("Data.Skill.Magic.Divine"))) return USignetSkillAttributeSet::GetDivineAttribute();
		if (Tag == FGameplayTag::RequestGameplayTag(TEXT("Data.Skill.Magic.Elemental"))) return USignetSkillAttributeSet::GetElementalAttribute();
		if (Tag == FGameplayTag::RequestGameplayTag(TEXT("Data.Skill.Magic.Enfeebling"))) return USignetSkillAttributeSet::GetEnfeeblingAttribute();
		if (Tag == FGameplayTag::RequestGameplayTag(TEXT("Data.Skill.Magic.Enhancing"))) return USignetSkillAttributeSet::GetEnhancingAttribute();
		if (Tag == FGameplayTag::RequestGameplayTag(TEXT("Data.Skill.Magic.Geomancy"))) return USignetSkillAttributeSet::GetGeomancyAttribute();
		if (Tag == FGameplayTag::RequestGameplayTag(TEXT("Data.Skill.Magic.Handbell"))) return USignetSkillAttributeSet::GetHandbellAttribute();
		if (Tag == FGameplayTag::RequestGameplayTag(TEXT("Data.Skill.Magic.Healing"))) return USignetSkillAttributeSet::GetHealingAttribute();
		if (Tag == FGameplayTag::RequestGameplayTag(TEXT("Data.Skill.Magic.Summoning"))) return USignetSkillAttributeSet::GetSummoningAttribute();
		if (Tag == FGameplayTag::RequestGameplayTag(TEXT("Data.Skill.Magic.Ninjutsu"))) return USignetSkillAttributeSet::GetNinjutsuAttribute();
		if (Tag == FGameplayTag::RequestGameplayTag(TEXT("Data.Skill.Magic.Singing"))) return USignetSkillAttributeSet::GetSingingAttribute();
		if (Tag == FGameplayTag::RequestGameplayTag(TEXT("Data.Skill.Magic.StringInstrument"))) return USignetSkillAttributeSet::GetStringInstrumentAttribute();
		if (Tag == FGameplayTag::RequestGameplayTag(TEXT("Data.Skill.Magic.WindInstrument"))) return USignetSkillAttributeSet::GetWindInstrumentAttribute();
		return FGameplayAttribute();
	};

	// For each skill in the save, set the base value in the AttributeSet
	for (const auto& Kvp : Save->Skills)
	{
		FGameplayAttribute SkillAttr = GetSkillAttr(Kvp.Key);
		if (SkillAttr.IsValid())
		{
			Asc->SetNumericAttributeBase(SkillAttr, Kvp.Value.Rank);
		}
	}

	// Bind delegates for all skill attributes to save changes
	TArray<FGameplayTag> AllSkillTags = {
		SkillTags.Sword, SkillTags.GreatSword, SkillTags.Axe, SkillTags.GreatAxe,
		SkillTags.Dagger, SkillTags.Club, SkillTags.Staff, SkillTags.Polearm,
		SkillTags.Scythe, SkillTags.Katana, SkillTags.GreatKatana, SkillTags.H2H,
		SkillTags.Archery, SkillTags.Marksmanship, SkillTags.Throwing, SkillTags.Shield,
		SkillTags.Singing, SkillTags.StringInstrument, SkillTags.WindInstrument,
		FGameplayTag::RequestGameplayTag(TEXT("Data.Skill.Evasion")),
		FGameplayTag::RequestGameplayTag(TEXT("Data.Skill.Parrying")),
		FGameplayTag::RequestGameplayTag(TEXT("Data.Skill.Guard")),
		FGameplayTag::RequestGameplayTag(TEXT("Data.Skill.Magic.Blue")),
		FGameplayTag::RequestGameplayTag(TEXT("Data.Skill.Magic.Dark")),
		FGameplayTag::RequestGameplayTag(TEXT("Data.Skill.Magic.Divine")),
		FGameplayTag::RequestGameplayTag(TEXT("Data.Skill.Magic.Elemental")),
		FGameplayTag::RequestGameplayTag(TEXT("Data.Skill.Magic.Enfeebling")),
		FGameplayTag::RequestGameplayTag(TEXT("Data.Skill.Magic.Enhancing")),
		FGameplayTag::RequestGameplayTag(TEXT("Data.Skill.Magic.Geomancy")),
		FGameplayTag::RequestGameplayTag(TEXT("Data.Skill.Magic.Handbell")),
		FGameplayTag::RequestGameplayTag(TEXT("Data.Skill.Magic.Healing")),
		FGameplayTag::RequestGameplayTag(TEXT("Data.Skill.Magic.Summoning")),
		FGameplayTag::RequestGameplayTag(TEXT("Data.Skill.Magic.Ninjutsu")),
		FGameplayTag::RequestGameplayTag(TEXT("Data.Skill.Magic.Singing")),
		FGameplayTag::RequestGameplayTag(TEXT("Data.Skill.Magic.StringInstrument")),
		FGameplayTag::RequestGameplayTag(TEXT("Data.Skill.Magic.WindInstrument"))
	};

	for (const FGameplayTag& Tag : AllSkillTags)
	{
		FGameplayAttribute Attr = GetSkillAttr(Tag);
		if (Attr.IsValid())
		{
			Asc->GetGameplayAttributeValueChangeDelegate(Attr).AddUObject(this, &ASignetPlayerState::OnSkillAttributeChanged);
		}
	}
}

void ASignetPlayerState::OnSkillAttributeChanged(const FOnAttributeChangeData& Data)
{
	if (!HasAuthority()) return;

	const auto* GI = GetGameInstance();
	if (!GI) return;

	auto* SaveSubsystem = GI->GetSubsystem<USignetSaveSubsystem>();
	if (!SaveSubsystem) return;

	// We need to find the Tag for this Attribute
	const auto& SkillTags = FTagCache::Get().Skill;
	FGameplayTag SkillTag;

	if (Data.Attribute == USignetSkillAttributeSet::GetSwordAttribute()) SkillTag = SkillTags.Sword;
	else if (Data.Attribute == USignetSkillAttributeSet::GetGreatSwordAttribute()) SkillTag = SkillTags.GreatSword;
	else if (Data.Attribute == USignetSkillAttributeSet::GetAxeAttribute()) SkillTag = SkillTags.Axe;
	else if (Data.Attribute == USignetSkillAttributeSet::GetGreatAxeAttribute()) SkillTag = SkillTags.GreatAxe;
	else if (Data.Attribute == USignetSkillAttributeSet::GetDaggerAttribute()) SkillTag = SkillTags.Dagger;
	else if (Data.Attribute == USignetSkillAttributeSet::GetClubAttribute()) SkillTag = SkillTags.Club;
	else if (Data.Attribute == USignetSkillAttributeSet::GetStaffAttribute()) SkillTag = SkillTags.Staff;
	else if (Data.Attribute == USignetSkillAttributeSet::GetPolearmAttribute()) SkillTag = SkillTags.Polearm;
	else if (Data.Attribute == USignetSkillAttributeSet::GetScytheAttribute()) SkillTag = SkillTags.Scythe;
	else if (Data.Attribute == USignetSkillAttributeSet::GetKatanaAttribute()) SkillTag = SkillTags.Katana;
	else if (Data.Attribute == USignetSkillAttributeSet::GetGreatKatanaAttribute()) SkillTag = SkillTags.GreatKatana;
	else if (Data.Attribute == USignetSkillAttributeSet::GetHandToHandAttribute()) SkillTag = SkillTags.H2H;
	else if (Data.Attribute == USignetSkillAttributeSet::GetArcheryAttribute()) SkillTag = SkillTags.Archery;
	else if (Data.Attribute == USignetSkillAttributeSet::GetMarksmanshipAttribute()) SkillTag = SkillTags.Marksmanship;
	else if (Data.Attribute == USignetSkillAttributeSet::GetThrowingAttribute()) SkillTag = SkillTags.Throwing;
	else if (Data.Attribute == USignetSkillAttributeSet::GetShieldAttribute()) SkillTag = SkillTags.Shield;
	else if (Data.Attribute == USignetSkillAttributeSet::GetSingingAttribute()) SkillTag = SkillTags.Singing;
	else if (Data.Attribute == USignetSkillAttributeSet::GetStringInstrumentAttribute()) SkillTag = SkillTags.StringInstrument;
	else if (Data.Attribute == USignetSkillAttributeSet::GetWindInstrumentAttribute()) SkillTag = SkillTags.WindInstrument;
	else if (Data.Attribute == USignetSkillAttributeSet::GetEvasionAttribute()) SkillTag = FGameplayTag::RequestGameplayTag(TEXT("Data.Skill.Evasion"));
	else if (Data.Attribute == USignetSkillAttributeSet::GetParryingAttribute()) SkillTag = FGameplayTag::RequestGameplayTag(TEXT("Data.Skill.Parrying"));
	else if (Data.Attribute == USignetSkillAttributeSet::GetGuardAttribute()) SkillTag = FGameplayTag::RequestGameplayTag(TEXT("Data.Skill.Guard"));
	else if (Data.Attribute == USignetSkillAttributeSet::GetBlueAttribute()) SkillTag = FGameplayTag::RequestGameplayTag(TEXT("Data.Skill.Magic.Blue"));
	else if (Data.Attribute == USignetSkillAttributeSet::GetDarkAttribute()) SkillTag = FGameplayTag::RequestGameplayTag(TEXT("Data.Skill.Magic.Dark"));
	else if (Data.Attribute == USignetSkillAttributeSet::GetDivineAttribute()) SkillTag = FGameplayTag::RequestGameplayTag(TEXT("Data.Skill.Magic.Divine"));
	else if (Data.Attribute == USignetSkillAttributeSet::GetElementalAttribute()) SkillTag = FGameplayTag::RequestGameplayTag(TEXT("Data.Skill.Magic.Elemental"));
	else if (Data.Attribute == USignetSkillAttributeSet::GetEnfeeblingAttribute()) SkillTag = FGameplayTag::RequestGameplayTag(TEXT("Data.Skill.Magic.Enfeebling"));
	else if (Data.Attribute == USignetSkillAttributeSet::GetEnhancingAttribute()) SkillTag = FGameplayTag::RequestGameplayTag(TEXT("Data.Skill.Magic.Enhancing"));
	else if (Data.Attribute == USignetSkillAttributeSet::GetGeomancyAttribute()) SkillTag = FGameplayTag::RequestGameplayTag(TEXT("Data.Skill.Magic.Geomancy"));
	else if (Data.Attribute == USignetSkillAttributeSet::GetHandbellAttribute()) SkillTag = FGameplayTag::RequestGameplayTag(TEXT("Data.Skill.Magic.Handbell"));
	else if (Data.Attribute == USignetSkillAttributeSet::GetHealingAttribute()) SkillTag = FGameplayTag::RequestGameplayTag(TEXT("Data.Skill.Magic.Healing"));
	else if (Data.Attribute == USignetSkillAttributeSet::GetSummoningAttribute()) SkillTag = FGameplayTag::RequestGameplayTag(TEXT("Data.Skill.Magic.Summoning"));
	else if (Data.Attribute == USignetSkillAttributeSet::GetNinjutsuAttribute()) SkillTag = FGameplayTag::RequestGameplayTag(TEXT("Data.Skill.Magic.Ninjutsu"));
	else if (Data.Attribute == USignetSkillAttributeSet::GetSingingAttribute()) SkillTag = FGameplayTag::RequestGameplayTag(TEXT("Data.Skill.Magic.Singing"));
	else if (Data.Attribute == USignetSkillAttributeSet::GetStringInstrumentAttribute()) SkillTag = FGameplayTag::RequestGameplayTag(TEXT("Data.Skill.Magic.StringInstrument"));
	else if (Data.Attribute == USignetSkillAttributeSet::GetWindInstrumentAttribute()) SkillTag = FGameplayTag::RequestGameplayTag(TEXT("Data.Skill.Magic.WindInstrument"));

	if (SkillTag.IsValid())
	{
		SaveSubsystem->SetSkillRank(SkillTag, Data.NewValue);
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
	if (!IsValid(GetPawn())) return nullptr;
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

void ASignetPlayerState::DebounceValidateEquipment()
{
	if (!HasAuthority()) return;
	
	const auto W = GetWorld();
	if (!W) return;

	if (!W->GetTimerManager().IsTimerActive(ValidateEquipmentDebounceTimer))
	{
		W->GetTimerManager().SetTimer(ValidateEquipmentDebounceTimer, [this]()
		{
			ValidateEquipment();
		}, 0.15f, false);
	}
}


void ASignetPlayerState::TriggerFaceVisualUpdate()
{
	const auto W = GetWorld();
	if (!W) return;
	
	const auto P = Cast<ASignetPlayerCharacter>(GetPawn());
	if (!P) return;
	
	P->CaptureVisualState(Race, Face);
}

void ASignetPlayerState::TriggerRaceVisualUpdate()
{
	const auto W = GetWorld();
	if (!W) return;
	
	const auto P = Cast<ASignetPlayerCharacter>(GetPawn());
	if (!P) return;

	P->CaptureVisualState(Race, Face);
}

void ASignetPlayerState::ValidateEquipment()
{
	// TODO: Make sure equipment is valid for new class

	if (const auto P = GetSignetPawn())
	{
		if (const auto Inv = P->GetInventoryComponent())
		{
			Inv->ValidateEquipment(Race, Job);
		}
	}
}
