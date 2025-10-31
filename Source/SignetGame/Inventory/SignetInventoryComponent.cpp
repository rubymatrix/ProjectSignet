﻿// Copyright Red Lotus Games, All Rights Reserved.


#include "SignetInventoryComponent.h"
#include "Net/UnrealNetwork.h"
#include "SignetGame/Data/GameDataSubsystem.h"
#include "SignetGame/Save/SignetSaveSubsystem.h"


class USignetSaveSubsystem;

void FInvList::PreReplicatedRemove(const TArrayView<int32>& Removed, int32)
{
	RemovedItemIDsShadow.Reset(Removed.Num());
	for (const int32 Idx : Removed)
	{
		if (Entries.IsValidIndex(Idx))
		{
			RemovedItemIDsShadow.Add(Entries[Idx].ItemID);
		}
	}
}
void FInvList::PostReplicatedRemove(const TArrayView<int32>&, int32)
{
	if (!Owner) return;
	for (const int32 ItemID : RemovedItemIDsShadow)
	{
		Owner->OnBagEntryRemoved.Broadcast(ItemID);
	}
	RemovedItemIDsShadow.Reset();
}
void FInvList::PostReplicatedAdd(const TArrayView<int32>& Added, int32)
{
	if (!Owner) return;
	for (const int32 Idx : Added)
	{
		const FInvEntry& E = Entries[Idx];
		Owner->OnBagEntryAdded.Broadcast(E.ItemID, E.Quantity);
	}
}
void FInvList::PostReplicatedChange(const TArrayView<int32>& Changed, int32)
{
	if (!Owner) return;
	for (const int32 Idx : Changed)
	{
		const FInvEntry& E = Entries[Idx];
		Owner->OnBagEntryChanged.Broadcast(E.ItemID, E.Quantity);
	}
}

USignetInventoryComponent::USignetInventoryComponent()
{
	SetIsReplicatedByDefault(true);
	Bag.Owner = this;

	EquippedItemIDs.SetNum(SlotCount());
	EquippedInstanceIds.SetNum(SlotCount());
}

void USignetInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	OnBagEntryAdded.AddDynamic(this, &USignetInventoryComponent::HandleBagAdded);
	OnBagEntryChanged.AddDynamic(this, &USignetInventoryComponent::HandleBagChanged);
	OnBagEntryRemoved.AddDynamic(this, &USignetInventoryComponent::HandleBagRemoved);
	OnEquipmentChanged.AddDynamic(this, &USignetInventoryComponent::HandleEquipID);
	OnEquipmentChangedInstance.AddDynamic(this, &USignetInventoryComponent::HandleEquipInstance);
}

void USignetInventoryComponent::InvPrint()
{
	auto Print = [](const FString& S){
		if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, S);
		UE_LOG(LogTemp, Log, TEXT("%s"), *S);
	};

	Print(TEXT("=== BAG ==="));
	for (const auto& E : Bag.Entries)
	{
		Print(FString::Printf(TEXT("ItemID=%d Qty=%d Inst=%s"),
			E.ItemID, (int32)E.Quantity, *E.InstanceId.ToString()));
	}

	Print(TEXT("=== EQUIPPED ==="));
	for (int32 i=0;i<EquippedItemIDs.Num();++i)
	{
		const int32 ID   = EquippedItemIDs[i];
		const FGuid Inst = EquippedInstanceIds.IsValidIndex(i) ? EquippedInstanceIds[i] : FGuid();
		Print(FString::Printf(TEXT("%d) Slot=%d  ItemID=%d  Inst=%s"),
			i, i, ID, *Inst.ToString()));
	}
}


void USignetInventoryComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(USignetInventoryComponent, Bag);
	DOREPLIFETIME(USignetInventoryComponent, EquippedItemIDs);
	DOREPLIFETIME(USignetInventoryComponent, EquippedInstanceIds);
}

int32 USignetInventoryComponent::GetTotalQuantity(const int32 ItemID) const
{
	int32 Total = 0;
	for (const auto& E : Bag.Entries)
	{
		if (E.ItemID == ItemID) Total += static_cast<int32>(E.Quantity);
	}
	return Total;
}

int32 USignetInventoryComponent::GetEquippedItemID(const EGearSlot Slot) const
{
	const int32 I = SlotToIndex(Slot);
	return EquippedItemIDs.IsValidIndex(I) ? EquippedItemIDs[I] : 0;
}

void USignetInventoryComponent::GetEquippedItemDefBP(const EGearSlot Slot, FInventoryItem& OutItem, bool& bFound) const
{
	bFound = false;
	OutItem = FInventoryItem();

	const int32 ItemID = GetEquippedItemID(Slot);
	if (ItemID == 0) return;

	if (const FInventoryItem* Def = FindDef(ItemID))
	{
		OutItem = *Def;
		bFound = true;
	}
}

