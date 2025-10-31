#include "SignetEquipmentViewWidget.h"
#include "SignetEquipmentSlotWidget.h"
#include "Components/SizeBox.h"
#include "Components/UniformGridPanel.h"
#include "Components/UniformGridSlot.h"
#include "SignetGame/Inventory/SignetInventoryComponent.h"
#include "SignetGame/Data/GameDataSubsystem.h"
#include "Engine/World.h"

static constexpr float kEquipCellSize = 65.f;

static const TArray<EGearSlot> GVisualOrder = {
	EGearSlot::Main,    EGearSlot::Sub,      EGearSlot::Ranged,   EGearSlot::Ammo,
	EGearSlot::Head,    EGearSlot::Neck,     EGearSlot::LeftEar,  EGearSlot::RightEar,
	EGearSlot::Body,    EGearSlot::Hands,    EGearSlot::LeftRing, EGearSlot::RightRing,
	EGearSlot::Back,    EGearSlot::Waist,    EGearSlot::Legs,     EGearSlot::Feet
};

const TArray<EGearSlot>& USignetEquipmentViewWidget::VisualOrder()
{
	return GVisualOrder;
}

void USignetEquipmentViewWidget::GridPosForIndex(int32 Index, int32& OutRow, int32& OutCol)
{
	OutRow = Index / 4;
	OutCol = Index % 4;
}

void USignetEquipmentViewWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	BuildGridOnce();
	
	if (!Inventory.IsValid())
	{
		if (APlayerController* PC = GetOwningPlayer())
		{
			if (APawn* P = PC->GetPawn())
			{
				if (auto* Cmp = P->FindComponentByClass<USignetInventoryComponent>())
				{
					InitializeWithInventory(Cmp);
				}
			}
		}
	}
}

void USignetEquipmentViewWidget::NativeDestruct()
{
	if (Inventory.IsValid())
	{
		auto* Inv = Inventory.Get();
		Inv->OnEquipmentChanged.RemoveDynamic(this, &USignetEquipmentViewWidget::HandleEquipID);
		Inv->OnEquipmentChangedInstance.RemoveDynamic(this, &USignetEquipmentViewWidget::HandleEquipInstance);
	}
	Super::NativeDestruct();
}

void USignetEquipmentViewWidget::BuildGridOnce()
{
	if (!Grid || SlotWidgets.Num() == GVisualOrder.Num()) { return; }
	if (!SlotWidgetClass) { return; }

	SlotWidgets.SetNum(GVisualOrder.Num());

	for (int32 i = 0; i < GVisualOrder.Num(); ++i)
	{
		// Create the slot widget
		USignetEquipmentSlotWidget* SlotW = CreateWidget<USignetEquipmentSlotWidget>(this, SlotWidgetClass);
		SlotWidgets[i] = SlotW;

		// Wrap it in a fixed-size SizeBox
		USizeBox* Cell = NewObject<USizeBox>(this);
		Cell->SetWidthOverride(kEquipCellSize);
		Cell->SetHeightOverride(kEquipCellSize);

		// Add the slot to the SizeBox
		Cell->AddChild(SlotW);

		// Place the SizeBox into the grid
		int32 R, C;
		GridPosForIndex(i, R, C);
		if (UUniformGridSlot* GridSlot = Grid->AddChildToUniformGrid(Cell, R, C))
		{
			GridSlot->SetHorizontalAlignment(HAlign_Fill);
			GridSlot->SetVerticalAlignment(VAlign_Fill);
		}

		// Bind click
		SlotW->OnSlotClicked.AddDynamic(this, &USignetEquipmentViewWidget::HandleSlotClicked);
	}
}

void USignetEquipmentViewWidget::InitializeWithInventory(USignetInventoryComponent* InInventory)
{
	if (!InInventory) { return; }
	
	if (Inventory.IsValid())
	{
		auto* Old = Inventory.Get();
		Old->OnEquipmentChanged.RemoveDynamic(this, &USignetEquipmentViewWidget::HandleEquipID);
		Old->OnEquipmentChangedInstance.RemoveDynamic(this, &USignetEquipmentViewWidget::HandleEquipInstance);
	}

	Inventory = InInventory;

	InInventory->OnEquipmentChanged.AddDynamic(this, &USignetEquipmentViewWidget::HandleEquipID);
	InInventory->OnEquipmentChangedInstance.AddDynamic(this, &USignetEquipmentViewWidget::HandleEquipInstance);

	RefreshAllSlots();
}

const FInventoryItem* USignetEquipmentViewWidget::FindDef(int32 ItemID) const
{
	if (ItemID == 0) { return nullptr; }
	if (const UWorld* W = GetWorld())
	{
		if (const UGameInstance* GI = W->GetGameInstance())
		{
			if (const auto Sub = GI->GetSubsystem<UGameDataSubsystem>())
			{
				return Sub->GetItem(ItemID);
			}
		}
	}
	return nullptr;
}

void USignetEquipmentViewWidget::RefreshAllSlots()
{
	if (!Inventory.IsValid()) { return; }
	for (int32 i = 0; i < GVisualOrder.Num(); ++i)
	{
		RefreshSlot(GVisualOrder[i]);
	}
}

void USignetEquipmentViewWidget::RefreshSlot(EGearSlot GearSlot)
{
	if (!Inventory.IsValid()) { return; }

	const int32 ItemID = Inventory->GetEquippedItemID(GearSlot);
	const FInventoryItem* Def = FindDef(ItemID);
	
	int32 Index = VisualOrder().Find(GearSlot);
	if (!SlotWidgets.IsValidIndex(Index)) { return; }

	if (USignetEquipmentSlotWidget* SlotW = SlotWidgets[Index])
	{
		SlotW->SetSlotData(GearSlot, Def);
	}
}

void USignetEquipmentViewWidget::HandleEquipID(EGearSlot GearSlot, int32 OldID, int32 NewID)
{
	RefreshSlot(GearSlot);
}

void USignetEquipmentViewWidget::HandleEquipInstance(EGearSlot GearSlot, FGuid OldInst, FGuid NewInst)
{
	// Instance swaps don’t change the icon/name; but if you display a +1 overlay or augment,
	// you could refresh here too. Safe to just refresh.
	RefreshSlot(GearSlot);
}

void USignetEquipmentViewWidget::HandleSlotClicked(EGearSlot InSlot)
{
	FocusedSlot = InSlot;
}
