#include "SignetItemTableEditorWidget.h"

#include "EditorUtilityWidgetComponents.h"
#include "SignetItemRowEntry.h"
#include "UObject/Package.h"
#include "UObject/SoftObjectPath.h"
#include "Engine/Texture2D.h"

USignetItemRowEntry* USignetItemRowEntry::MakeItemRowEntry(UObject* WorldContextObject, FName InRowName, int32 InItemID, const FString& InDisplayName)
{
	if (WorldContextObject == nullptr)
	{
		return nullptr;
	}

	USignetItemRowEntry* Entry = NewObject<USignetItemRowEntry>(WorldContextObject);
	if (Entry)
	{
		Entry->RowName = InRowName;
		Entry->ItemID = InItemID;
		Entry->DisplayName = InDisplayName;
	}

	return Entry;
}

void USignetItemTableEditorWidget::NativeConstruct()
{
	Super::NativeConstruct();

	
	if (RowSearchText)
	{
		RowSearchText->OnTextChanged.AddDynamic(this, &ThisClass::RowSearchTextChanged);
		RowSearchText->OnTextCommitted.AddDynamic(this, &ThisClass::RowSearchTextCommited);
	}

	if (ItemSearchText)
	{
		ItemSearchText->OnTextChanged.AddDynamic(this, &ThisClass::ItemSearchTextChanged);
		ItemSearchText->OnTextCommitted.AddDynamic(this, &ThisClass::ItemSearchTextCommited);
	}

	if (TargetTable)
	{
		RefreshFilteredRows();
	}

}

void USignetItemTableEditorWidget::RefreshFilteredRows()
{
	FilteredRows.Reset();

	if (TargetTable == nullptr)
	{
		return;
	}

	// Get all row names
	const TArray<FName> AllRowNames = TargetTable->GetRowNames();

	for (const FName& RowName : AllRowNames)
	{
		FItemRow FoundRow;
		if (!TryGetItemRow(RowName, FoundRow))
		{
			continue;
		}

		// Check filters
		bool bMatches = false;

		// Row name filter
		if (!RowNameFilter.IsEmpty())
		{
			const FString RowNameStr = RowName.ToString();
			if (RowNameStr.Contains(RowNameFilter, ESearchCase::IgnoreCase, ESearchDir::FromStart))
			{
				bMatches = true;
			}
		}

		// Item name filter
		if (!ItemNameFilter.IsEmpty())
		{
			if (FoundRow.Item.En.Contains(ItemNameFilter, ESearchCase::IgnoreCase, ESearchDir::FromStart) ||
				FoundRow.Item.Enl.Contains(ItemNameFilter, ESearchCase::IgnoreCase, ESearchDir::FromStart))
			{
				bMatches = true;
			}
		}

		// If no filters, show all
		if (RowNameFilter.IsEmpty() && ItemNameFilter.IsEmpty())
		{
			bMatches = true;
		}

		if (bMatches)
		{
			if (USignetItemRowEntry* Entry = BuildEntryForRow(RowName, FoundRow))
			{
				FilteredRows.Add(Entry);
			}
		}
	}
	
	// At this point your BP widget can "Refresh" its ListView or has it bound

	if (ResultsListView)
	{
		ResultsListView->ClearListItems();
		ResultsListView->SetListItems(FilteredRows);
	}
}

void USignetItemTableEditorWidget::SelectRow(USignetItemRowEntry* Entry)
{
	if (Entry == nullptr || TargetTable == nullptr)
	{
		return;
	}

	FItemRow FoundRow;
	if (TryGetItemRow(Entry->RowName, FoundRow))
	{
		CurrentRowName = Entry->RowName;
		CurrentRow = FoundRow;
	}
}

bool USignetItemTableEditorWidget::CommitCurrentRow()
{
	if (TargetTable == nullptr)
	{
		return false;
	}

	if (CurrentRowName.IsNone())
	{
		return false;
	}

	// Make the asset dirty so it can be saved
	TargetTable->Modify();

	// Overwrite or add
	FItemRow* Existing = TargetTable->FindRow<FItemRow>(CurrentRowName, TEXT("Signet CommitCurrentRow"));
	if (Existing != nullptr)
	{
		*Existing = CurrentRow;
	}
	else
	{
		TargetTable->AddRow(CurrentRowName, CurrentRow);
	}

	TargetTable->MarkPackageDirty();

	// Refresh the visible list so changes (name, etc) show up
	RefreshFilteredRows();

	return true;
}