FGuid USignetInventoryComponent::GetEquippedInstance(EGearSlot Slot) const
{
	const int32 I = SlotToIndex(Slot);
	return EquippedInstanceIds.IsValidIndex(I) ? EquippedInstanceIds[I] : FGuid();
}

bool USignetInventoryComponent::IsEquipped(const FGuid& InstanceId) const
{
	for (const FGuid& G : EquippedInstanceIds)
	{
		if (G == InstanceId) return true;
	}
	return false;
}

const FInventoryItem* USignetInventoryComponent::FindDef(const int32 ItemID) const
{
	if (ItemID == 0) return nullptr;
	const UWorld* World = GetWorld();
	if (!World) return nullptr;
	const UGameInstance* GI = World->GetGameInstance();
	if (!GI) return nullptr;
	if (const auto Sub = GI->GetSubsystem<UGameDataSubsystem>())
	{
		return Sub->GetItem(ItemID);
	}
	return nullptr;
}

void USignetInventoryComponent::OnRep_EquippedIDs(const TArray<int32>& Old)
{
	BroadcastEquipChangeFromArrays(Old, EquippedItemIDs);
}

void USignetInventoryComponent::OnRep_EquippedInstances(const TArray<FGuid>& Old)
{
	const int32 Count = FMath::Min(Old.Num(), EquippedInstanceIds.Num());
	for (int32 i = 0; i < Count; ++i)
	{
		const FGuid& OldGuid = Old[i];
		const FGuid& NewGuid = EquippedInstanceIds[i];
		if (OldGuid != NewGuid)
		{
			OnEquipmentChangedInstance.Broadcast(static_cast<EGearSlot>(i), OldGuid, NewGuid);
		}
	}
}

void USignetInventoryComponent::BroadcastEquipChangeFromArrays(const TArray<int32>& OldIDs, const TArray<int32>& NewIDs)
{
	const int32 Count = FMath::Min(OldIDs.Num(), NewIDs.Num());
	for (int32 i = 0; i < Count; ++i)
	{
		if (OldIDs[i] != NewIDs[i])
		{
			OnEquipmentChanged.Broadcast(static_cast<EGearSlot>(i), OldIDs[i], NewIDs[i]);
		}
	}
}

void USignetInventoryComponent::CaptureToSave(FSignetSavedInventory& Out) const
{
	Out = FSignetSavedInventory{};
	Out.Bag.Reserve(Bag.Entries.Num());

	for (const auto& E : Bag.Entries)
	{
		FSignetSavedBagEntry SE;
		SE.ItemID     = E.ItemID;
		SE.Quantity   = (int32)E.Quantity;
		SE.InstanceId = E.InstanceId;
		Out.Bag.Add(SE);
	}

	Out.EquippedItemIDs     = EquippedItemIDs;
	Out.EquippedInstanceIds = EquippedInstanceIds;
	Out.SchemaVersion       = 1;
}

void USignetInventoryComponent::LoadFromSave(const FSignetSavedInventory& In)
{
	Bag.Entries.Reset();
	
	Bag.Entries.Reserve(In.Bag.Num());
	for (const auto& SE : In.Bag)
	{
		auto& NewE = Bag.Entries.AddDefaulted_GetRef();
		NewE.ItemID     = SE.ItemID;
		NewE.Quantity   = (uint16)FMath::Clamp(SE.Quantity, 0, 65535);
		NewE.InstanceId = SE.InstanceId;
	}
	Bag.MarkArrayDirty();
	
	const int32 SlotCount = USignetInventoryComponent::SlotCount();

	EquippedItemIDs.SetNum(SlotCount);
	EquippedInstanceIds.SetNum(SlotCount);

	if (In.EquippedItemIDs.Num() == SlotCount)
	{
		EquippedItemIDs = In.EquippedItemIDs;
	}
	else
	{
		for (int32 i = 0; i < SlotCount; ++i) { EquippedItemIDs[i] = 0; }
	}

	if (In.EquippedInstanceIds.Num() == SlotCount)
	{
		EquippedInstanceIds = In.EquippedInstanceIds;
	}
	else
	{
		for (int32 i = 0; i < SlotCount; ++i) { EquippedInstanceIds[i] = FGuid(); }
	}
	
	for (int32 i = 0; i < SlotCount; ++i)
	{
		const int32 ItemID = EquippedItemIDs[i];
		const FGuid Inst   = EquippedInstanceIds[i];

		if (ItemID == 0 || !Inst.IsValid())
		{
			EquippedItemIDs[i]     = 0;
			EquippedInstanceIds[i] = FGuid();
			continue;
		}

		bool bFound = false;
		for (const auto& E : Bag.Entries)
		{
			if (E.ItemID == ItemID && E.InstanceId == Inst)
			{
				bFound = true;
				break;
			}
		}
		if (!bFound)
		{
			EquippedItemIDs[i]     = 0;
			EquippedInstanceIds[i] = FGuid();
		}
	}
}

