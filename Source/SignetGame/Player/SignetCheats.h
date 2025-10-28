#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CheatManager.h"

#include "SignetCheats.generated.h"


UCLASS()
class SIGNETGAME_API USignetCheatManager : public UCheatManager
{
	GENERATED_BODY()
public:
	// Add items to bag (server authoritative)
	UFUNCTION(exec) void AddItem(int32 ItemID, int32 Qty);

	// Equip by item id (auto-pick a free instance)
	UFUNCTION(exec) void EquipID(int32 Slot, int32 ItemID);

	// Unequip a slot
	UFUNCTION(exec) void Unequip(int32 Slot);

	// Print state
	UFUNCTION(exec) void Inv();
};