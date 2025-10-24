// Source/SignetGame/Private/Save/SignetSaveSubsystem.cpp

#include "SignetSaveSubsystem.h"
#include "Kismet/GameplayStatics.h"
#include "Misc/DateTime.h"
#include "Misc/Paths.h"
#include "HAL/FileManager.h"
#include "SignetGame/Util/Logging.h"

static const TCHAR* kDefaultSlot = TEXT("Signet_SaveSlot_0");

void USignetSaveSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	LoadOrCreate(kDefaultSlot);
}

void USignetSaveSubsystem::Deinitialize()
{
	if (bDirty) { SaveNow(); }
	Super::Deinitialize();
}

bool USignetSaveSubsystem::LoadOrCreate(const FString& InSlotName, int InUserIdx)
{
	const FString Slot = GetScopedSlotName(InSlotName);   // stays "Signet_SaveSlot_0"
	const int32   User = GetScopedUserIndex(InUserIdx);   // becomes 0, 1, etc. in PIE

	ActiveSlot = Slot;
	UserIndex  = User;

	UE_LOG(LogSignetSave, Log, TEXT("Using slot '%s' (UserIndex=%d)"), *ActiveSlot, UserIndex);

	if (UGameplayStatics::DoesSaveGameExist(ActiveSlot, UserIndex))
	{
		if (USaveGame* Loaded = UGameplayStatics::LoadGameFromSlot(ActiveSlot, UserIndex))
		{
			CurrentSave = Cast<USignetSaveGame>(Loaded);
			if (!CurrentSave)
			{
				return CreateNewSave();
			}
			
			// Validate & migrate
			if (!ValidateChecksum(CurrentSave))
			{
				UE_LOG(LogSignetSave, Warning, TEXT("Save checksum mismatch (slot %s)"), *ActiveSlot);
			}

			// Make sure the profile has a default name value
			if (CurrentSave->Profile.DisplayName.IsEmpty())
			{
				CurrentSave->Profile.DisplayName = TEXT("Soldier");	
			}
			
			MigrateIfNeeded(CurrentSave);
			OnSaveLoaded.Broadcast();
			return true;
		}
	}

	return CreateNewSave();
}

FString USignetSaveSubsystem::GetScopedSlotName(const FString& BaseSlot) const
{
	FString CmdSuffix;
	if (FParse::Value(FCommandLine::Get(), TEXT("SignetSlotSuffix="), CmdSuffix) && !CmdSuffix.IsEmpty())
	{
		return FString::Printf(TEXT("%s_%s"), *BaseSlot, *CmdSuffix);
	}

#if WITH_EDITOR
	if (GIsEditor)
	{
		if (const UWorld* World = GetWorld())
		{
			if (const FWorldContext* Ctx = GEngine->GetWorldContextFromWorld(World))
			{
				if (Ctx->WorldType == EWorldType::PIE)
				{
					// Each PIE window gets its own slot filename
					return FString::Printf(TEXT("%s_PIE%d"), *BaseSlot, Ctx->PIEInstance);
				}
			}
		}
	}
#endif

	return BaseSlot; // normal standalone build
}

int32 USignetSaveSubsystem::GetScopedUserIndex(int32 InUserIdx) const
{
#if WITH_EDITOR
	if (GIsEditor)
	{
		if (const UWorld* World = GetWorld())
		{
			if (const FWorldContext* Ctx = GEngine->GetWorldContextFromWorld(World))
			{
				if (Ctx->WorldType == EWorldType::PIE)
				{
					return Ctx->PIEInstance;
				}
			}
		}
	}
#endif
	return InUserIdx;
}


bool USignetSaveSubsystem::CreateNewSave()
{
	CurrentSave = Cast<USignetSaveGame>(UGameplayStatics::CreateSaveGameObject(USignetSaveGame::StaticClass()));
	if (!CurrentSave) return false;

	CurrentSave->SaveVersion = USignetSaveGame::CURRENT_VERSION;
	CurrentSave->SaveGuid = FGuid::NewGuid().ToString(EGuidFormats::Digits);
	StampMeta();
	
	for (uint8 i = 0; i < static_cast<uint8>(EJob::WhiteMage) + 1; ++i)
	{
		const EJob Job = static_cast<EJob>(i);
		FSignetJobProgress& JP = CurrentSave->Jobs.FindOrAdd(Job);
		JP.JobType = Job;
		JP.Level = 1;
	}

	return SaveNow();
}

void USignetSaveSubsystem::StampMeta()
{
	const FString Now = FDateTime::UtcNow().ToIso8601();
	if (CurrentSave->CreatedAtUtc.IsEmpty())
	{
		CurrentSave->CreatedAtUtc = Now;
	}
	CurrentSave->UpdatedAtUtc = Now;
	// Fill with your changelist/version if you have one.
}