void USignetInventoryComponent::ServerInventoryMutated()
{
	FSignetSavedInventory Snapshot;
	CaptureToSave(Snapshot);
	ClientSyncInventory(Snapshot);
}

void USignetInventoryComponent::ClientSyncInventory_Implementation(const FSignetSavedInventory& Snapshot)
{
	if (const UWorld* W = GetWorld())
	{
		if (const UGameInstance* GI = W->GetGameInstance())
		{
			if (const auto SaveSys = GI->GetSubsystem<USignetSaveSubsystem>())
			{
				if (auto* Save = SaveSys->GetSave())
				{
					Save->Inventory = Snapshot;
					SaveSys->MarkDirty();
				}
			}
		}
	}
}

void USignetInventoryComponent::HandleBagAdded(int32 ItemID, uint16 Qty)
{
	if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Cyan,
		FString::Printf(TEXT("[BagAdded] %d x%d"), ItemID, static_cast<int32>(Qty)));
}

void USignetInventoryComponent::HandleBagChanged(int32 ItemID, uint16 NewQty)
{
	if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Cyan,
		FString::Printf(TEXT("[BagChanged] %d -> %d"), ItemID, static_cast<int32>(NewQty)));
}

void USignetInventoryComponent::HandleBagRemoved(int32 ItemID)
{
	if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Cyan,
		FString::Printf(TEXT("[BagRemoved] %d"), ItemID));
}

void USignetInventoryComponent::HandleEquipID(EGearSlot Slot, int32 OldID, int32 NewID)
{
	if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Yellow,
		FString::Printf(TEXT("[EquipID] Slot %d: %d -> %d"), static_cast<int32>(Slot), OldID, NewID));
}

void USignetInventoryComponent::HandleEquipInstance(EGearSlot Slot, FGuid OldG, FGuid NewG)
{
	if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Yellow,
		FString::Printf(TEXT("[EquipInst] Slot %d: %s -> %s"),
			static_cast<int32>(Slot), *OldG.ToString(), *NewG.ToString()));
}


void USignetInventoryComponent::ServerTryAddItem_Implementation(int32 ItemID, int32 Quantity)
{
	int32 Leftover = 0;
	if (TryAddItem_Internal(ItemID, Quantity, Leftover))
	{
		ServerInventoryMutated();
	}
}

void USignetInventoryComponent::ServerConsumeItem_Implementation(int32 ItemID, int32 Quantity)
{
	if (TryRemoveItem_Internal(ItemID, Quantity))
	{
		ServerInventoryMutated();
	}
}

void USignetInventoryComponent::ServerEquipItemByItemID_Implementation(EGearSlot Slot, int32 ItemID)
{
	if (TryEquip_Internal(Slot, ItemID, FGuid()))
	{
		ServerInventoryMutated();
	}
}

void USignetInventoryComponent::ServerEquipFromInstance_Implementation(EGearSlot Slot, const FGuid& InstanceId)
{
	int32 FoundItemID = 0;
	for (const auto& E : Bag.Entries)
	{
		if (E.InstanceId == InstanceId)
		{
			FoundItemID = E.ItemID;
			break;
		}
	}
	if (FoundItemID != 0)
	{
		if (TryEquip_Internal(Slot, FoundItemID, InstanceId))
		{
			ServerInventoryMutated();
		}
	}
}

void USignetInventoryComponent::ServerUnequip_Implementation(EGearSlot Slot)
{
	if (TryUnequip_Internal(Slot))
	{
		ServerInventoryMutated();
	}
}

