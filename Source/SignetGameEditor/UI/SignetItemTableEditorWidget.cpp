#include "SignetItemTableEditorWidget.h"

#include "EditorUtilityWidgetComponents.h"
#include "SignetItemRowEntry.h"
#include "SignetGame/Abilities/Attributes/SignetCombatAttributeSet.h"
#include "SignetGame/Abilities/Attributes/SignetPrimaryAttributeSet.h"
#include "SignetGame/Abilities/Attributes/SignetSkillAttributeSet.h"
#include "Misc/FileHelper.h"
#include "Misc/Paths.h"
#include "Internationalization/Regex.h"
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

namespace
{
	struct FModMapping
	{
		FGameplayAttribute Attribute;
		bool bIsPercent = false;
		float Scale = 1.0f;
	};

	static TMap<int32, FModMapping> BuildModMappings()
	{
		TMap<int32, FModMapping> Map;

		// Primary stats
		Map.Add(1, { USignetCombatAttributeSet::GetDefenseAttribute(), false, 1.0f }); // DEF
		Map.Add(2, { USignetPrimaryAttributeSet::GetMaxHPAttribute(), false, 1.0f }); // HP
		Map.Add(3, { USignetPrimaryAttributeSet::GetMaxHPAttribute(), true, 0.01f }); // HPP
		Map.Add(5, { USignetPrimaryAttributeSet::GetMaxMPAttribute(), false, 1.0f }); // MP
		Map.Add(6, { USignetPrimaryAttributeSet::GetMaxMPAttribute(), true, 0.01f }); // MPP

		Map.Add(8,  { USignetPrimaryAttributeSet::GetSTRAttribute(), false, 1.0f }); // STR
		Map.Add(9,  { USignetPrimaryAttributeSet::GetDEXAttribute(), false, 1.0f }); // DEX
		Map.Add(10, { USignetPrimaryAttributeSet::GetVITAttribute(), false, 1.0f }); // VIT
		Map.Add(11, { USignetPrimaryAttributeSet::GetAGIAttribute(), false, 1.0f }); // AGI
		Map.Add(12, { USignetPrimaryAttributeSet::GetINTAttribute(), false, 1.0f }); // INT
		Map.Add(13, { USignetPrimaryAttributeSet::GetMNDAttribute(), false, 1.0f }); // MND
		Map.Add(14, { USignetPrimaryAttributeSet::GetCHRAttribute(), false, 1.0f }); // CHR

		// Combat stats
		Map.Add(23, { USignetCombatAttributeSet::GetAttackAttribute(), false, 1.0f }); // ATT
		Map.Add(24, { USignetCombatAttributeSet::GetRangedAttackAttribute(), false, 1.0f }); // RATT
		Map.Add(25, { USignetCombatAttributeSet::GetAccuracyAttribute(), false, 1.0f }); // ACC
		Map.Add(26, { USignetCombatAttributeSet::GetRangedAccuracyAttribute(), false, 1.0f }); // RACC
		Map.Add(28, { USignetCombatAttributeSet::GetMagicAttackAttribute(), false, 1.0f }); // MATT
		Map.Add(29, { USignetCombatAttributeSet::GetMagicDefenseAttribute(), false, 1.0f }); // MDEF
		Map.Add(30, { USignetCombatAttributeSet::GetMagicAccuracyAttribute(), false, 1.0f }); // MACC
		Map.Add(31, { USignetCombatAttributeSet::GetMagicEvasionAttribute(), false, 1.0f }); // MEVA

		Map.Add(62, { USignetCombatAttributeSet::GetAttackAttribute(), true, 0.01f }); // ATTP
		Map.Add(63, { USignetCombatAttributeSet::GetDefenseAttribute(), true, 0.01f }); // DEFP
		Map.Add(66, { USignetCombatAttributeSet::GetRangedAttackAttribute(), true, 0.01f }); // RATTP

		Map.Add(68, { USignetCombatAttributeSet::GetEvasionAttribute(), false, 1.0f }); // EVA
		Map.Add(69, { USignetCombatAttributeSet::GetRangedDefenseAttribute(), false, 1.0f }); // RDEF
		Map.Add(70, { USignetCombatAttributeSet::GetRangedEvasionAttribute(), false, 1.0f }); // REVA

		// Weapon skills
		Map.Add(80, { USignetSkillAttributeSet::GetHandToHandAttribute(), false, 1.0f }); // HTH
		Map.Add(81, { USignetSkillAttributeSet::GetDaggerAttribute(), false, 1.0f }); // DAGGER
		Map.Add(82, { USignetSkillAttributeSet::GetSwordAttribute(), false, 1.0f }); // SWORD
		Map.Add(83, { USignetSkillAttributeSet::GetGreatSwordAttribute(), false, 1.0f }); // GSWORD
		Map.Add(84, { USignetSkillAttributeSet::GetAxeAttribute(), false, 1.0f }); // AXE
		Map.Add(85, { USignetSkillAttributeSet::GetGreatAxeAttribute(), false, 1.0f }); // GAXE
		Map.Add(86, { USignetSkillAttributeSet::GetScytheAttribute(), false, 1.0f }); // SCYTHE
		Map.Add(87, { USignetSkillAttributeSet::GetPolearmAttribute(), false, 1.0f }); // POLEARM
		Map.Add(88, { USignetSkillAttributeSet::GetKatanaAttribute(), false, 1.0f }); // KATANA
		Map.Add(89, { USignetSkillAttributeSet::GetGreatKatanaAttribute(), false, 1.0f }); // GKATANA
		Map.Add(90, { USignetSkillAttributeSet::GetClubAttribute(), false, 1.0f }); // CLUB
		Map.Add(91, { USignetSkillAttributeSet::GetStaffAttribute(), false, 1.0f }); // STAFF
		Map.Add(104, { USignetSkillAttributeSet::GetArcheryAttribute(), false, 1.0f }); // ARCHERY
		Map.Add(105, { USignetSkillAttributeSet::GetMarksmanshipAttribute(), false, 1.0f }); // MARKSMAN
		Map.Add(106, { USignetSkillAttributeSet::GetThrowingAttribute(), false, 1.0f }); // THROW
		Map.Add(107, { USignetSkillAttributeSet::GetGuardAttribute(), false, 1.0f }); // GUARD
		Map.Add(108, { USignetSkillAttributeSet::GetEvasionAttribute(), false, 1.0f }); // EVASION
		Map.Add(109, { USignetSkillAttributeSet::GetShieldAttribute(), false, 1.0f }); // SHIELD
		Map.Add(110, { USignetSkillAttributeSet::GetParryingAttribute(), false, 1.0f }); // PARRY

		// Magic skills
		Map.Add(111, { USignetSkillAttributeSet::GetDivineAttribute(), false, 1.0f }); // DIVINE
		Map.Add(112, { USignetSkillAttributeSet::GetHealingAttribute(), false, 1.0f }); // HEALING
		Map.Add(113, { USignetSkillAttributeSet::GetEnhancingAttribute(), false, 1.0f }); // ENHANCE
		Map.Add(114, { USignetSkillAttributeSet::GetEnfeeblingAttribute(), false, 1.0f }); // ENFEEBLE
		Map.Add(115, { USignetSkillAttributeSet::GetElementalAttribute(), false, 1.0f }); // ELEM
		Map.Add(116, { USignetSkillAttributeSet::GetDarkAttribute(), false, 1.0f }); // DARK
		Map.Add(117, { USignetSkillAttributeSet::GetSummoningAttribute(), false, 1.0f }); // SUMMONING
		Map.Add(118, { USignetSkillAttributeSet::GetNinjutsuAttribute(), false, 1.0f }); // NINJUTSU
		Map.Add(119, { USignetSkillAttributeSet::GetSingingAttribute(), false, 1.0f }); // SINGING
		Map.Add(120, { USignetSkillAttributeSet::GetStringInstrumentAttribute(), false, 1.0f }); // STRING
		Map.Add(121, { USignetSkillAttributeSet::GetWindInstrumentAttribute(), false, 1.0f }); // WIND
		Map.Add(122, { USignetSkillAttributeSet::GetBlueAttribute(), false, 1.0f }); // BLUE
		Map.Add(123, { USignetSkillAttributeSet::GetGeomancyAttribute(), false, 1.0f }); // GEOMANCY
		Map.Add(124, { USignetSkillAttributeSet::GetHandbellAttribute(), false, 1.0f }); // HANDBELL

		return Map;
	}
}

