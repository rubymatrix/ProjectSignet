// Copyright Red Lotus Games, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AlsCharacter.h"
#include "InputActionValue.h"
#include "VisualState.h"
#include "GameplayEffectTypes.h"
#include "SignetGame/Combat/CombatInterface.h"
#include "SignetGame/Combat/CombatTypes.h"
#include "SignetGame/Data/CharacterParts.h"
#include "SignetGame/Inventory/InventoryTypes.h"
#include "SignetPlayerCharacter.generated.h"

class UCameraShakeComponent;
class UCharacterAudioComponent;
class UCharacterDataComponent;
class UCombatTextComponent;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnVisualStateUpdated, const FVisualState&, InVisualState);

class USignetVisualComponent;
class USignetAbilitySystemComponent;
class USignetInventoryComponent;
class UInventoryComponent;
class UTargetingComponent;
class UStatsComponent;
class USignetCameraComponent;
class UInputAction;
class UInputMappingContext;
class UGameDataSubsystem;

enum class EGearSlot : uint8;
struct FInputActionValue;
struct FInventoryItem;
struct FCharacterPartsRow;

UCLASS()
class SIGNETGAME_API ASignetPlayerCharacter : public AAlsCharacter, public ICombatInterface
{
	GENERATED_BODY()

// Begin Visual Mesh Systems
public:

	UPROPERTY(BlueprintAssignable)
	FOnVisualStateUpdated VisualStateUpdated;
	
protected:
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, ReplicatedUsing = OnRep_VisualState)
	FVisualState VisualState;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta=(HideInDetailPanel=true), Category="Components")
	TObjectPtr<USkeletalMeshComponent> MainMesh;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta=(HideInDetailPanel=true), Category="Components")
	TObjectPtr<USkeletalMeshComponent> SubMesh;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta=(HideInDetailPanel=true), Category="Components")
	TObjectPtr<USkeletalMeshComponent> RangedMesh;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta=(HideInDetailPanel=true), Category="Components")
	TObjectPtr<USkeletalMeshComponent> FaceMesh;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta=(HideInDetailPanel=true), Category="Components")
	TObjectPtr<USkeletalMeshComponent> HeadMesh;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta=(HideInDetailPanel=true), Category="Components")
	TObjectPtr<USkeletalMeshComponent> BodyMesh;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta=(HideInDetailPanel=true), Category="Components")
	TObjectPtr<USkeletalMeshComponent> HandsMesh;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta=(HideInDetailPanel=true), Category="Components")
	TObjectPtr<USkeletalMeshComponent> LegsMesh;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta=(HideInDetailPanel=true), Category="Components")
	TObjectPtr<USkeletalMeshComponent> FeetMesh;

// Begin Camera System

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta=(HideInDetailPanel=true), Category="Components")
	TObjectPtr<USignetCameraComponent> Camera;

// Begin Components

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta=(HideInDetailPanel=true), Category="Components")
	TObjectPtr<UStatsComponent> Stats;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta=(HideInDetailPanel=true), Category="Components")
	TObjectPtr<UTargetingComponent> Targeting;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta=(HideInDetailPanel=true), Category="Components")
	TObjectPtr<USignetInventoryComponent> InventoryComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta=(HideInDetailPanel=true), Category="Components")
	TObjectPtr<UCharacterDataComponent> CharacterData;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta=(HideInDetailPanel=true), Category="Components")
	TObjectPtr<UCharacterAudioComponent> CharacterAudio;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta=(HideInDetailPanel=true), Category="Components")
	TObjectPtr<UCameraShakeComponent> CameraShakeComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta=(HideInDetailPanel=true), Category="Components")
	TObjectPtr<UCombatTextComponent> CombatTextComp;

public:

	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool IsAscValid() const;

	FORCEINLINE USignetAbilitySystemComponent* GetSignetAsc() const;

	UFUNCTION(BlueprintCallable, BlueprintPure)
	FORCEINLINE UStatsComponent* GetStatsComponent() const { return Stats; }
	
	UFUNCTION(BlueprintCallable, Category="Movement")
	float GetMoveSpeedMultiplier() const;

	UFUNCTION(BlueprintCallable, BlueprintPure)
	FORCEINLINE UTargetingComponent* GetTargetingComponent() const { return Targeting; }
	
	UFUNCTION(BlueprintCallable, BlueprintPure)
	FORCEINLINE USignetInventoryComponent* GetInventoryComponent() const { return InventoryComponent; }

	UFUNCTION(BlueprintCallable, BlueprintPure)
	FORCEINLINE UCharacterAudioComponent* GetCharacterAudioComponent() const { return CharacterAudio; }

	USkeletalMeshComponent* GetFaceComponent();
	USkeletalMeshComponent* GetMeshComponent(const EGearSlot& GearSlot);

