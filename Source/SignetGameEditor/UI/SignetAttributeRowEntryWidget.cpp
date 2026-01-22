#include "SignetAttributeRowEntryWidget.h"

#include "SignetAttributeRowEntry.h"
#include "Components/Border.h"
#include "Components/EditableText.h"
#include "Components/TextBlock.h"

void USignetAttributeRowEntryWidget::NativeOnListItemObjectSet(UObject* ListItemObject)
{
	USignetAttributeRowEntry* RowData = Cast<USignetAttributeRowEntry>(ListItemObject);
	if (RowData == nullptr)
	{
		return;
	}

	Item = RowData;

	ApplySelectionVisuals(false);
	SyncEntryValuesToWidgets();
	
	if (NameText)
	{
		if (!RowData->DisplayName.IsEmpty())
		{
			NameText->SetText(FText::FromString(RowData->DisplayName));
		}
		else
		{
			NameText->SetText(FText::FromName(RowData->RowName));
		}
	}

	if (KeyText)
	{
		KeyText->SetText(FText::FromName(RowData->RowName));
	}

	if (SetText)
	{
		SetText->SetText(FText::FromString(RowData->SetName));
	}

	if (BaseValueText)
	{
		BaseValueText->OnTextChanged.RemoveDynamic(this, &ThisClass::HandleBaseValueChanged);
		BaseValueText->OnTextChanged.AddDynamic(this, &ThisClass::HandleBaseValueChanged);
	}

	if (MinValueText)
	{
		MinValueText->OnTextChanged.RemoveDynamic(this, &ThisClass::HandleMinValueChanged);
		MinValueText->OnTextChanged.AddDynamic(this, &ThisClass::HandleMinValueChanged);
	}

	if (MaxValueText)
	{
		MaxValueText->OnTextChanged.RemoveDynamic(this, &ThisClass::HandleMaxValueChanged);
		MaxValueText->OnTextChanged.AddDynamic(this, &ThisClass::HandleMaxValueChanged);
	}
}

void USignetAttributeRowEntryWidget::NativeOnItemSelectionChanged(bool bIsSelected)
{
	ApplySelectionVisuals(bIsSelected);
}

void USignetAttributeRowEntryWidget::ApplySelectionVisuals(bool bIsSelected)
{
	if (SelectionBorder)
	{
		SelectionBorder->SetBrushColor(bIsSelected ? SelectedBorderColor : UnselectedBorderColor);
	}
}

void USignetAttributeRowEntryWidget::SyncEntryValuesToWidgets()
{
	if (Item == nullptr)
	{
		return;
	}

	if (BaseValueText)
	{
		BaseValueText->SetText(FText::AsNumber(Item->BaseValue));
	}

	if (MinValueText)
	{
		MinValueText->SetText(FText::AsNumber(Item->MinValue));
	}

	if (MaxValueText)
	{
		MaxValueText->SetText(FText::AsNumber(Item->MaxValue));
	}
}

void USignetAttributeRowEntryWidget::ApplyTextToValue(const FText& InText, float& OutValue)
{
	const FString TextValue = InText.ToString();
	const float Parsed = FCString::Atof(*TextValue);
	OutValue = Parsed;
}

void USignetAttributeRowEntryWidget::HandleBaseValueChanged(const FText& NewText)
{
	if (Item == nullptr)
	{
		return;
	}

	ApplyTextToValue(NewText, Item->BaseValue);
}

void USignetAttributeRowEntryWidget::HandleMinValueChanged(const FText& NewText)
{
	if (Item == nullptr)
	{
		return;
	}

	ApplyTextToValue(NewText, Item->MinValue);
}

void USignetAttributeRowEntryWidget::HandleMaxValueChanged(const FText& NewText)
{
	if (Item == nullptr)
	{
		return;
	}

	ApplyTextToValue(NewText, Item->MaxValue);
}
