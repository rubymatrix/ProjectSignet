// Copyright Red Lotus Games, All Rights Reserved.

#pragma once
#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "SignetSaveGame.h"
#include "SignetSaveSubsystem.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnSaveLoaded);
DECLARE_MULTICAST_DELEGATE(FOnSaveWritten);

UCLASS()
class SIGNETGAME_API USignetSaveSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:

	// Event Delegates 
	FOnSaveLoaded  OnSaveLoaded;
	FOnSaveWritten OnSaveWritten;
	
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
	
	bool LoadOrCreate(const FString& InSlotName, int InUserIdx = 0);
	bool SaveNow();
	void MarkDirty();

	// Save Slot Management
	TArray<FString> ListExistingSlots() const;
	bool DeleteSlot(const FString& SlotName);
	FString GetActiveSlot() const { return ActiveSlot; }

	
	USignetSaveGame* GetSave() const { return CurrentSave; }

	FSignetPlayerProfile& GetPlayerProfile();
	FSignetCurrencyWallet& GetWallet();
	FSignetJobProgress* GetOrAddJob(EJob Job);
	int GetSkillTier(EJob Job, FName NodeId) const;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Profile")
	FString GetPlayerName();
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Profile")
	ERace GetCurrentRace();
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Profile")
	EFace GetCurrentFace();
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Profile")
	EJob GetCurrentJob();
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Profile")
	ENation GetCurrentNation();
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Profile")
	int GetCurrentJobLevel();
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Profile")
	int GetCurrentJobExp();

	// Mutators

	UFUNCTION(BlueprintCallable, Category = "Profile")
	void SetPlayerName(const FString& NewName);
	UFUNCTION(BlueprintCallable, Category = "Profile")
	void SetNation(const ENation& NewNation);
	UFUNCTION(BlueprintCallable, Category = "Profile")
	void SetSelectedJob(const EJob& NewJob);
	UFUNCTION(BlueprintCallable, Category = "Profile")
	void SetSelectedRace(const ERace& NewRace);
	UFUNCTION(BlueprintCallable, Category = "Profile")
	void SetSelectedFace(const EFace& NewFace);
	UFUNCTION(BlueprintCallable, Category = "Profile")
	void SetCurrentJobLevel(int NewLevel);
	UFUNCTION(BlueprintCallable, Category = "Profile")
	void SetCurrentJobExp(int NewExp);
	
	UFUNCTION(BlueprintCallable, Category = "Profile")
	void AddCurrency(ECurrencyType Type, int64 Delta, bool bAutosave = true);
	int UpgradeSkillTier(EJob Job, FName NodeId, int AddTiers = 1, bool bAutosave = true);
	

private:
	UPROPERTY() TObjectPtr<USignetSaveGame> CurrentSave = nullptr;
	FString ActiveSlot;
	int   UserIndex = 0;
	
	FTimerHandle AutosaveTimer;
	bool bDirty = false;
	
	// Helpers
	FString GetScopedSlotName(const FString& BaseSlot) const;
	int32   GetScopedUserIndex(int32 InUserIdx) const;
	
	bool CreateNewSave();
	void StampMeta();
	int ComputeChecksum(const USignetSaveGame* Save) const;
	bool ValidateChecksum(const USignetSaveGame* Save) const;
	void MigrateIfNeeded(USignetSaveGame* Save);

	void ScheduleAutosave(float DelaySeconds = 2.0f);
	void DoAutosave();

	// Disk ops
	static FString MakeSlotPath(const FString& SlotName);
	static TArray<FString> FindSlotsOnDisk();
};