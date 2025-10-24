#include "SGProfileEditorWidget.h"

#include "Chaos/Deformable/MuscleActivationConstraints.h"
#include "Components/EditableTextBox.h"
#include "Components/ComboBoxString.h"
#include "Components/SpinBox.h"
#include "Components/Button.h"
#include "UObject/UnrealType.h"
#include "Engine/Engine.h"
#include "Kismet/GameplayStatics.h"
#include "SignetGame/Player/SignetPlayerController.h"
#include "SignetGame/Save/SignetSaveSubsystem.h"
#include "SignetGame/Util/Logging.h"

void USGProfileEditorWidget::NativeConstruct()
{
	Super::NativeConstruct();

	// Resolve UEnum pointers (update paths/names if needed)
	UEnum* JobEnum = StaticEnum<EJob>();
	UEnum* CurrencyEnum = StaticEnum<ECurrencyType>();

	checkf(JobEnum, TEXT("StaticEnum<EJob>() not found. Include the header with EJob or update the enum type."));
	checkf(CurrencyEnum, TEXT("StaticEnum<ECurrencyType>() not found. Include the header with ECurrencyType."));

	// Populate combos
	if (JobCombo)
	{
		JobCombo->ClearOptions();
		PopulateEnumCombo(JobCombo, JobEnum, JobLabelToValue);
	}

	if (CurrencyCombo)
	{
		CurrencyCombo->ClearOptions();
		PopulateEnumCombo(CurrencyCombo, CurrencyEnum, CurrencyLabelToValue);
	}

	// Hook up buttons
	if (SaveButton)
	{
		SaveButton->OnClicked.Clear();
		SaveButton->OnClicked.AddDynamic(this, &USGProfileEditorWidget::HandleSavePressed);
	}

	if (AddCurrencyButton)
	{
		AddCurrencyButton->OnClicked.Clear();
		AddCurrencyButton->OnClicked.AddDynamic(this, &USGProfileEditorWidget::HandleAddCurrencyPressed);
	}

	// Reasonable defaults
	if (LevelSpinBox)
	{
		LevelSpinBox->SetMinValue(1);
		LevelSpinBox->SetMaxValue(99); // tweak if your cap differs
		LevelSpinBox->SetValue(1);
	}

	if (ExpSpinBox)
	{
		ExpSpinBox->SetMinValue(0);
		ExpSpinBox->SetMaxValue(9'999'999); // tweak to your curve
		ExpSpinBox->SetValue(0);
	}

	if (CurrencyAmountSpinBox)
	{
		CurrencyAmountSpinBox->SetMinValue(1);
		CurrencyAmountSpinBox->SetMaxValue(9'999'999);
		CurrencyAmountSpinBox->SetValue(100);
	}
}

void USGProfileEditorWidget::InitializeFromData(const USignetSaveGame* Save)
{
	if (Save == nullptr)
	{
		UE_LOG(LogSignet, Warning, TEXT("USGProfileEditorWidget::InitializeFromData: Save data is null."));
		return;
	}

	auto Profile = Save->Profile;
	auto Progress = Save->Jobs.Find(Profile.CurrentJob);
	
	if (NameTextBox)
	{
		NameTextBox->SetText(FText::FromString(Profile.DisplayName));
	}

	// Select Job in combo
	if (UEnum* JobEnum = StaticEnum<EJob>())
	{
		const int64 JobValue = static_cast<int64>(Profile.CurrentJob);
		const FString Label = MakeEnumEntryLabel(JobEnum, JobValue);
		if (JobCombo && JobLabelToValue.Contains(Label))
		{
			JobCombo->SetSelectedOption(Label);
		}
	}

	if (LevelSpinBox)
	{
		LevelSpinBox->SetValue(Progress->Level);
	}

	if (ExpSpinBox)
	{
		ExpSpinBox->SetValue(Progress->CurrentExp);
	}
}

void USGProfileEditorWidget::PopulateEnumCombo(UComboBoxString* Combo, UEnum* Enum, TMap<FString, int64>& OutLabelToValue)
{
	OutLabelToValue.Reset();
	if (!Combo || !Enum) return;

	const int32 Num = Enum->NumEnums();
	for (int32 Index = 0; Index < Num; ++Index)
	{
		const int64 Value = Enum->GetValueByIndex(Index);
		const FString Label = MakeEnumEntryLabel(Enum, Value);

		if (!Label.IsEmpty())
		{
			Combo->AddOption(Label);
			OutLabelToValue.Add(Label, Value);
		}
	}

	// Select first option by default
	if (Combo->GetOptionCount() > 0)
	{
		Combo->SetSelectedIndex(0);
	}
}