int USignetSaveSubsystem::ComputeChecksum(const USignetSaveGame* Save) const
{
	// Simple Fletcher-32 style over a subset; for demo only.
	// For real use, serialize to a temp buffer and hash.
	int sum1 = 0xffff, sum2 = 0xffff;
	auto Acc = [&](int v){ sum1 = (sum1 + v) % 0xfff1; sum2 = (sum2 + sum1) % 0xfff1; };
	Acc(Save->SaveVersion);
	Acc(Save->Wallet.AbilityPoints);
	Acc(Save->Wallet.SkillPoints);
	Acc(Save->Wallet.BeastmenSeals);
	Acc(Save->Wallet.Gil);
	return (sum2 << 16) | sum1;
}

bool USignetSaveSubsystem::ValidateChecksum(const USignetSaveGame* Save) const
{
	return Save->Checksum == 0 || Save->Checksum == ComputeChecksum(Save);
}

void USignetSaveSubsystem::MigrateIfNeeded(USignetSaveGame* Save)
{
	while (Save->SaveVersion < USignetSaveGame::CURRENT_VERSION)
	{
		switch (Save->SaveVersion)
		{
		case 1:
			// migrate 1 -> 2
			break;
		default:
			break;
		}
		Save->SaveVersion++;
	}
}

bool USignetSaveSubsystem::SaveNow()
{
	if (!CurrentSave) return false;

	StampMeta();
	CurrentSave->Checksum = ComputeChecksum(CurrentSave);

	const FString Slot = ActiveSlot.IsEmpty() ? TEXT("Signet_SaveSlot_0") : ActiveSlot;
	const int32   User = GetScopedUserIndex(UserIndex);

	const bool bOk = UGameplayStatics::SaveGameToSlot(CurrentSave, Slot, User);
	if (bOk)
	{
		bDirty = false;
		OnSaveWritten.Broadcast();
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to SaveGameToSlot: %s"), *ActiveSlot);
	}
	return bOk;
}

void USignetSaveSubsystem::MarkDirty()
{
	bDirty = true;
	ScheduleAutosave();
}

void USignetSaveSubsystem::ScheduleAutosave(float DelaySeconds)
{
	if (!GetWorld()) return;
	GetWorld()->GetTimerManager().ClearTimer(AutosaveTimer);
	GetWorld()->GetTimerManager().SetTimer(AutosaveTimer, this, &USignetSaveSubsystem::DoAutosave, DelaySeconds, false);
}

void USignetSaveSubsystem::DoAutosave()
{
	if (bDirty) { SaveNow(); }
}

FSignetPlayerProfile& USignetSaveSubsystem::GetPlayerProfile()
{
	check(CurrentSave);
	return CurrentSave->Profile;
}

FString USignetSaveSubsystem::GetPlayerName()
{
	check(CurrentSave);
	return CurrentSave->Profile.DisplayName;
}

ERace USignetSaveSubsystem::GetCurrentRace()
{
	check(CurrentSave);
	return CurrentSave->Profile.SelectedRace;
}

EFace USignetSaveSubsystem::GetCurrentFace()
{
	check(CurrentSave);
	return CurrentSave->Profile.SelectedFace;
}

EJob USignetSaveSubsystem::GetCurrentJob() 
{
	check(CurrentSave);
	return CurrentSave->Profile.CurrentJob;
}

ENation USignetSaveSubsystem::GetCurrentNation()
{
	check(CurrentSave);
	return CurrentSave->Profile.HomeNation;
}

int USignetSaveSubsystem::GetCurrentJobLevel()
{
	check(CurrentSave);

	if (const auto Prog = GetOrAddJob(CurrentSave->Profile.CurrentJob))
	{
		return Prog->Level;
	}

	return 1;
}

int USignetSaveSubsystem::GetCurrentJobExp()
{
	check(CurrentSave);

	if (const auto Prog = GetOrAddJob(CurrentSave->Profile.CurrentJob))
	{
		return Prog->CurrentExp;
	}
	
	return 0;
}

void USignetSaveSubsystem::SetNation(const ENation& NewNation)
{
	check(CurrentSave);
	CurrentSave->Profile.HomeNation = NewNation;
	MarkDirty();
}

void USignetSaveSubsystem::SetCurrentJobLevel(int NewLevel)
{
	check(CurrentSave);
	if (const auto Prog = GetOrAddJob(CurrentSave->Profile.CurrentJob))
	{
		Prog->Level = NewLevel;
		MarkDirty();
	}
}

void USignetSaveSubsystem::SetCurrentJobExp(int NewExp)
{
	check(CurrentSave);
	if (const auto Prog = GetOrAddJob(CurrentSave->Profile.CurrentJob))
	{
		Prog->CurrentExp = NewExp;
		MarkDirty();
	}
}

