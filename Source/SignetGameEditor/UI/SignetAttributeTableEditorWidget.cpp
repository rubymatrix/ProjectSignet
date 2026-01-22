#include "SignetAttributeTableEditorWidget.h"

#include "AttributeSet.h"
#include "EditorUtilityWidgetComponents.h"
#include "SignetAttributeRowEntry.h"
#include "SignetGame/Abilities/Attributes/SignetCombatAttributeSet.h"
#include "SignetGame/Abilities/Attributes/SignetPrimaryAttributeSet.h"
#include "SignetGame/Abilities/Attributes/SignetSkillAttributeSet.h"
#include "AssetToolsModule.h"
#include "ContentBrowserModule.h"
#include "Factories/DataTableFactory.h"
#include "AssetRegistry/AssetRegistryModule.h"
#include "AssetRegistry/AssetData.h"
#include "IContentBrowserSingleton.h"
#include "Misc/PackageName.h"
#include "UObject/Package.h"

USignetAttributeRowEntry* USignetAttributeRowEntry::MakeAttributeRowEntry(UObject* WorldContextObject, FName InRowName, const FString& InDisplayName, const FString& InAttributeName, const FString& InSetName, float InBaseValue, float InMinValue, float InMaxValue)
{
	if (WorldContextObject == nullptr)
	{
		return nullptr;
	}

	USignetAttributeRowEntry* Entry = NewObject<USignetAttributeRowEntry>(WorldContextObject);
	if (Entry)
	{
		Entry->RowName = InRowName;
		Entry->DisplayName = InDisplayName;
		Entry->AttributeName = InAttributeName;
		Entry->SetName = InSetName;
		Entry->BaseValue = InBaseValue;
		Entry->MinValue = InMinValue;
		Entry->MaxValue = InMaxValue;
	}

	return Entry;
}

void USignetAttributeTableEditorWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (AttributeSearchText)
	{
		AttributeSearchText->OnTextChanged.AddDynamic(this, &ThisClass::AttributeSearchTextChanged);
		AttributeSearchText->OnTextCommitted.AddDynamic(this, &ThisClass::AttributeSearchTextCommited);
	}

	BuildAttributeCatalog();
	RefreshAvailableAttributes();
}

void USignetAttributeTableEditorWidget::RefreshAvailableAttributes()
{
	AvailableAttributes.Reset();

	if (!IsCatalogBuilt())
	{
		BuildAttributeCatalog();
	}

	for (const FAttributeDefinition& Def : Catalog)
	{
		bool bMatches = false;

		if (!AttributeSearchFilter.IsEmpty())
		{
			if (Def.DisplayName.Contains(AttributeSearchFilter, ESearchCase::IgnoreCase, ESearchDir::FromStart) ||
				Def.AttributeName.Contains(AttributeSearchFilter, ESearchCase::IgnoreCase, ESearchDir::FromStart) ||
				Def.RowName.ToString().Contains(AttributeSearchFilter, ESearchCase::IgnoreCase, ESearchDir::FromStart))
			{
				bMatches = true;
			}
		}
		else
		{
			bMatches = true;
		}

		if (bMatches)
		{
			if (USignetAttributeRowEntry* Entry = BuildEntry(Def.RowName, Def.DisplayName, Def.AttributeName, Def.SetName, NewBaseValue, NewMinValue, NewMaxValue))
			{
				AvailableAttributes.Add(Entry);
			}
		}
	}

	if (AvailableListView)
	{
		AvailableListView->ClearListItems();
		AvailableListView->SetListItems(AvailableAttributes);
	}
}

void USignetAttributeTableEditorWidget::AddAttributeToSelection(USignetAttributeRowEntry* Entry, float InBaseValue, float InMinValue, float InMaxValue)
{
	if (Entry == nullptr)
	{
		return;
	}

	const int32 ExistingIndex = SelectedAttributes.IndexOfByPredicate([Entry](const USignetAttributeRowEntry* Existing)
	{
		return Existing && Existing->RowName == Entry->RowName;
	});

	if (ExistingIndex != INDEX_NONE)
	{
		USignetAttributeRowEntry* Existing = SelectedAttributes[ExistingIndex];
		if (Existing)
		{
			Existing->BaseValue = InBaseValue;
			Existing->MinValue = InMinValue;
			Existing->MaxValue = InMaxValue;
		}

		return;
	}

	USignetAttributeRowEntry* Clone = BuildEntry(Entry->RowName, Entry->DisplayName, Entry->AttributeName, Entry->SetName, InBaseValue, InMinValue, InMaxValue);
	if (Clone)
	{
		SelectedAttributes.Add(Clone);
	}

	if (SelectedListView)
	{
		SelectedListView->ClearListItems();
		SelectedListView->SetListItems(SelectedAttributes);
	}
}

