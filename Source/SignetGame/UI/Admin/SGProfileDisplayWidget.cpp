#include "SGProfileDisplayWidget.h"

#include "Components/TextBlock.h"
#include "SignetGame/Save/SignetSaveGame.h"
#include "SignetGame/Save/SignetSaveSubsystem.h"

void USGProfileDisplayWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void USGProfileDisplayWidget::SetDataFromSave()
{
	auto* GI = GetGameInstance();
	if (!GI) return;

	auto* SaveSystem = GI->GetSubsystem<USignetSaveSubsystem>();
	if (!SaveSystem) return;

	const auto* Save = SaveSystem->GetSave();
	if (!Save) return;

	const auto* Progress = SaveSystem->GetOrAddJob(Save->Profile.CurrentJob);
	if (!Progress) return;

	auto JobName = GetJobName(Save->Profile.CurrentJob);
	auto JobLevel = Progress->Level;
	auto ExpAmount = Progress->CurrentExp;
	auto JobLine = FString::Printf(TEXT("Lv. %i %s"), JobLevel, *JobName);
	auto ExpLine = FString::Printf(TEXT("%lld / 3000"), ExpAmount);

	PlayerName->SetText(FText::FromString(Save->Profile.DisplayName));
	Job->SetText(FText::FromString(JobLine));
	EXP->SetText(FText::FromString(ExpLine));
	
	AP->SetText(GetText(Save->Wallet.AbilityPoints));
	SP->SetText(GetText(Save->Wallet.SkillPoints));
	BeastmenSeals->SetText(GetText(Save->Wallet.BeastmenSeals));
	Gil->SetText(GetText(Save->Wallet.Gil));
}

FText USGProfileDisplayWidget::GetText(int Amount)
{
	return FText::FromString(FString::FromInt(Amount));
}
