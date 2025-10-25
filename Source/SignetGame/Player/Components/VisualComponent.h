// Copyright Red Lotus Games, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SignetGame/Player/VisualState.h"
#include "SignetGame/Inventory/InventoryTypes.h"
#include "VisualComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnGearSlotChanged, const EGearSlot&, ChangedSlot, USkeletalMesh*, NewMesh);

UCLASS(ClassGroup=(Signet), meta=(BlueprintSpawnableComponent))
class SIGNETGAME_API UVisualComponent : public UActorComponent
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintAssignable)
	FOnGearSlotChanged OnGearSlotChanged;

	UVisualComponent();
	
	virtual void AsyncUpdateSlot(const EGearSlot& Slot, uint16 NewItemID);
	
	virtual void AsyncUpdateVisualState(const FVisualState& NewState);

};


// Inventory Component
// -> Player Changes Equipment
// -> Trigger's the OnGearSlotChanged Delegate
// -> Pawn's OnGearSlotChanged function is triggered
// -> Pawn Trigger's Visual Component's LoadVisualComponent
//