// Copyright Red Lotus Games, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AlsCharacter.h"
#include "InputActionValue.h"
#include "SignetPlayerCharacter.generated.h"


#define HIDDEN_PROPERTY() UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta=(HideInDetailPanel=true))
#define INPUT_PROPERTY(CategoryName) \
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=CategoryName, meta=(DisplayThumbnail=false))

class UInventoryComponent;
class UTargetingComponent;
class UStatsComponent;
class USignetCameraComponent;
struct FInputActionValue;
class UInputAction;
class UInputMappingContext;

UCLASS()
class SIGNETGAME_API ASignetPlayerCharacter : public AAlsCharacter
{
	GENERATED_BODY()

// Begin Visual Mesh Systems
protected:

	
	HIDDEN_PROPERTY() TObjectPtr<USkeletalMeshComponent> MainMesh;
	HIDDEN_PROPERTY() TObjectPtr<USkeletalMeshComponent> SubMesh;
	HIDDEN_PROPERTY() TObjectPtr<USkeletalMeshComponent> RangedMesh;
	HIDDEN_PROPERTY() TObjectPtr<USkeletalMeshComponent> FaceMesh;
	HIDDEN_PROPERTY() TObjectPtr<USkeletalMeshComponent> HeadMesh;
	HIDDEN_PROPERTY() TObjectPtr<USkeletalMeshComponent> BodyMesh;
	HIDDEN_PROPERTY() TObjectPtr<USkeletalMeshComponent> HandsMesh;
	HIDDEN_PROPERTY() TObjectPtr<USkeletalMeshComponent> LegsMesh;
	HIDDEN_PROPERTY() TObjectPtr<USkeletalMeshComponent> FeetMesh;

// Begin Camera System

	HIDDEN_PROPERTY() TObjectPtr<USignetCameraComponent> Camera;

// Begin Components

	HIDDEN_PROPERTY() TObjectPtr<UStatsComponent> Stats;
	HIDDEN_PROPERTY() TObjectPtr<UTargetingComponent> Targeting;
	HIDDEN_PROPERTY() TObjectPtr<UInventoryComponent> Inventory;

public:

	UFUNCTION(BlueprintCallable, BlueprintPure)
	FORCEINLINE UStatsComponent* GetStatsComponent() const { return Stats; }
	
	UFUNCTION(BlueprintCallable, BlueprintPure)
	FORCEINLINE UTargetingComponent* GetTargetingComponent() const { return Targeting; }
	
	UFUNCTION(BlueprintCallable, BlueprintPure)
	FORCEINLINE UInventoryComponent* GetInventoryComponent() const { return Inventory; }

protected:

// Begin Player Input Schema

	INPUT_PROPERTY("Settings|Input")
	TObjectPtr<UInputMappingContext> DefaultMappingContext;
	
	INPUT_PROPERTY("Settings|Input|Bindings")
	TObjectPtr<UInputAction> LookMouseAction;

	INPUT_PROPERTY("Settings|Input|Bindings")
	TObjectPtr<UInputAction> LookAction;

	INPUT_PROPERTY("Settings|Input|Bindings") 
	TObjectPtr<UInputAction> MoveAction;

	INPUT_PROPERTY("Settings|Input|Bindings") 
	TObjectPtr<UInputAction> ToggleRunWalkAction;

	INPUT_PROPERTY("Settings|Input|Bindings") 
	TObjectPtr<UInputAction> JumpAction;

	INPUT_PROPERTY("Settings|Input|Bindings") 
	TObjectPtr<UInputAction> SitAction;

	INPUT_PROPERTY("Settings|Input|Bindings") // Doubles as the Target Lock action 
	TObjectPtr<UInputAction> HealAction;

	INPUT_PROPERTY("Settings|Input|Bindings") 
	TObjectPtr<UInputAction> EmoteAction;
	
	INPUT_PROPERTY("Settings|Input|Bindings") 
	TObjectPtr<UInputAction> AcceptAction;

	INPUT_PROPERTY("Settings|Input|Bindings") 
	TObjectPtr<UInputAction> CancelAction;

	INPUT_PROPERTY("Settings|Input|Bindings") 
	TObjectPtr<UInputAction> MenuAction;

	INPUT_PROPERTY("Settings|Input|Options")
	float LookUpMouseSensitivity{1.0f};

	INPUT_PROPERTY("Settings|Input|Options")
	float LookRightMouseSensitivity{1.0f};

	INPUT_PROPERTY("Settings|Input|Options")
	float LookUpRate{90.f};

	INPUT_PROPERTY("Settings|Input|Options")
	float LookRightRate{90.f};
	
public:

	ASignetPlayerCharacter();

	virtual void NotifyControllerChanged() override;

protected:
	
	virtual void BeginPlay() override;
	
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

	virtual void CalcCamera(float DeltaTime, struct FMinimalViewInfo& OutResult) override;

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

// Begin Debug Display
public:
	virtual void DisplayDebug(UCanvas* Canvas, const FDebugDisplayInfo& DisplayInfo, float& Unused, float& VerticalLocation) override;
};

#undef HIDDEN_PROPERTY
#undef INPUT_PROPERTY