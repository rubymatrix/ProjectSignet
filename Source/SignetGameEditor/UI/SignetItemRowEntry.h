
#pragma once

#include "CoreMinimal.h"
#include "SignetItemRowEntry.generated.h"

/**
* Simple view model for a single row in the item table.
 * This makes it easy to bind a ListView to it in UMG.
 */
UCLASS(BlueprintType)
class SIGNETGAMEEDITOR_API USignetItemRowEntry : public UObject
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadOnly, Category="Signet|ItemTable")
	FName RowName;

	UPROPERTY(BlueprintReadOnly, Category="Signet|ItemTable")
	int32 ItemID = 0;

	UPROPERTY(BlueprintReadOnly, Category="Signet|ItemTable")
	FString DisplayName;

	// Helper to make an entry
	UFUNCTION(BlueprintCallable, Category="Signet|ItemTable", meta=(WorldContext="WorldContextObject"))
	static USignetItemRowEntry* MakeItemRowEntry(UObject* WorldContextObject, FName InRowName, int32 InItemID, const FString& InDisplayName);
};