void USignetItemTableEditorWidget::SetIconFromAsset(UTexture2D* InTexture)
{
	if (InTexture == nullptr)
	{
		return;
	}

	// We only modify our local copy; user still has to hit "Commit"
	CurrentRow.Item.Icon = InTexture;
}

void USignetItemTableEditorWidget::SetMeshFromAsset(UObject* InAsset, const FString& PrefixToRemove)
{
	if (InAsset == nullptr)
	{
		return;
	}

	const FString Trimmed = MakeTrimmedPath(InAsset, PrefixToRemove);
	CurrentRow.Item.MeshPath = Trimmed;
}

USignetItemRowEntry* USignetItemTableEditorWidget::BuildEntryForRow(const FName& RowName, const FItemRow& Row)
{
	// Use the widget itself as the outer so GC doesn’t eat it
	USignetItemRowEntry* Entry = NewObject<USignetItemRowEntry>(this);
	if (Entry)
	{
		Entry->RowName = RowName;
		Entry->ItemID = Row.Item.ItemID;

		// Prefer En, then Enl, then RowName
		if (!Row.Item.En.IsEmpty())
		{
			Entry->DisplayName = Row.Item.En;
		}
		else if (!Row.Item.Enl.IsEmpty())
		{
			Entry->DisplayName = Row.Item.Enl;
		}
		else
		{
			Entry->DisplayName = RowName.ToString();
		}
	}

	return Entry;
}

bool USignetItemTableEditorWidget::TryGetItemRow(const FName& RowName, FItemRow& OutRow) const
{
	if (TargetTable == nullptr)
	{
		return false;
	}

	if (FItemRow* Found = TargetTable->FindRow<FItemRow>(RowName, TEXT("Signet TryGetItemRow")))
	{
		OutRow = *Found;
		return true;
	}

	return false;
}

FString USignetItemTableEditorWidget::MakeTrimmedPath(UObject* Asset, const FString& PrefixToRemove) const
{
	const FString FullPath = Asset->GetPathName();

	// If user gave an explicit prefix, try that first
	if (!PrefixToRemove.IsEmpty() && FullPath.StartsWith(PrefixToRemove))
	{
		return FullPath.Mid(PrefixToRemove.Len());
	}

	// Otherwise, remove default /Game/
	constexpr TCHAR GamePrefix[] = TEXT("/Game/");
	if (FullPath.StartsWith(GamePrefix))
	{
		return FullPath.Mid(FCString::Strlen(GamePrefix));
	}

	// fallback: return full path
	return FullPath;
}

void USignetItemTableEditorWidget::DebounceRefreshRows()
{
	const auto World = GetWorld();
	if (World == nullptr) return;

	if (!World->GetTimerManager().IsTimerActive(SearchDebounceTimer))
	{
		World->GetTimerManager().SetTimer(SearchDebounceTimer, [&]()
		{
			RefreshFilteredRows();
		}, 0.1f, false);
	}
}

void USignetItemTableEditorWidget::RowSearchTextChanged(const FText& NewText)
{
	RowNameFilter = NewText.ToString();
	DebounceRefreshRows();
}

void USignetItemTableEditorWidget::RowSearchTextCommited(const FText& NewText, ETextCommit::Type CommitType)
{
	RowNameFilter = NewText.ToString();
	DebounceRefreshRows();
}

void USignetItemTableEditorWidget::ItemSearchTextChanged(const FText& NewText)
{
	ItemNameFilter = NewText.ToString();
	DebounceRefreshRows();
}

void USignetItemTableEditorWidget::ItemSearchTextCommited(const FText& NewText, ETextCommit::Type CommitType)
{
	ItemNameFilter = NewText.ToString();
	DebounceRefreshRows();
}
