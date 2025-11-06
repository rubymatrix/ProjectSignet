#include "SignetItemRowEntryWidget.h"

#include "SignetItemRowEntry.h"
#include "Components/TextBlock.h"

void USignetItemRowEntryWidget::NativeOnListItemObjectSet(UObject* ListItemObject)
{
	USignetItemRowEntry* RowData = Cast<USignetItemRowEntry>(ListItemObject);
	if (RowData == nullptr)
	{
		return;
	}

	Item = RowData;
	
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

	if (RowNameText)
	{
		RowNameText->SetText(FText::FromName(RowData->RowName));
	}

	if (ItemIdText)
	{
		ItemIdText->SetText(FText::AsNumber(RowData->ItemID));
	}
}