void USignetAttributeTableEditorWidget::RemoveAttributeFromSelection(USignetAttributeRowEntry* Entry)
{
	if (Entry == nullptr)
	{
		return;
	}

	SelectedAttributes.RemoveAll([Entry](const USignetAttributeRowEntry* Existing)
	{
		return Existing && Existing->RowName == Entry->RowName;
	});

	if (SelectedListView)
	{
		SelectedListView->ClearListItems();
		SelectedListView->SetListItems(SelectedAttributes);
	}
}

void USignetAttributeTableEditorWidget::ClearSelection()
{
	SelectedAttributes.Reset();

	if (SelectedListView)
	{
		SelectedListView->ClearListItems();
	}
}

void USignetAttributeTableEditorWidget::LoadSelectionFromTable()
{
	SelectedAttributes.Reset();

	if (SelectedListView)
	{
		SelectedListView->ClearListItems();
	}

	if (TargetTable == nullptr)
	{
		return;
	}

	if (!IsCatalogBuilt())
	{
		BuildAttributeCatalog();
	}

	const TMap<FName, uint8*>& RowMap = TargetTable->GetRowMap();
	if (RowMap.Num() == 0)
	{
		return;
	}

	for (const TPair<FName, uint8*>& RowPair : RowMap)
	{
		const FAttributeMetaData* Row = TargetTable->FindRow<FAttributeMetaData>(RowPair.Key, TEXT("Signet LoadSelectionFromTable"));
		if (Row == nullptr)
		{
			continue;
		}

		const FName RowName = RowPair.Key;
		const FAttributeDefinition* Def = Catalog.FindByPredicate([&RowName](const FAttributeDefinition& Item)
		{
			return Item.RowName == RowName;
		});

		FString DisplayName;
		FString AttributeName;
		FString SetName;

		if (Def)
		{
			DisplayName = Def->DisplayName;
			AttributeName = Def->AttributeName;
			SetName = Def->SetName;
		}
		else
		{
			const FString RowKey = RowName.ToString();
			RowKey.Split(TEXT("."), &SetName, &AttributeName);

			if (AttributeName.IsEmpty())
			{
				AttributeName = RowKey;
			}

			if (SetName.IsEmpty())
			{
				SetName = TEXT("Unknown");
			}

			DisplayName = MakeHumanReadableAttributeName(AttributeName);
		}

		USignetAttributeRowEntry* Entry = BuildEntry(RowName, DisplayName, AttributeName, SetName, Row->BaseValue, Row->MinValue, Row->MaxValue);
		if (Entry)
		{
			SelectedAttributes.Add(Entry);
		}
	}

	if (SelectedListView)
	{
		SelectedListView->ClearListItems();
		SelectedListView->SetListItems(SelectedAttributes);
	}
}

bool USignetAttributeTableEditorWidget::MoveSelectedAttribute(int32 FromIndex, int32 ToIndex)
{
	if (!SelectedAttributes.IsValidIndex(FromIndex) || !SelectedAttributes.IsValidIndex(ToIndex))
	{
		return false;
	}

	if (FromIndex == ToIndex)
	{
		return false;
	}

	USignetAttributeRowEntry* Moved = SelectedAttributes[FromIndex];
	SelectedAttributes.RemoveAt(FromIndex);
	SelectedAttributes.Insert(Moved, ToIndex);

	if (SelectedListView)
	{
		SelectedListView->ClearListItems();
		SelectedListView->SetListItems(SelectedAttributes);
	}

	return true;
}

