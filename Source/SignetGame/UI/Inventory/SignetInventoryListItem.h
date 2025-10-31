#pragma once
#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "SignetGame/Inventory/InventoryTypes.h"
#include "SignetInventoryListItem.generated.h"


UCLASS(BlueprintType)
class SIGNETGAME_API USignetInventoryListItem : public UObject
{
	GENERATED_BODY()
public:

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FGuid  InstanceId;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int ItemID = 0;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	EItemQuality ItemQuality = EItemQuality::Normal;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	EItemType ItemType = EItemType::None;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	EGearSlot GearSlot = EGearSlot::None;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int Quantity = 0;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FText DisplayName;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UTexture2D> Icon = nullptr;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bIsEquipped = false;
};