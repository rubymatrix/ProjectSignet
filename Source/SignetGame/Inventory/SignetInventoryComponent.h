// Copyright Red Lotus Games, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "InvList.h"
#include "InventoryTypes.h"
#include "Components/ActorComponent.h"
#include "SignetInventoryComponent.generated.h"

enum class ERace : uint8;
enum class EJob : uint8;
struct FSignetSavedInventory;
class USignetItemDataSubsystem;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnBagEntryAdded,   int32, ItemID, uint16, Quantity);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnBagEntryChanged, int32, ItemID, uint16, NewQuantity);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam (FOnBagEntryRemoved, int32, ItemID);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnEquipmentChanged,         EGearSlot, Slot, int32, OldItemID, int32, NewItemID);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnEquipmentChangedInstance, EGearSlot, Slot, FGuid, OldInstance, FGuid, NewInstance);

constexpr bool ENABLE_INVENTORY_DEBUG = false;

UCLASS(ClassGroup=(Signet), meta=(BlueprintSpawnableComponent))
class SIGNETGAME_API USignetInventoryComponent : public UActorComponent
{
	GENERATED_BODY()
public:
	USignetInventoryComponent();

	virtual void BeginPlay() override;

	UFUNCTION(Exec) void InvAdd(int32 ItemID, int32 Qty) { if (GetOwner()->HasAuthority()) { int32 L=0; TryAddItem_Internal(ItemID, Qty, L); } }
	UFUNCTION(Exec) void InvEquipID(EGearSlot Slot, int32 ItemID) { if (GetOwner()->HasAuthority()) { TryEquip_Internal(Slot, ItemID, FGuid()); } }
	UFUNCTION(Exec) void InvUnequip(EGearSlot Slot) { if (GetOwner()->HasAuthority()) { TryUnequip_Internal(Slot); } }
	UFUNCTION(Exec) void InvPrint();
	
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
	UPROPERTY(BlueprintAssignable, Category="Inventory|Events") FOnBagEntryAdded OnBagEntryAdded;
	UPROPERTY(BlueprintAssignable, Category="Inventory|Events") FOnBagEntryChanged OnBagEntryChanged;
	UPROPERTY(BlueprintAssignable, Category="Inventory|Events") FOnBagEntryRemoved OnBagEntryRemoved;
	UPROPERTY(BlueprintAssignable, Category="Inventory|Events") FOnEquipmentChanged OnEquipmentChanged;
	UPROPERTY(BlueprintAssignable, Category="Inventory|Events") FOnEquipmentChangedInstance OnEquipmentChangedInstance;

	
	UFUNCTION(BlueprintPure, Category="Inventory")
	int32 GetTotalQuantity(int32 ItemID) const;
	UFUNCTION(BlueprintPure, Category="Inventory")
	int32 GetEquippedItemID(EGearSlot Slot) const;
	UFUNCTION(BlueprintPure, Category="Inventory")
	void GetEquippedItemDefBP(EGearSlot Slot, FInventoryItem& OutItem, bool& bFound) const;
	UFUNCTION(BlueprintPure, Category="Inventory")
	FGuid GetEquippedInstance(EGearSlot Slot) const;

	UFUNCTION(BlueprintPure, Category="Inventory")
	FGameplayTag GetEquippedWeaponSkill(EGearSlot Slot = EGearSlot::Main) const;

	UFUNCTION(BlueprintPure, Category="Inventory")
	bool IsEquipped(const FGuid& InstanceId) const;
	
	UFUNCTION(Server, Reliable) void ServerTryAddItem(int32 ItemID, int32 Quantity);
	UFUNCTION(Server, Reliable) void ServerConsumeItem(int32 ItemID, int32 Quantity);
	UFUNCTION(Server, Reliable) void ServerEquipItemByItemID(EGearSlot Slot, int32 ItemID);
	UFUNCTION(Server, Reliable) void ServerEquipFromInstance(EGearSlot Slot, const FGuid& InstanceId);
	UFUNCTION(Server, Reliable) void ServerUnequip(EGearSlot Slot);

protected:

	UPROPERTY(Replicated)
	FInvList Bag;
	
	UPROPERTY(ReplicatedUsing=OnRep_EquippedIDs)
	TArray<int32> EquippedItemIDs;
	
	UPROPERTY(ReplicatedUsing=OnRep_EquippedInstances)
	TArray<FGuid> EquippedInstanceIds;
	
	TKeyValuePair<int32, FGameplayTag> CachedMainSkill = {0, FGameplayTag::EmptyTag};
	TKeyValuePair<int32, FGameplayTag> CachedOffSkill = {0, FGameplayTag::EmptyTag};
	
	UFUNCTION()
	void OnRep_EquippedIDs(const TArray<int32>& Old);
	UFUNCTION()
	void OnRep_EquippedInstances(const TArray<FGuid>& Old);
	
	bool TryAddItem_Internal(int32 ItemID, int32 Quantity, int32& OutLeftover);
	bool TryRemoveItem_Internal(int32 ItemID, int32 Quantity); 
	bool TryEquip_Internal(EGearSlot Slot, int32 ItemID, FGuid InstanceId);
	bool TryUnequip_Internal(EGearSlot Slot);
	
	const FInventoryItem* FindDef(int32 ItemID) const; 
	void BroadcastEquipChangeFromArrays(const TArray<int32>& OldIDs, const TArray<int32>& NewIDs);

public:

	FInvList& GetBag() { return Bag; }
	static FORCEINLINE int32 SlotToIndex(EGearSlot Slot) { return static_cast<int32>(Slot); }
	static constexpr int32 SlotCount() { return static_cast<int32>(EGearSlot::Feet) + 1; }
	void CaptureToSave(FSignetSavedInventory& Out) const;
	void LoadFromSave(const FSignetSavedInventory& In);
	void ValidateEquipment(ERace PlayerRace, EJob PlayerJob);

private:

	void ServerInventoryMutated();
	UFUNCTION(Client, Reliable)
	void ClientSyncInventory(const FSignetSavedInventory& Snapshot);

protected:
	
	UFUNCTION() void HandleBagAdded(int32 ItemID, uint16 Qty);
	UFUNCTION() void HandleBagChanged(int32 ItemID, uint16 NewQty);
	UFUNCTION() void HandleBagRemoved(int32 ItemID);
	UFUNCTION() void HandleEquipID(EGearSlot Slot, int32 OldID, int32 NewID);
	UFUNCTION() void HandleEquipInstance(EGearSlot Slot, FGuid OldG, FGuid NewG);
};
