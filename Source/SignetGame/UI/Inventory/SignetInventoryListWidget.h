#pragma once
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SignetGame/Inventory/InventoryTypes.h"
#include "SignetInventoryListWidget.generated.h"

class UListView;
class USignetInventoryComponent;
class USignetItemDataSubsystem;
class USignetInventoryListItem;

UENUM(BlueprintType)
enum class EItemSortMode : uint8
{
	ByAlphabetical,
	ByItemType
};


UCLASS()
class SIGNETGAME_API USignetInventoryListWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	
	UFUNCTION(BlueprintCallable, Category="Inventory")
	void InitializeWithInventory(USignetInventoryComponent* InInventory);
	
	UFUNCTION(BlueprintCallable, Category="Inventory")
	void SetTextFilter(const FString& InFilter);
	
	UFUNCTION(BlueprintCallable, Category="Inventory|Sort")
	void SetSortMode(EItemSortMode InMode);
	
	UFUNCTION(BlueprintCallable, Category="Inventory|Filter")
	void SetAllowedItemTypes(const TSet<EItemType>& InAllowed);

	UFUNCTION(BlueprintCallable, Category="Inventory|Filter")
	void ClearAllowedItemTypes();

protected:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UListView> ItemList = nullptr;
	
	void RebuildList();
	
	UFUNCTION() void HandleBagAdded(int32 ItemID, uint16 Qty);
	UFUNCTION() void HandleBagChanged(int32 ItemID, uint16 NewQty);
	UFUNCTION() void HandleBagRemoved(int32 ItemID);
	UFUNCTION() void HandleEquipID(EGearSlot InSlot, int32 OldID, int32 NewID);
	UFUNCTION() void HandleEquipInstance(EGearSlot InSlot, FGuid OldG, FGuid NewG);

	const FInventoryItem* FindDef(int32 ItemID) const;
	bool IsAnyInstanceEquipped(int32 ItemID) const;

private:
	UPROPERTY()
	TWeakObjectPtr<USignetInventoryComponent> Inventory;
	
	FString TextFilter;
	
	UPROPERTY()
	EItemSortMode SortMode = EItemSortMode::ByAlphabetical;
	
	UPROPERTY()
	TSet<EItemType> AllowedTypes;
	
	static int32 TypePriority(EItemType T);
};