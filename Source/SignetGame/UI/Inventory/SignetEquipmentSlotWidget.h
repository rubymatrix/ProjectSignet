// Copyright Red Lotus Games, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SignetGame/Inventory/InventoryTypes.h"
#include "SignetEquipmentSlotWidget.generated.h"

class UButton;
class UImage;
class UTextBlock;
class UBorder;
struct FInventoryItem;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEquipmentSlotClicked, EGearSlot, GearSlot);

UCLASS()
class SIGNETGAME_API USignetEquipmentSlotWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	USignetEquipmentSlotWidget(const FObjectInitializer& ObjectInitializer);

	// Called by the view to (re)populate
	void SetSlotData(EGearSlot InSlot, const FInventoryItem* DefOrNull);

	// External selection (optional highlight)
	UFUNCTION(BlueprintCallable, Category="Equipment")
	void SetSelected(bool bInSelected);

	// Click/press event outward
	UPROPERTY(BlueprintAssignable, Category="Equipment|Events")
	FOnEquipmentSlotClicked OnSlotClicked;

	EGearSlot GetSlot() const { return GearSlot; }

protected:
	virtual void NativeConstruct() override;
	virtual FReply NativeOnFocusReceived(const FGeometry& InGeometry, const FFocusEvent& InFocusEvent) override;
	virtual void NativeOnFocusLost(const FFocusEvent& InFocusEvent) override;
	virtual FReply NativeOnKeyDown(const FGeometry& MyGeometry, const FKeyEvent& InKeyEvent) override;

	// --- BindWidget references (create these in your BP) ---
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UButton> RootButton = nullptr;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UImage> IconImage = nullptr;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> SlotLabel = nullptr;

	UPROPERTY(meta=(BindWidgetOptional))
	TObjectPtr<UBorder> FocusBorder = nullptr;

	UPROPERTY(meta=(BindWidgetOptional))
	TObjectPtr<UBorder> SelectedBorder = nullptr;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UBorder> ItemQualityBorder = nullptr;

private:
	UFUNCTION() void HandleClicked();

	UPROPERTY()
	EGearSlot GearSlot = EGearSlot::None;

	// Cached for quick empty/equipped checks
	UPROPERTY()
	bool bHasItem = false;
};