FString USGProfileEditorWidget::MakeEnumEntryLabel(UEnum* Enum, int64 Value)
{
	if (!Enum) return FString();

	// Prefer DisplayName (Editor/UMG friendly). Fallback to plain name.
#if WITH_EDITOR
	const FText DisplayName = Enum->GetDisplayNameTextByValue(Value);
	if (!DisplayName.IsEmpty())
	{
		return DisplayName.ToString();
	}
#endif

	const FName Name = Enum->GetNameByValue(Value);
	// Strip Namespace EnumName:: if present
	FString Clean = Name.ToString();
	int32 ScopeIdx;
	if (Clean.FindChar(':', ScopeIdx))
	{
		Clean = Clean.RightChop(ScopeIdx + 1);
	}
	return Clean;
}

void USGProfileEditorWidget::HandleSavePressed()
{
	USignetSaveSubsystem* SaveSystem = nullptr;
	if (const auto Inst = GetGameInstance())
	{
		SaveSystem = Inst->GetSubsystem<USignetSaveSubsystem>();
	}

	if (SaveSystem == nullptr)
	{
		UE_LOG(LogSignet, Warning, TEXT("USGProfileEditorWidget::HandleSavePressed: SaveSubsystem not found."));
		return;
	}

	auto Save = SaveSystem->GetSave();
	auto Profile = SaveSystem->GetPlayerProfile();
	
	// Assign New Name
	Save->Profile.DisplayName = NameTextBox ? NameTextBox->GetText().ToString() : TEXT("Soldier"); 

	if (EJob JobValue; TryGetSelectedJob(JobValue))
	{
		Save->Profile.CurrentJob = JobValue;
	}

	if (auto Progress = SaveSystem->GetOrAddJob(Save->Profile.CurrentJob))
	{
		Progress->Level = LevelSpinBox ? static_cast<int32>(LevelSpinBox->GetValue()) : 1;
		Progress->CurrentExp = ExpSpinBox ? static_cast<int32>(ExpSpinBox->GetValue()) : 0;
	}
	
	SaveSystem->SaveNow();

	if (auto* W = GetWorld())
	{
		if (auto* PC = Cast<ASignetPlayerController>(W->GetFirstPlayerController()))
		{
			PC->ClientRequestProfileSnapshot();
		}
	}

	OnProfileSaved.Broadcast(Profile);
}

void USGProfileEditorWidget::HandleAddCurrencyPressed()
{
	FCurrencyAdjustment Adj;
	ECurrencyType Curr;
	if (TryGetSelectedCurrency(Curr))
	{
		Adj.Currency = Curr;
	}
	Adj.Amount = CurrencyAmountSpinBox ? static_cast<int32>(CurrencyAmountSpinBox->GetValue()) : 0;

	USignetSaveSubsystem* SaveSystem = nullptr;
	if (const auto Inst = GetGameInstance())
	{
		SaveSystem = Inst->GetSubsystem<USignetSaveSubsystem>();
	}

	if (SaveSystem == nullptr)
	{
		UE_LOG(LogSignet, Warning, TEXT("USGProfileEditorWidget::HandleAddCurrencyPressed: SaveSubsystem not found."));
		return;
	}

	SaveSystem->AddCurrency(Adj.Currency, Adj.Amount, true);
	OnCurrencyAdded.Broadcast(Adj);
}

bool USGProfileEditorWidget::TryGetSelectedJob(EJob& OutJob) const
{
	if (!JobCombo) return false;
	const FString Selected = JobCombo->GetSelectedOption();
	if (const int64* Found = JobLabelToValue.Find(Selected))
	{
		OutJob = static_cast<EJob>(*Found);
		return true;
	}
	return false;
}

bool USGProfileEditorWidget::TryGetSelectedCurrency(ECurrencyType& OutCurrency) const
{
	if (!CurrencyCombo) return false;
	const FString Selected = CurrencyCombo->GetSelectedOption();
	if (const int64* Found = CurrencyLabelToValue.Find(Selected))
	{
		OutCurrency = static_cast<ECurrencyType>(*Found);
		return true;
	}
	return false;
}
