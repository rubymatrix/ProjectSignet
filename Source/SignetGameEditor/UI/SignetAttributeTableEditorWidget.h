#pragma once

#include "CoreMinimal.h"
#include "EditorUtilityWidget.h"
#include "SignetAttributeTableEditorWidget.generated.h"

class UDataTable;
class UEditorUtilityListView;
class UEditorUtilityEditableTextBox;
class UObjectPropertyEntryBox;
class USignetAttributeRowEntry;
struct FAssetData;

/**
 * Editor widget for building and organizing AttributeMetaData tables.
 * Make a BP Editor Utility Widget from this and bind your controls to the exposed properties.
 */
UCLASS()
class SIGNETGAMEEDITOR_API USignetAttributeTableEditorWidget : public UEditorUtilityWidget
{
	GENERATED_BODY()

public:

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UEditorUtilityEditableTextBox> AttributeSearchText;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta=(BindWidget))
	TObjectPtr<UEditorUtilityListView> AvailableListView;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta=(BindWidget))
	TObjectPtr<UEditorUtilityListView> SelectedListView;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Signet|AttributeTable")
	TObjectPtr<UDataTable> TargetTable = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Signet|AttributeTable|Filter")
	FString AttributeSearchFilter;

	UPROPERTY(BlueprintReadOnly, Category="Signet|AttributeTable")
	TArray<TObjectPtr<USignetAttributeRowEntry>> AvailableAttributes;

	UPROPERTY(BlueprintReadOnly, Category="Signet|AttributeTable")
	TArray<TObjectPtr<USignetAttributeRowEntry>> SelectedAttributes;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Signet|AttributeTable|Defaults")
	float NewBaseValue = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Signet|AttributeTable|Defaults")
	float NewMinValue = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Signet|AttributeTable|Defaults")
	float NewMaxValue = 0.0f;

public:

	virtual void NativeConstruct() override;

	UFUNCTION(BlueprintCallable, Category="Signet|AttributeTable")
	void RefreshAvailableAttributes();

	UFUNCTION(BlueprintCallable, Category="Signet|AttributeTable")
	void AddAttributeToSelection(USignetAttributeRowEntry* Entry, float InBaseValue, float InMinValue, float InMaxValue);

	UFUNCTION(BlueprintCallable, Category="Signet|AttributeTable")
	void RemoveAttributeFromSelection(USignetAttributeRowEntry* Entry);

	UFUNCTION(BlueprintCallable, Category="Signet|AttributeTable")
	void ClearSelection();

	UFUNCTION(BlueprintCallable, Category="Signet|AttributeTable")
	void LoadSelectionFromTable();

	UFUNCTION(BlueprintCallable, Category="Signet|AttributeTable")
	bool MoveSelectedAttribute(int32 FromIndex, int32 ToIndex);

	UFUNCTION(BlueprintCallable, Category="Signet|AttributeTable")
	bool CommitSelectionToTable();

	UFUNCTION(BlueprintCallable, Category="Signet|AttributeTable")
	bool RemoveRowFromTable(USignetAttributeRowEntry* Entry);

	UFUNCTION(BlueprintCallable, Category="Signet|AttributeTable|Assets")
	bool PromptForNewAttributeTablePath(FString& OutAssetPath);

	UFUNCTION(BlueprintCallable, Category="Signet|AttributeTable|Assets")
	UDataTable* CreateAttributeMetaTableAtPath(const FString& AssetPath);

protected:

	void BuildAttributeCatalog();
	bool IsCatalogBuilt() const;
	FString MakeHumanReadableAttributeName(const FString& RawName) const;
	USignetAttributeRowEntry* BuildEntry(const FName& RowName, const FString& DisplayName, const FString& AttributeName, const FString& SetName, float InBaseValue, float InMinValue, float InMaxValue);

	FTimerHandle SearchDebounceTimer;
	void DebounceRefreshAvailable();
	UFUNCTION() void AttributeSearchTextChanged(const FText& NewText);
	UFUNCTION() void AttributeSearchTextCommited(const FText& NewText, ETextCommit::Type CommitType);
	UFUNCTION() void HandleTargetTablePicked(const FAssetData& AssetData);

	struct FAttributeDefinition
	{
		FName RowName;
		FString DisplayName;
		FString AttributeName;
		FString SetName;
	};

	TArray<FAttributeDefinition> Catalog;
};