bool USignetInventoryComponent::TryAddItem_Internal(int32 ItemID, int32 Quantity, int32& OutLeftover)
{
	OutLeftover = 0;
	if (Quantity <= 0) return true;

	const FInventoryItem* Def = FindDef(ItemID);
	if (!Def) { OutLeftover = Quantity; return false; }

	int32 ToAdd = Quantity;
	const bool bIsEquipment = (Def->ItemType == EItemType::Equipment) && (Def->MaxStackSize == 1);

	
	if (!bIsEquipment)
	{
		for (auto& E : Bag.Entries)
		{
			if (E.ItemID != ItemID) continue;
			const int32 Room = FMath::Max(0, Def->MaxStackSize - static_cast<int32>(E.Quantity));
			if (Room > 0)
			{
				const int32 AddAmt = FMath::Min(Room, ToAdd);
				E.Quantity = static_cast<uint16>(static_cast<int32>(E.Quantity) + AddAmt);
				Bag.MarkItemDirty(E);
				
				OnBagEntryChanged.Broadcast(ItemID, E.Quantity);
				
				ToAdd -= AddAmt;
				if (ToAdd <= 0) break;
			}
		}
	}
	
	while (ToAdd > 0)
	{
		if (bIsEquipment)
		{
			FInvEntry& NewE = *new (Bag.Entries) FInvEntry();
			NewE.ItemID    = ItemID;
			NewE.Quantity  = 1;
			NewE.InstanceId = FGuid::NewGuid();
			Bag.MarkItemDirty(NewE);
			
			OnBagEntryAdded.Broadcast(NewE.ItemID, NewE.Quantity);
			
			--ToAdd;
		}
		else
		{
			const int32 AddAmt = FMath::Min(Def->MaxStackSize, ToAdd);
			FInvEntry& NewE = *new (Bag.Entries) FInvEntry();
			NewE.ItemID    = ItemID;
			NewE.Quantity  = static_cast<uint16>(AddAmt);
			NewE.InstanceId.Invalidate();
			Bag.MarkItemDirty(NewE);
			
			OnBagEntryAdded.Broadcast(NewE.ItemID, NewE.Quantity);
			
			ToAdd -= AddAmt;
		}
		
	}

	Bag.MarkArrayDirty();
	return true;
}

bool USignetInventoryComponent::TryRemoveItem_Internal(int32 ItemID, int32 Quantity)
{
	if (Quantity <= 0) return true;

	int32 ToRemove = Quantity;
	for (int32 i = Bag.Entries.Num() - 1; i >= 0 && ToRemove > 0; --i)
	{
		auto& E = Bag.Entries[i];
		if (E.ItemID != ItemID) continue;

		const int32 Take = FMath::Min<int32>(E.Quantity, ToRemove);
		E.Quantity = static_cast<uint16>(static_cast<int32>(E.Quantity) - Take);
		ToRemove  -= Take;

		if (E.Quantity == 0)
		{
			Bag.Entries.RemoveAtSwap(i);
			Bag.MarkArrayDirty();
		}
		else
		{
			Bag.MarkItemDirty(E);
		}
	}
	return ToRemove == 0;
}

bool USignetInventoryComponent::TryEquip_Internal(EGearSlot Slot, int32 ItemID, FGuid InstanceId)
{
	const int32 Index = SlotToIndex(Slot);
	if (!EquippedItemIDs.IsValidIndex(Index) || !EquippedInstanceIds.IsValidIndex(Index)) return false;

	const FInventoryItem* Def = FindDef(ItemID);
	if (!Def) return false;
	if (!(Def->ItemType == EItemType::Equipment && Def->GearSlot == Slot)) return false;
	
	if (!InstanceId.IsValid())
	{
		for (const auto& E : Bag.Entries)
		{
			if (E.ItemID == ItemID && E.Quantity == 1)
			{
				bool bAlreadyEquipped = false;
				for (const FGuid& G : EquippedInstanceIds)
				{
					if (G == E.InstanceId) { bAlreadyEquipped = true; break; }
				}
				if (!bAlreadyEquipped)
				{
					InstanceId = E.InstanceId;
					break;
				}
			}
		}
		if (!InstanceId.IsValid()) return false; 
	}
	else
	{
		bool bFound = false;
		for (const auto& E : Bag.Entries)
		{
			if (E.InstanceId == InstanceId && E.ItemID == ItemID && E.Quantity == 1)
			{
				bFound = true; break;
			}
		}
		if (!bFound) return false;
		
		for (const FGuid& G : EquippedInstanceIds)
		{
			if (G == InstanceId) return false;
		}
	}
	
	TArray<int32> OldIDs = EquippedItemIDs;
	const FGuid OldGuid  = EquippedInstanceIds[Index];
	
	EquippedItemIDs[Index]     = ItemID;
	EquippedInstanceIds[Index] = InstanceId;

	BroadcastEquipChangeFromArrays(OldIDs, EquippedItemIDs);
	OnEquipmentChangedInstance.Broadcast(Slot, OldGuid, InstanceId);

	return true;
}

bool USignetInventoryComponent::TryUnequip_Internal(EGearSlot Slot)
{
	const int32 Index = SlotToIndex(Slot);
	if (!EquippedItemIDs.IsValidIndex(Index) || !EquippedInstanceIds.IsValidIndex(Index)) return false;

	TArray<int32> OldIDs = EquippedItemIDs;
	const FGuid OldGuid  = EquippedInstanceIds[Index];

	EquippedItemIDs[Index]     = 0;
	EquippedInstanceIds[Index] = FGuid();

	BroadcastEquipChangeFromArrays(OldIDs, EquippedItemIDs);
	OnEquipmentChangedInstance.Broadcast(Slot, OldGuid, FGuid());

	return true;
}