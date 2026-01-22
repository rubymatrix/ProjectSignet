#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "EditorUtilityWidget.h"
#include "SignetGame/Inventory/InventoryTypes.h"
#include "SignetItemTableEditorWidget.generated.h"

class UEditorUtilityListView;
class UEditorUtilityEditableTextBox;
class USignetItemRowEntry;
/**
 * Editor widget for browsing / filtering / editing FItemRow in a UDataTable.
 * Make a BP Editor Utility Widget from this and bind your controls to the exposed properties.
 */
UCLASS()
class SIGNETGAMEEDITOR_API USignetItemTableEditorWidget : public UEditorUtilityWidget
{
	GENERATED_BODY()

public:

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UEditorUtilityEditableTextBox> RowSearchText;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UEditorUtilityEditableTextBox> ItemSearchText;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UEditorUtilityListView> ResultsListView;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Signet|ItemTable")
	TObjectPtr<UDataTable> TargetTable = nullptr;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Signet|ItemTable|Filter")
	FString RowNameFilter;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Signet|ItemTable|Filter")
	FString ItemNameFilter;
	
	UPROPERTY(BlueprintReadOnly, Category="Signet|ItemTable")
	TArray<TObjectPtr<USignetItemRowEntry>> FilteredRows;
	
	UPROPERTY(BlueprintReadOnly, Category="Signet|ItemTable")
	FName CurrentRowName;
	
	UPROPERTY(BlueprintReadOnly, Category="Signet|ItemTable")
	FItemRow CurrentRow;

public:

	virtual void NativeConstruct() override;
	
	UFUNCTION(BlueprintCallable, Category="Signet|ItemTable")
	void RefreshFilteredRows();
	
	UFUNCTION(BlueprintCallable, Category="Signet|ItemTable")
	void SelectRow(USignetItemRowEntry* Entry);
	
	UFUNCTION(BlueprintCallable, Category="Signet|ItemTable")
	bool CommitCurrentRow();
	
	UFUNCTION(BlueprintCallable, Category="Signet|ItemTable|Assets")
	void SetIconFromAsset(UTexture2D* InTexture);

	/**
	 * Set CurrentRow.Item.MeshPath from an asset, trimming a prefix
	 * Example:
	 *   Asset path:  /Game/Characters/Modular/Hume/Male/Parts/Range/_Decazoom_MkXI/_Decazoom_MkXI._Decazoom_MkXI
	 *   Prefix:      /Game/Characters/Modular/Hume/Male/
	 *   Result:      Parts/Range/_Decazoom_MkXI/_Decazoom_MkXI._Decazoom_MkXI
	 */
	UFUNCTION(BlueprintCallable, Category="Signet|ItemTable|Assets")
	void SetMeshFromAsset(UObject* InAsset, const FString& PrefixToRemove);

	// Reads server-base/sql/item_mods.sql and applies modifiers for the given ItemID.
	UFUNCTION(BlueprintCallable, Category="Signet|ItemTable|Mods")
	bool ImportItemModsFromSql(int32 ItemID, bool bOverwriteExisting = true, bool bCommit = true);

protected:

	/** Utility: build one entry from a row */
	USignetItemRowEntry* BuildEntryForRow(const FName& RowName, const FItemRow& Row);

	/** Utility: tries to get a typed row from the table */
	bool TryGetItemRow(const FName& RowName, FItemRow& OutRow) const;

	/** Utility: find a row name by ItemID */
	bool TryFindRowNameByItemID(int32 ItemID, FName& OutRowName) const;

	/** Utility: trim path */
	FString MakeTrimmedPath(UObject* Asset, const FString& PrefixToRemove) const;

	FTimerHandle SearchDebounceTimer;
	void DebounceRefreshRows();
	UFUNCTION() void RowSearchTextChanged(const FText& NewText);
	UFUNCTION() void RowSearchTextCommited(const FText& NewText, ETextCommit::Type CommitType);
	UFUNCTION() void ItemSearchTextChanged(const FText& NewText);
	UFUNCTION() void ItemSearchTextCommited(const FText& NewText, ETextCommit::Type CommitType);
	
};
