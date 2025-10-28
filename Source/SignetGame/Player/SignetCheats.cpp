// SignetCheats.cpp
#include "SignetCheats.h"
#include "GameFramework/PlayerController.h"
#include "SignetGame/Inventory/SignetInventoryComponent.h"

static USignetInventoryComponent* GetInvCmpFromCheat(USignetCheatManager* CM)
{
	if (!CM) return nullptr;
	if (APlayerController* PC = CM->GetOuterAPlayerController())
	{
		APawn* Pawn = PC->GetPawn();
		if (Pawn) return Pawn->FindComponentByClass<USignetInventoryComponent>();
	}
	return nullptr;
}

void USignetCheatManager::AddItem(int32 ItemID, int32 Qty)
{
	if (auto* Inv = GetInvCmpFromCheat(this))
	{
		// Call the server RPC so it works in multiplayer
		Inv->ServerTryAddItem(ItemID, Qty);
	}
}

void USignetCheatManager::EquipID(int32 Slot, int32 ItemID)
{
	if (auto* Inv = GetInvCmpFromCheat(this))
	{
		Inv->ServerEquipItemByItemID(static_cast<EGearSlot>(Slot), ItemID);
	}
}

void USignetCheatManager::Unequip(int32 Slot)
{
	if (auto* Inv = GetInvCmpFromCheat(this))
	{
		Inv->ServerUnequip(static_cast<EGearSlot>(Slot));
	}
}

void USignetCheatManager::Inv()
{
	if (auto* Inv = GetInvCmpFromCheat(this))
	{
		Inv->InvPrint(); // your debug print helper
	}
}