bool USignetAttributeTableEditorWidget::CommitSelectionToTable()
{
	if (TargetTable == nullptr)
	{
		return false;
	}

	if (SelectedAttributes.Num() == 0)
	{
		return false;
	}

	TargetTable->Modify();

	for (const USignetAttributeRowEntry* Entry : SelectedAttributes)
	{
		if (Entry == nullptr)
		{
			continue;
		}

		const FName RowName = Entry->RowName;
		FAttributeMetaData* Existing = TargetTable->FindRow<FAttributeMetaData>(RowName, TEXT("Signet CommitSelectionToTable"));
		if (Existing == nullptr)
		{
			FAttributeMetaData NewRow;
			NewRow.BaseValue = Entry->BaseValue;
			NewRow.MinValue = Entry->MinValue;
			NewRow.MaxValue = Entry->MaxValue;
			TargetTable->AddRow(RowName, NewRow);
		}
		else
		{
			Existing->BaseValue = Entry->BaseValue;
			Existing->MinValue = Entry->MinValue;
			Existing->MaxValue = Entry->MaxValue;
		}
	}

	TargetTable->MarkPackageDirty();
	return true;
}

bool USignetAttributeTableEditorWidget::RemoveRowFromTable(USignetAttributeRowEntry* Entry)
{
	if (TargetTable == nullptr || Entry == nullptr)
	{
		return false;
	}

	if (!TargetTable->GetRowMap().Contains(Entry->RowName))
	{
		return false;
	}

	TargetTable->Modify();
	TargetTable->RemoveRow(Entry->RowName);
	TargetTable->MarkPackageDirty();
	return true;
}

bool USignetAttributeTableEditorWidget::PromptForNewAttributeTablePath(FString& OutAssetPath)
{
	FContentBrowserModule& ContentBrowserModule = FModuleManager::LoadModuleChecked<FContentBrowserModule>("ContentBrowser");
	FSaveAssetDialogConfig SaveConfig;
	SaveConfig.DialogTitleOverride = FText::FromString(TEXT("Create Attribute Meta Data Table"));
	SaveConfig.DefaultPath = TEXT("/Game");
	SaveConfig.DefaultAssetName = TEXT("DT_AttributeMeta");
	SaveConfig.AssetClassNames.Add(UDataTable::StaticClass()->GetClassPathName());

	const FString SavePath = ContentBrowserModule.Get().CreateModalSaveAssetDialog(SaveConfig);
	if (SavePath.IsEmpty())
	{
		return false;
	}

	OutAssetPath = SavePath;
	return true;
}

UDataTable* USignetAttributeTableEditorWidget::CreateAttributeMetaTableAtPath(const FString& AssetPath)
{
	if (AssetPath.IsEmpty())
	{
		return nullptr;
	}

	if (!FPackageName::IsValidObjectPath(AssetPath))
	{
		return nullptr;
	}

	const FString PackagePath = FPackageName::GetLongPackagePath(AssetPath);
	const FString AssetName = FPackageName::GetShortName(AssetPath);

	if (PackagePath.IsEmpty() || AssetName.IsEmpty())
	{
		return nullptr;
	}

	UDataTableFactory* Factory = NewObject<UDataTableFactory>();
	Factory->Struct = FAttributeMetaData::StaticStruct();

	FAssetToolsModule& AssetToolsModule = FModuleManager::LoadModuleChecked<FAssetToolsModule>("AssetTools");
	UObject* NewAsset = AssetToolsModule.Get().CreateAsset(AssetName, PackagePath, UDataTable::StaticClass(), Factory);

	UDataTable* DataTable = Cast<UDataTable>(NewAsset);
	if (DataTable)
	{
		FAssetRegistryModule::AssetCreated(DataTable);
		DataTable->MarkPackageDirty();
	}

	return DataTable;
}