protected:

// Begin Player Input Schema
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(DisplayThumbnail=false), Category="Settings|Input")
	TObjectPtr<UInputMappingContext> DefaultMappingContext;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(DisplayThumbnail=false), Category="Settings|Input|Bindings")
	TObjectPtr<UInputAction> LookMouseAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(DisplayThumbnail=false), Category="Settings|Input|Bindings")
	TObjectPtr<UInputAction> LookAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(DisplayThumbnail=false), Category="Settings|Input|Bindings")
	TObjectPtr<UInputAction> MoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(DisplayThumbnail=false), Category="Settings|Input|Bindings")
	TObjectPtr<UInputAction> ToggleRunWalkAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(DisplayThumbnail=false), Category="Settings|Input|Bindings")
	TObjectPtr<UInputAction> JumpAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(DisplayThumbnail=false), Category="Settings|Input|Bindings")
	TObjectPtr<UInputAction> SitAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(DisplayThumbnail=false), Category="Settings|Input|Bindings")// Doubles as the Target Lock action 
	TObjectPtr<UInputAction> HealAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(DisplayThumbnail=false), Category="Settings|Input|Bindings")
	TObjectPtr<UInputAction> EmoteAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(DisplayThumbnail=false), Category="Settings|Input|Bindings")
	TObjectPtr<UInputAction> AcceptAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(DisplayThumbnail=false), Category="Settings|Input|Bindings")
	TObjectPtr<UInputAction> CancelAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(DisplayThumbnail=false), Category="Settings|Input|Bindings")
	TObjectPtr<UInputAction> MenuAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(DisplayThumbnail=false), Category="Settings|Input|Bindings")
	TObjectPtr<UInputAction> TabTargetAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(DisplayThumbnail=false), Category="Settings|Input|Bindings")
	TObjectPtr<UInputAction> ZoomAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(DisplayThumbnail=false), Category="Settings|Input|Bindings")
	TObjectPtr<UInputAction> NextTargetAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(DisplayThumbnail=false), Category="Settings|Input|Options")
	float LookUpMouseSensitivity{-1.0f};

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(DisplayThumbnail=false), Category="Settings|Input|Options")
	float LookRightMouseSensitivity{1.0f};

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(DisplayThumbnail=false), Category="Settings|Input|Options")
	float LookUpRate{90.f};

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(DisplayThumbnail=false), Category="Settings|Input|Options")
	float LookRightRate{90.f};

	/** Max turn speed when locked on in degrees per second. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings|Targeting")
	float MaxTurnRateDegPerSec = 540.f;

	/** Movement speed scalar when locked on. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings|Targeting")
	float LockedMoveSpeedScalar = 0.9f;

	/** Radial (forward/back) movement speed scalar when locked on. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings|Targeting")
	float LockedRadialSpeedScalar = 0.8f;

// Begin Stats System

	UPROPERTY()
	FActiveGameplayEffectHandle BaseStatsEffectHandle;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Settings|Gameplay")
	TSubclassOf<UGameplayEffect> BaseStatsEffectClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Settings|Gameplay")
	TSubclassOf<UGameplayEffect> HealEffectClass;

// Cheats


	
public:
	
	ASignetPlayerCharacter(const FObjectInitializer& ObjectInitializer);

	virtual void NotifyControllerChanged() override;

	UFUNCTION(Server, Reliable)
	void ResetBaseStats();

	UFUNCTION(Server, Reliable)
	void Heal();

protected:
	
	virtual void BeginPlay() override;
	
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

	virtual void CalcCamera(float DeltaTime, struct FMinimalViewInfo& OutResult) override;

	virtual void PostInitializeComponents() override;

	virtual void Tick(float DeltaSeconds) override;

	
// Begin Input Action Handlers
	
	virtual void Input_OnLookMouse(const FInputActionValue& ActionValue);
	virtual void Input_OnLook(const FInputActionValue& ActionValue);
	virtual void Input_OnMove(const FInputActionValue& ActionValue);
	virtual void Input_OnToggleRunWalk(const FInputActionValue& ActionValue);
	virtual void Input_OnJump(const FInputActionValue& ActionValue);
	virtual void Input_OnSit(const FInputActionValue& ActionValue);
	virtual void Input_OnHeal(const FInputActionValue& ActionValue);
	virtual void Input_OnEmotePressed(const FInputActionValue& ActionValue);
	virtual void Input_OnEmoteReleased(const FInputActionValue& ActionValue);
	virtual void Input_OnAccept(const FInputActionValue& ActionValue);
	virtual void Input_OnCancel(const FInputActionValue& ActionValue);
	virtual void Input_OnMenu(const FInputActionValue& ActionValue);
	virtual void Input_OnTabTarget(const FInputActionValue& ActionValue);
	virtual void Input_OnZoom(const FInputActionValue& ActionValue);
	virtual void Input_OnNextTarget(const FInputActionValue& ActionValue);


// Begin Replication Functions

	UFUNCTION()
	void OnRep_VisualState();

public:

	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	

// Begin Debug Display
	
	virtual void DisplayDebug(UCanvas* Canvas, const FDebugDisplayInfo& DisplayInfo, float& Unused, float& VerticalLocation) override;


// Begin Mesh Manipulation
private:

	FTimerHandle VisualUpdateDebounceTimer;
	
	ERace CurrentRace = ERace::HumeMale;
	EFace CurrentFace = EFace::Invalid;
	int32 CurrentHideParts = 0;

	// Helper Accessors
	TArray<USkeletalMeshComponent*> GetComponentsToHide();
	UGameDataSubsystem* GetGameData();
	const FInventoryItem* FindItemDef(const int ItemID);
	const FCharacterPartsRow* GetCharacterParts();

public:
	
	// Component manipulation
	UFUNCTION(Exec)
	void RefreshAllSlots();
	void ApplyBaseCharacter();
	void ApplyEquipmentSlot(const EGearSlot InGearSlot);

	UFUNCTION(BlueprintCallable)
	void UpdateRace(const ERace NewRace);
	UFUNCTION(BlueprintCallable)
	void UpdateFace(const EFace NewFace);

	UFUNCTION(BlueprintCallable)
	void PerformAttack();

	void CaptureVisualState(const ERace NewRace, const EFace NewFace);

	UFUNCTION(BlueprintCallable, BlueprintPure)
	FORCEINLINE ERace GetCurrentRace() const { return VisualState.Race; }
	
	UFUNCTION(BlueprintCallable, BlueprintPure)
	FORCEINLINE EFace GetCurrentFace() const { return VisualState.Face; }


	//~ Begin ICombatInterface
	virtual FString GetEntityName() override;
	virtual float GetHealth() override;
	virtual float GetMaxHealth() override;
	virtual float GetPower() override;
	virtual float GetMaxPower() override;
	virtual float GetTP() override;
	virtual void PlayHitReaction() override;
	virtual void HitFlash() override;
	virtual void PlayVocalization(const EVocalizationType VoiceType) override;
	virtual void PlayWeaponSwingAtLocation(const FVector& Location) override;
	virtual void PlayImpactSoundAtLocation(const FVector& Location) override;

	virtual void PlayWeaponWhoosh() override;
	virtual void PlayWeaponImpact() override;

	virtual void PushAttackResults(const TArray<FAttackResult>& Results) override;
	virtual void AdvanceAttackResult() override;
	virtual bool GetCurrentAttackResult(FAttackResult& OutResult) override;
	virtual void ClearAttackResults() override;
	virtual void DoAttackSequence() override;
	virtual void CameraShake(const ECameraShakeType& Shake, float Intensity = 1) override;
	//~ End ICombatInterface
	
	UPROPERTY(Transient)
	TArray<FAttackResult> AttackResultQueue;

	UPROPERTY(Transient)
	int32 CurrentResultIndex = -1;

private:
	
	void ApplyDefaultMesh(const EGearSlot InGearSlot, const FCharacterPartsRow* Parts);
	void ApplyFace(const FCharacterPartsRow* Parts, const EFaceClipStage DesiredClipStage = EFaceClipStage::None);
	void ApplyMeshHiding();

	void ApplyItemStatModifiers(const FInventoryItem& ItemDef, const FGuid& InstanceId);
	void RemoveItemStatModifiers(const FGuid& InstanceId);

	UFUNCTION()
	void OnEquipmentChanged(EGearSlot InGearSlot, int32 OldItemID, int32 NewItemID);

	UFUNCTION()
	void OnEquipmentChangedInstance(EGearSlot InGearSlot, FGuid OldInstance, FGuid NewInstance);

	UPROPERTY(Transient)
	TMap<FGuid, FActiveGameplayEffectHandle> ItemStatEffectHandles;
};

