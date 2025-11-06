// Copyright Red Lotus Games, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "ModularGameplayActors/GSCModularPlayerState.h"
#include "SignetGame/Save/SignetSaveTypes.h"
#include "SignetPlayerState.generated.h"

class USignetInventoryComponent;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPlayerUpdatedDelegate, ASignetPlayerState*, Player);

class UVisualComponent;

/**
 * 
 */
UCLASS()
class SIGNETGAME_API ASignetPlayerState : public AGSCModularPlayerState
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintAssignable)
	FOnPlayerUpdatedDelegate OnPlayerUpdated;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Profile", meta=(DisplayPriority=0), ReplicatedUsing = OnRep_IsLobbyHost)
	bool bIsHost = false;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Profile", meta=(DisplayPriority=0), ReplicatedUsing = OnRep_DisplayName)
	FString PlayerName;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Profile", meta=(DisplayPriority=0), ReplicatedUsing = OnRep_Nation)
	ENation Nation = ENation::Bastok;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Profile", meta=(DisplayPriority=0), ReplicatedUsing = OnRep_Race)
	ERace Race = ERace::HumeMale;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Profile", meta=(DisplayPriority=0), ReplicatedUsing = OnRep_Face)
	EFace Face = EFace::F1A;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Profile", meta=(DisplayPriority=0), ReplicatedUsing = OnRep_Job)
	EJob Job = EJob::Warrior;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Profile", meta=(DisplayPriority=0), ReplicatedUsing = OnRep_Level)
	int Level = 1;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Profile", meta=(DisplayPriority=0), ReplicatedUsing = OnRep_CurrentExp)
	int CurrentExp;

protected:

	UFUNCTION() void OnRep_IsLobbyHost();
	UFUNCTION() void OnRep_DisplayName();
	UFUNCTION() void OnRep_Nation();
	UFUNCTION() void OnRep_Race();
	UFUNCTION() void OnRep_Face();
	UFUNCTION() void OnRep_Job();
	UFUNCTION() void OnRep_Level();
	UFUNCTION() void OnRep_CurrentExp();


public:

	UFUNCTION(BlueprintCallable, Server, Reliable)
	void SetRace(const ERace& NewRace);
	
	UFUNCTION(BlueprintCallable, Server, Reliable)
	void SetFace(const EFace& NewFace);
	
	UFUNCTION(BlueprintCallable, Server, Reliable)
	void SetJob(const EJob& NewJob);

	void ReceivedPlayerProfile();

	UFUNCTION(BlueprintCallable, BlueprintPure)
	FORCEINLINE bool IsLobbyHost() const { return bIsHost; }

	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;


private:

	class ASignetPlayerCharacter* GetSignetPawn();
	
	bool IsOwningClient() const;
	FTimerHandle PersistDebounceTimer;
	FTimerHandle UpdateDebounceTimer;
	FTimerHandle StatsDebounceTimer;
	void DebounceUpdate();
	void DebouncePersist();
	void DebounceRecalculateStats();

	FTimerHandle FaceUpdateTimer;
	FTimerHandle RaceUpdateTimer;
	void TriggerFaceVisualUpdate();
	void TriggerRaceVisualUpdate();
	void ValidateEquipment();
};
