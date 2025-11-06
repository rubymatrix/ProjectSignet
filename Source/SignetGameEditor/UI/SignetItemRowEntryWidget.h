#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Blueprint/IUserObjectListEntry.h"
#include "SignetItemRowEntryWidget.generated.h"

class UTextBlock;
class USignetItemRowEntry;

/**
 * Simple row widget for the item table ListView.
 * Shows RowName, ItemID, and DisplayName.
 */
UCLASS()
class SIGNETGAMEEDITOR_API USignetItemRowEntryWidget : public UUserWidget, public IUserObjectListEntry
{
	GENERATED_BODY()

public:

	/** Text block for the main label (eg. DisplayName) */
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> NameText = nullptr;

	/** Optional text block for the row name (DT key) */
	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<UTextBlock> RowNameText = nullptr;

	/** Optional text block for the item id */
	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<UTextBlock> ItemIdText = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Transient)
	TWeakObjectPtr<USignetItemRowEntry> Item;

protected:

	// IUserObjectListEntry
	virtual void NativeOnListItemObjectSet(UObject* ListItemObject) override;
};