void USignetAttributeTableEditorWidget::BuildAttributeCatalog()
{
	Catalog.Reset();

	TArray<UClass*> AttributeSetClasses;
	AttributeSetClasses.Add(USignetPrimaryAttributeSet::StaticClass());
	AttributeSetClasses.Add(USignetCombatAttributeSet::StaticClass());
	AttributeSetClasses.Add(USignetSkillAttributeSet::StaticClass());

	for (UClass* AttributeSetClass : AttributeSetClasses)
	{
		if (AttributeSetClass == nullptr)
		{
			continue;
		}

		FString SetName = AttributeSetClass->GetName();
		if (SetName.StartsWith(TEXT("U")))
		{
			SetName.RightChopInline(1);
		}

		for (TFieldIterator<FProperty> It(AttributeSetClass, EFieldIteratorFlags::IncludeSuper); It; ++It)
		{
			const FStructProperty* StructProp = CastField<FStructProperty>(*It);
			if (StructProp == nullptr)
			{
				continue;
			}

			if (StructProp->Struct != FGameplayAttributeData::StaticStruct())
			{
				continue;
			}

			const FString AttributeName = StructProp->GetName();
			const FString DisplayName = MakeHumanReadableAttributeName(AttributeName);
			const FString RowKey = FString::Printf(TEXT("%s.%s"), *SetName, *AttributeName);

			FAttributeDefinition Def;
			Def.RowName = FName(*RowKey);
			Def.DisplayName = DisplayName;
			Def.AttributeName = AttributeName;
			Def.SetName = SetName;
			Catalog.Add(Def);
		}
	}
}

bool USignetAttributeTableEditorWidget::IsCatalogBuilt() const
{
	return Catalog.Num() > 0;
}

FString USignetAttributeTableEditorWidget::MakeHumanReadableAttributeName(const FString& RawName) const
{
	if (RawName.IsEmpty())
	{
		return RawName;
	}

	FString Result;
	Result.Reserve(RawName.Len() + 4);

	for (int32 Index = 0; Index < RawName.Len(); ++Index)
	{
		const TCHAR Current = RawName[Index];
		const TCHAR Prev = Index > 0 ? RawName[Index - 1] : TEXT('\0');
		const TCHAR Next = Index + 1 < RawName.Len() ? RawName[Index + 1] : TEXT('\0');

		const bool bIsUpper = FChar::IsUpper(Current);
		const bool bPrevIsUpper = FChar::IsUpper(Prev);
		const bool bPrevIsLower = FChar::IsLower(Prev);
		const bool bNextIsLower = FChar::IsLower(Next);

		const bool bShouldInsertSpace = Index > 0 && ((bIsUpper && bPrevIsLower) || (bIsUpper && bPrevIsUpper && bNextIsLower));
		if (bShouldInsertSpace)
		{
			Result.AppendChar(TEXT(' '));
		}

		Result.AppendChar(Current);
	}

	return Result;
}

USignetAttributeRowEntry* USignetAttributeTableEditorWidget::BuildEntry(const FName& RowName, const FString& DisplayName, const FString& AttributeName, const FString& SetName, float InBaseValue, float InMinValue, float InMaxValue)
{
	USignetAttributeRowEntry* Entry = NewObject<USignetAttributeRowEntry>(this);
	if (Entry)
	{
		Entry->RowName = RowName;
		Entry->DisplayName = DisplayName;
		Entry->AttributeName = AttributeName;
		Entry->SetName = SetName;
		Entry->BaseValue = InBaseValue;
		Entry->MinValue = InMinValue;
		Entry->MaxValue = InMaxValue;
	}

	return Entry;
}

void USignetAttributeTableEditorWidget::DebounceRefreshAvailable()
{
	const UWorld* World = GetWorld();
	if (World == nullptr)
	{
		return;
	}

	if (!World->GetTimerManager().IsTimerActive(SearchDebounceTimer))
	{
		World->GetTimerManager().SetTimer(SearchDebounceTimer, [&]()
		{
			RefreshAvailableAttributes();
		}, 0.1f, false);
	}
}

void USignetAttributeTableEditorWidget::AttributeSearchTextChanged(const FText& NewText)
{
	AttributeSearchFilter = NewText.ToString();
	DebounceRefreshAvailable();
}

void USignetAttributeTableEditorWidget::AttributeSearchTextCommited(const FText& NewText, ETextCommit::Type CommitType)
{
	AttributeSearchFilter = NewText.ToString();
	DebounceRefreshAvailable();
}

void USignetAttributeTableEditorWidget::HandleTargetTablePicked(const FAssetData& AssetData)
{
	TargetTable = Cast<UDataTable>(AssetData.GetAsset());
	LoadSelectionFromTable();
}
