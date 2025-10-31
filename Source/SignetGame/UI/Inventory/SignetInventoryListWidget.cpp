#include "SignetInventoryListWidget.h"
#include "Components/ListView.h"
#include "SignetInventoryListItem.h"
#include "Engine/World.h"
#include "SignetGame/Data/GameDataSubsystem.h"
#include "SignetGame/Inventory/InventoryTypes.h"
#include "SignetGame/Inventory/SignetInventoryComponent.h"

static bool IsThisEntryEquipped(const USignetInventoryComponent* Inv, const FGuid& Instance)
{
	if (!Inv) return false;
	if (!Instance.IsValid()) return false; 
	for (int32 i = 0; i < USignetInventoryComponent::SlotCount(); ++i)
	{
		if (Inv->GetEquippedInstance(static_cast<EGearSlot>(i)) == Instance)
			return true;
	}
	return false;
}

void USignetInventoryListWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	if (!Inventory.IsValid())
	{
		if (APlayerController* PC = GetOwningPlayer())
		{
			if (const auto P = PC->GetPawn())
			{
				if (auto* Cmp = P->FindComponentByClass<USignetInventoryComponent>())
				{
					InitializeWithInventory(Cmp);
				}
			}
		}
	}
}

void USignetInventoryListWidget::NativeDestruct()
{
	// Unbind if bound
	if (Inventory.IsValid())
	{
		auto* Inv = Inventory.Get();
		Inv->OnBagEntryAdded.RemoveDynamic(this, &USignetInventoryListWidget::HandleBagAdded);
		Inv->OnBagEntryChanged.RemoveDynamic(this, &USignetInventoryListWidget::HandleBagChanged);
		Inv->OnBagEntryRemoved.RemoveDynamic(this, &USignetInventoryListWidget::HandleBagRemoved);
		Inv->OnEquipmentChanged.RemoveDynamic(this, &USignetInventoryListWidget::HandleEquipID);
		Inv->OnEquipmentChangedInstance.RemoveDynamic(this, &USignetInventoryListWidget::HandleEquipInstance);
	}
	Super::NativeDestruct();
}

void USignetInventoryListWidget::InitializeWithInventory(USignetInventoryComponent* InInventory)
{
	if (!InInventory) return;

	// Unbind previous
	if (Inventory.IsValid())
	{
		auto* Old = Inventory.Get();
		Old->OnBagEntryAdded.RemoveDynamic(this, &USignetInventoryListWidget::HandleBagAdded);
		Old->OnBagEntryChanged.RemoveDynamic(this, &USignetInventoryListWidget::HandleBagChanged);
		Old->OnBagEntryRemoved.RemoveDynamic(this, &USignetInventoryListWidget::HandleBagRemoved);
		Old->OnEquipmentChanged.RemoveDynamic(this, &USignetInventoryListWidget::HandleEquipID);
		Old->OnEquipmentChangedInstance.RemoveDynamic(this, &USignetInventoryListWidget::HandleEquipInstance);
	}

	Inventory = InInventory;

	// Bind new
	InInventory->OnBagEntryAdded.AddDynamic(this, &USignetInventoryListWidget::HandleBagAdded);
	InInventory->OnBagEntryChanged.AddDynamic(this, &USignetInventoryListWidget::HandleBagChanged);
	InInventory->OnBagEntryRemoved.AddDynamic(this, &USignetInventoryListWidget::HandleBagRemoved);
	InInventory->OnEquipmentChanged.AddDynamic(this, &USignetInventoryListWidget::HandleEquipID);
	InInventory->OnEquipmentChangedInstance.AddDynamic(this, &USignetInventoryListWidget::HandleEquipInstance);

	RebuildList();
}

void USignetInventoryListWidget::SetTextFilter(const FString& InFilter)
{
	TextFilter = InFilter;
	RebuildList();
}