void USignetSaveSubsystem::SetPlayerName(const FString& NewName)
{
	check(CurrentSave);
	CurrentSave->Profile.DisplayName = NewName;
	MarkDirty();
}

void USignetSaveSubsystem::SetSelectedJob(const EJob& NewJob)
{
	check(CurrentSave);
	CurrentSave->Profile.CurrentJob = NewJob;
	MarkDirty();
}

void USignetSaveSubsystem::SetSelectedRace(const ERace& NewRace)
{
	check(CurrentSave);
	CurrentSave->Profile.SelectedRace = NewRace;
	MarkDirty();
}

void USignetSaveSubsystem::SetSelectedFace(const EFace& NewFace)
{
	CurrentSave->Profile.SelectedFace = NewFace;
	MarkDirty();
}

FSignetCurrencyWallet& USignetSaveSubsystem::GetWallet()
{
	check(CurrentSave);
	return CurrentSave->Wallet;
}

FSignetJobProgress* USignetSaveSubsystem::GetOrAddJob(EJob Job)
{
	check(CurrentSave);
	FSignetJobProgress* Found = CurrentSave->Jobs.Find(Job);
	if (!Found)
	{
		FSignetJobProgress& NewRef = CurrentSave->Jobs.Add(Job);
		NewRef.JobType = Job;
		return &NewRef;
	}
	return Found;
}

void USignetSaveSubsystem::AddCurrency(ECurrencyType Type, int64 Delta, bool bAutosave)
{
	int64& V = GetWallet().GetRef(Type);
	V = FMath::Max<int64>(0, V + Delta);
	if (bAutosave) MarkDirty();
}

int USignetSaveSubsystem::GetSkillTier(EJob Job, FName NodeId) const
{
	if (!CurrentSave) return 0;
	// if (const FSignetJobProgress* JP = CurrentSave->Jobs.Find(Job))
	// {
	// 	if (const FSkillNodeProgress* Node = JP->SkillProgress.FindByPredicate(
	// 		[&](const FSkillNodeProgress& N){ return N.NodeId == NodeId; }))
	// 	{
	// 		return Node->CurrentTier;
	// 	}
	// }
	return 0;
}

int USignetSaveSubsystem::UpgradeSkillTier(EJob Job, FName NodeId, int AddTiers, bool bAutosave)
{
	// FSignetJobProgress* JP = GetOrAddJob(Job);
	// FSkillNodeProgress* Node = JP->SkillProgress.FindByPredicate(
	// 	[&](const FSkillNodeProgress& N){ return N.NodeId == NodeId; });
	//
	// if (!Node)
	// {
	// 	FSkillNodeProgress NewNode;
	// 	NewNode.NodeId = NodeId;
	// 	NewNode.CurrentTier = FMath::Max(0, AddTiers);
	// 	JP->SkillProgress.Add(NewNode);
	// }
	// else
	// {
	// 	Node->CurrentTier = FMath::Max(0, Node->CurrentTier + AddTiers);
	// }
	//
	// if (bAutosave) MarkDirty();
	return GetSkillTier(Job, NodeId);
}

TArray<FString> USignetSaveSubsystem::ListExistingSlots() const
{
	// If you store with SaveGame system only, you can track a manifest.
	// For a simple approach, scan Saved/SaveGames for "*.sav".

	// If you want the browser to only show THIS PIE instance’s saves:
	const FString Prefix = GetScopedSlotName(TEXT("")); // e.g. "_PIE0"
	TArray<FString> All = FindSlotsOnDisk();
	All.RemoveAll([&](const FString& S){ return !S.EndsWith(Prefix) && !Prefix.IsEmpty(); });
	return All;
}

bool USignetSaveSubsystem::DeleteSlot(const FString& SlotName)
{
	if (UGameplayStatics::DoesSaveGameExist(SlotName, UserIndex))
	{
		// There is no direct delete API; remove file manually.
		const FString Path = MakeSlotPath(SlotName);
		return IFileManager::Get().Delete(*Path);
	}
	return false;
}

FString USignetSaveSubsystem::MakeSlotPath(const FString& SlotName)
{
	const FString Dir = FPaths::ProjectSavedDir() / TEXT("SaveGames");
	return Dir / (SlotName + TEXT(".sav"));
}

TArray<FString> USignetSaveSubsystem::FindSlotsOnDisk()
{
	TArray<FString> Out;
	const FString Dir = FPaths::ProjectSavedDir() / TEXT("SaveGames");
	IFileManager::Get().FindFiles(Out, *(Dir / TEXT("*.sav")), true, false);
	for (FString& Name : Out)
	{
		Name = FPaths::GetBaseFilename(Name); // strip .sav
	}
	return Out;
}
