#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Blueprint/IUserObjectListEntry.h"
#include "SignetAttributeRowEntryWidget.generated.h"

class UTextBlock;
class UBorder;
class UEditableText;
class USignetAttributeRowEntry;

UCLASS()
class SIGNETGAMEEDITOR_API USignetAttributeRowEntryWidget : public UUserWidget, public IUserObjectListEntry
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadOnly, Category="Signet|AttributeTable")
	TObjectPtr<USignetAttributeRowEntry> Item;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> NameText;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> KeyText;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> SetText;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta=(BindWidget))
	TObjectPtr<UBorder> SelectionBorder;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta=(BindWidget))
	TObjectPtr<UEditableText> BaseValueText;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta=(BindWidget))
	TObjectPtr<UEditableText> MinValueText;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta=(BindWidget))
	TObjectPtr<UEditableText> MaxValueText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Signet|AttributeTable|Selection")
	FLinearColor SelectedBorderColor = FLinearColor(0.1f, 0.6f, 1.0f, 0.9f);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Signet|AttributeTable|Selection")
	FLinearColor UnselectedBorderColor = FLinearColor(0.12f, 0.12f, 0.12f, 0.8f);

	virtual void NativeOnListItemObjectSet(UObject* ListItemObject) override;

	virtual void NativeOnItemSelectionChanged(bool bIsSelected) override;

private:

	void ApplySelectionVisuals(bool bIsSelected);
	void SyncEntryValuesToWidgets();
	void ApplyTextToValue(const FText& InText, float& OutValue);

	UFUNCTION() void HandleBaseValueChanged(const FText& NewText);
	UFUNCTION() void HandleMinValueChanged(const FText& NewText);
	UFUNCTION() void HandleMaxValueChanged(const FText& NewText);
};
