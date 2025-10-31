#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SignetGame/Inventory/InventoryTypes.h"
#include "SignetEquipmentViewWidget.generated.h"

class UUniformGridPanel;
class USignetInventoryComponent;
class USignetEquipmentSlotWidget;
class UUserWidget;
struct FInventoryItem;

UCLASS()
class SIGNETGAME_API USignetEquipmentViewWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, Category="Equipment")
	void InitializeWithInventory(USignetInventoryComponent* InInventory);

	// Get current focused/selected slot if you want to open an equip picker
	UFUNCTION(BlueprintPure, Category="Equipment")
	EGearSlot GetFocusedSlot() const { return FocusedSlot; }

protected:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

	// === Layout ===
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UUniformGridPanel> Grid = nullptr;

	// BP slot class (defaults to a BP inheriting USignetEquipmentSlotWidget)
	UPROPERTY(EditDefaultsOnly, Category="Equipment")
	TSubclassOf<USignetEquipmentSlotWidget> SlotWidgetClass;

	// === Event hooks ===
	UFUNCTION() void HandleEquipID(EGearSlot GearSlot, int32 OldID, int32 NewID);
	UFUNCTION() void HandleEquipInstance(EGearSlot GearSlot, FGuid OldInst, FGuid NewInst);

	UFUNCTION()
	void HandleSlotClicked(EGearSlot InSlot);

private:
	UPROPERTY()
	TWeakObjectPtr<USignetInventoryComponent> Inventory;

	UPROPERTY()
	TArray<TObjectPtr<USignetEquipmentSlotWidget>> SlotWidgets;

	UPROPERTY()
	EGearSlot FocusedSlot = EGearSlot::None;

	// helpers
	void BuildGridOnce();
	void RefreshAllSlots();
	void RefreshSlot(EGearSlot GearSlot);
	const FInventoryItem* FindDef(int32 ItemID) const;

	// grid mapping (4x4 in visual order)
	static const TArray<EGearSlot>& VisualOrder();
	static void GridPosForIndex(int32 Index, int32& OutRow, int32& OutCol);
};