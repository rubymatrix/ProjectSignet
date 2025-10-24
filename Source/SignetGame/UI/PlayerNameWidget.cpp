// Copyright 2025, SignetGame

#include "PlayerNameWidget.h"
#include "Components/TextBlock.h"
#include "Components/EditableTextBox.h"
#include "SignetGame/Save/SignetSaveSubsystem.h"

void UPlayerNameWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (Label)
	{
		Label->SetText(FText::FromString(TEXT("Name:")));
	}

	if (NameTextBox)
	{
		NameTextBox->OnTextCommitted.AddDynamic(this, &UPlayerNameWidget::HandleNameCommitted);
	}
}

void UPlayerNameWidget::SetPlayerName(const FString& NewName)
{
	if (NameTextBox)
	{
		NameTextBox->SetText(FText::FromString(NewName));
	}
}

FString UPlayerNameWidget::GetPlayerName() const
{
	return NameTextBox ? NameTextBox->GetText().ToString() : FString();
}

void UPlayerNameWidget::HandleNameCommitted(const FText& Text, ETextCommit::Type CommitMethod)
{
	if (const auto Save = GetGameInstance()->GetSubsystem<USignetSaveSubsystem>())
	{
		Save->SetPlayerName(Text.ToString());
	}
	OnPlayerNameChanged.Broadcast(Text.ToString());
}