const FInventoryItem* USignetInventoryListWidget::FindDef(int32 ItemID) const
{
	if (ItemID == 0) return nullptr;
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

bool USignetInventoryListWidget::IsAnyInstanceEquipped(int32 ItemID) const
{
	if (!Inventory.IsValid()) return false;
	
	for (int32 i = 0; i < USignetInventoryComponent::SlotCount(); ++i)
	{
		if (Inventory->GetEquippedItemID(static_cast<EGearSlot>(i)) == ItemID)
		{
			return true;
		}
	}
	return false;
}

void USignetInventoryListWidget::RebuildList()
{
	if (!ItemList) return;
	ItemList->ClearListItems();

	if (!Inventory.IsValid()) return;
	const auto Inv = Inventory.Get();

	// 1) Build raw rows (one per bag entry), applying FILTERS
	TArray<USignetInventoryListItem*> Rows;
	Rows.Reserve(Inv->GetBag().Entries.Num());

	for (const auto& E : Inv->GetBag().Entries)
	{
		const FInventoryItem* Def = FindDef(E.ItemID);

		// Type filter (if any)
		EItemType RowType = Def ? Def->ItemType : EItemType::None;
		if (AllowedTypes.Num() > 0 && !AllowedTypes.Contains(RowType))
		{
			continue;
		}

		// Create row
		USignetInventoryListItem* Row = NewObject<USignetInventoryListItem>(this);
		Row->InstanceId  = E.InstanceId;
		Row->ItemID      = E.ItemID;
		Row->Quantity    = static_cast<int32>(E.Quantity);
		Row->bIsEquipped = (E.InstanceId.IsValid() && IsThisEntryEquipped(Inv, E.InstanceId));
		Row->ItemType    = RowType;

		if (Def)
		{
			Row->DisplayName = FText::FromString(Def->En.Len() ? Def->En : FString::FromInt(E.ItemID));
			Row->Icon        = Cast<UTexture2D>(Def->Icon);
			Row->ItemQuality = Def->ItemQuality;
			Row->ItemType    = Def->ItemType;
			Row->GearSlot	 = Def->GearSlot;
		}
		else
		{
			Row->DisplayName = FText::AsNumber(E.ItemID);
			Row->Icon        = nullptr;
			Row->ItemQuality = EItemQuality::Normal; // sensible default
			Row->ItemType    = EItemType::None;
		}

		// Text filter (name or ItemID)
		if (!TextFilter.IsEmpty())
		{
			const FString Name = Row->DisplayName.ToString();
			if (!Name.Contains(TextFilter, ESearchCase::IgnoreCase) &&
				!FString::FromInt(Row->ItemID).Contains(TextFilter, ESearchCase::IgnoreCase))
			{
				continue;
			}
		}

		Rows.Add(Row);
	}

	// 2) SORT
	// Notes:
	// - Consumables always float to top via primary key (TypePriority)
	// - For ItemType mode: group by type (priority), then Name (A→Z), then Quality (desc), then Quantity (desc)
	// - For Alphabetical mode: still lift Consumables first via TypePriority, then Name (A→Z), Quality (desc), Quantity (desc)
	//   (If you want absolutely strict “pure alpha” ignoring type except consumables-first, this matches your spec.)
	Rows.Sort([this](const USignetInventoryListItem& A, const USignetInventoryListItem& B)
	{
		const int32 PriA = TypePriority(A.ItemType);
		const int32 PriB = TypePriority(B.ItemType);
		if (PriA != PriB)
		{
			return PriA < PriB; // Consumable first, etc.
		}

		// If ItemType grouping requested, keep types grouped by their priority
		if (SortMode == EItemSortMode::ByItemType)
		{
			// Names (ascending)
			{
				const int32 Cmp = A.DisplayName.ToString().Compare(B.DisplayName.ToString(), ESearchCase::IgnoreCase);
				if (Cmp != 0) return Cmp < 0;
			}
			// Quality (desc) — assumes higher enum value = higher quality; flip if your enum is reversed
			if (A.ItemQuality != B.ItemQuality)
			{
				return static_cast<uint8>(A.ItemQuality) > static_cast<uint8>(B.ItemQuality);
			}
			// Quantity (desc)
			if (A.Quantity != B.Quantity)
			{
				return A.Quantity > B.Quantity;
			}
			// Tie-breaker: ItemID (asc)
			return A.ItemID < B.ItemID;
		}
		else // ByAlphabetical, still with consumables floated earlier by type priority
		{
			// Name (ascending)
			{
				const int32 Cmp = A.DisplayName.ToString().Compare(B.DisplayName.ToString(), ESearchCase::IgnoreCase);
				if (Cmp != 0) return Cmp < 0;
			}
			// Quality (desc)
			if (A.ItemQuality != B.ItemQuality)
			{
				return static_cast<uint8>(A.ItemQuality) > static_cast<uint8>(B.ItemQuality);
			}
			// Quantity (desc)
			if (A.Quantity != B.Quantity)
			{
				return A.Quantity > B.Quantity;
			}
			// Tie-breaker: ItemID (asc)
			return A.ItemID < B.ItemID;
		}
	});

	// 3) Feed the ListView
	for (auto* Row : Rows)
	{
		ItemList->AddItem(Row);
	}
	ItemList->RequestRefresh();
}

void USignetInventoryListWidget::HandleBagAdded(int32, uint16){ RebuildList(); }
void USignetInventoryListWidget::HandleBagChanged(int32, uint16){ RebuildList(); }
void USignetInventoryListWidget::HandleBagRemoved(int32){ RebuildList(); }
void USignetInventoryListWidget::HandleEquipID(EGearSlot, int32, int32){ RebuildList(); }
void USignetInventoryListWidget::HandleEquipInstance(EGearSlot, FGuid, FGuid){ RebuildList(); }


int32 USignetInventoryListWidget::TypePriority(EItemType T)
{
	switch (T)
	{
	case EItemType::Consumable: return 0;
	case EItemType::Material:   return 1;   
	case EItemType::Equipment:  return 2;
	default:                    return 3;
	}
}

void USignetInventoryListWidget::SetSortMode(EItemSortMode InMode)
{
	SortMode = InMode;
	RebuildList();
}

void USignetInventoryListWidget::SetAllowedItemTypes(const TSet<EItemType>& InAllowed)
{
	AllowedTypes = InAllowed;
	RebuildList();
}

void USignetInventoryListWidget::ClearAllowedItemTypes()
{
	AllowedTypes.Reset();
	RebuildList();
}