bool USignetItemTableEditorWidget::ImportItemModsFromSql(int32 ItemID, bool bOverwriteExisting, bool bCommit)
{
	if (TargetTable == nullptr || ItemID <= 0)
	{
		return false;
	}

	FName RowName;
	if (!TryFindRowNameByItemID(ItemID, RowName))
	{
		UE_LOG(LogTemp, Warning, TEXT("ImportItemModsFromSql: ItemID %d not found in table."), ItemID);
		return false;
	}

	FItemRow FoundRow;
	if (!TryGetItemRow(RowName, FoundRow))
	{
		return false;
	}

	const FString SqlPath = FPaths::ConvertRelativePathToFull(FPaths::ProjectDir() / TEXT("server-base/sql/item_mods.sql"));
	FString SqlText;
	if (!FFileHelper::LoadFileToString(SqlText, *SqlPath))
	{
		UE_LOG(LogTemp, Warning, TEXT("ImportItemModsFromSql: Failed to read %s"), *SqlPath);
		return false;
	}

	const TMap<int32, FModMapping> ModMap = BuildModMappings();
	TMap<FString, FItemStatModifier> CombinedMods;

	const FRegexPattern InsertPattern(TEXT("INSERT\\s+INTO\\s+`item_mods`\\s+VALUES\\s*\\((\\d+),\\s*(\\d+),\\s*([-\\d]+)\\)"));
	FRegexMatcher Matcher(InsertPattern, SqlText);

	while (Matcher.FindNext())
	{
		const int32 FoundItemId = FCString::Atoi(*Matcher.GetCaptureGroup(1));
		if (FoundItemId != ItemID)
		{
			continue;
		}

		const int32 ModId = FCString::Atoi(*Matcher.GetCaptureGroup(2));
		const int32 RawValue = FCString::Atoi(*Matcher.GetCaptureGroup(3));

		const FModMapping* Mapping = ModMap.Find(ModId);
		if (!Mapping || !Mapping->Attribute.IsValid())
		{
			UE_LOG(LogTemp, Warning, TEXT("ImportItemModsFromSql: Unsupported ModId %d for item %d"), ModId, ItemID);
			continue;
		}

		FItemStatModifier NewMod;
		NewMod.Attribute = Mapping->Attribute;
		NewMod.bIsPercent = Mapping->bIsPercent;
		NewMod.Magnitude = static_cast<float>(RawValue) * Mapping->Scale;

		const FString Key = FString::Printf(TEXT("%s_%d"), *Mapping->Attribute.GetName(), Mapping->bIsPercent ? 1 : 0);
		if (FItemStatModifier* Existing = CombinedMods.Find(Key))
		{
			Existing->Magnitude += NewMod.Magnitude;
		}
		else
		{
			CombinedMods.Add(Key, NewMod);
		}
	}

	TArray<FItemStatModifier> FinalMods;
	CombinedMods.GenerateValueArray(FinalMods);

	CurrentRowName = RowName;
	CurrentRow = FoundRow;

	if (bOverwriteExisting)
	{
		CurrentRow.Item.StatModifiers = FinalMods;
	}
	else
	{
		CurrentRow.Item.StatModifiers.Append(FinalMods);
	}

	if (bCommit)
	{
		return CommitCurrentRow();
	}

	return true;
}

USignetItemRowEntry* USignetItemTableEditorWidget::BuildEntryForRow(const FName& RowName, const FItemRow& Row)
{
	// Use the widget itself as the outer so GC doesn't eat it
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

bool USignetItemTableEditorWidget::TryFindRowNameByItemID(int32 ItemID, FName& OutRowName) const
{
	if (TargetTable == nullptr)
	{
		return false;
	}

	const TMap<FName, uint8*>& RowMap = TargetTable->GetRowMap();
	for (const TPair<FName, uint8*>& Pair : RowMap)
	{
		const FItemRow* Row = reinterpret_cast<const FItemRow*>(Pair.Value);
		if (Row && Row->Item.ItemID == ItemID)
		{
			OutRowName = Pair.Key;
			return true;
		}
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
