// Copyright Red Lotus Games, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AlsCharacter.h"
#include "InputActionValue.h"
#include "VisualState.h"
#include "SignetPlayerCharacter.generated.h"


class USignetAbilitySystemComponent;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnVisualStateUpdated, const FVisualState&, InVisualState);

class UInventoryComponent;
class UTargetingComponent;
class UStatsComponent;
class USignetCameraComponent;
class UInputAction;
class UInputMappingContext;

struct FInputActionValue;

UCLASS()
class SIGNETGAME_API ASignetPlayerCharacter : public AAlsCharacter
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
	TObjectPtr<UInventoryComponent> Inventory;

public:

	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool IsAscValid() const;

	FORCEINLINE USignetAbilitySystemComponent* GetSignetAsc() const;

	UFUNCTION(BlueprintCallable, BlueprintPure)
	FORCEINLINE UStatsComponent* GetStatsComponent() const { return Stats; }
	
	UFUNCTION(BlueprintCallable, BlueprintPure)
	FORCEINLINE UTargetingComponent* GetTargetingComponent() const { return Targeting; }
	
	UFUNCTION(BlueprintCallable, BlueprintPure)
	FORCEINLINE UInventoryComponent* GetInventoryComponent() const { return Inventory; }

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

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(DisplayThumbnail=false), Category="Settings|Input|Options")
	float LookUpMouseSensitivity{-1.0f};

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(DisplayThumbnail=false), Category="Settings|Input|Options")
	float LookRightMouseSensitivity{1.0f};

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(DisplayThumbnail=false), Category="Settings|Input|Options")
	float LookUpRate{90.f};

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(DisplayThumbnail=false), Category="Settings|Input|Options")
	float LookRightRate{90.f};
	
public:
	
	ASignetPlayerCharacter(const FObjectInitializer& ObjectInitializer);

	virtual void NotifyControllerChanged() override;

protected:
	
	virtual void BeginPlay() override;
	
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

	virtual void CalcCamera(float DeltaTime, struct FMinimalViewInfo& OutResult) override;

	virtual void PostInitializeComponents() override;

	
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


// Begin Replication Functions

	UFUNCTION()
	void OnRep_VisualState();

public:

	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	

// Begin Debug Display
	
	virtual void DisplayDebug(UCanvas* Canvas, const FDebugDisplayInfo& DisplayInfo, float& Unused, float& VerticalLocation) override;
};

