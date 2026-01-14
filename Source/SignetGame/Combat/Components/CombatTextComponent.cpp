// Copyright Red Lotus Games, All Rights Reserved.


#include "CombatTextComponent.h"

#include "SignetGame/UI/CombatText/CombatTextWidget.h"

UCombatTextComponent::UCombatTextComponent()
{
	static ConstructorHelpers::FClassFinder<UUserWidget> FCTClassFinder(TEXT("/Game/Signet/UI/CombatText/WBP_CombatText.WBP_CombatText"));
	CombatTextWidgetClass = FCTClassFinder.Class;
}

void UCombatTextComponent::AddDamageText(float Amount, bool bCritical)
{
	if (!CombatTextWidgetClass) return;
	
	FNumberFormattingOptions Options;
	Options.MaximumFractionalDigits = 0;
	Options.UseGrouping = true;

	FCombatTextData Data;
	Data.CombatTextType = ECombatTextType::Damage;
	Data.Text = FText::AsNumber(Amount, &Options);
	Data.TextColor = bCritical ? CriticalTextColor : DefaultTextColor;
	Data.bIsCritical = bCritical;

	if (APlayerController* PC = GetWorld()->GetFirstPlayerController())
	{
		UCombatTextWidget::CreateCombatText(PC, CombatTextWidgetClass, GetOwner(), Data);
	}
}

void UCombatTextComponent::AddHealingText(float Amount, bool bCritical)
{
	if (!CombatTextWidgetClass) return;

	FNumberFormattingOptions Options;
	Options.MaximumFractionalDigits = 0;
	Options.UseGrouping = true;

	FCombatTextData Data;
	Data.CombatTextType = ECombatTextType::Healing;
	Data.Text = FText::AsNumber(Amount, &Options);
	Data.TextColor = HealingTextColor;
	Data.bIsCritical = bCritical;

	if (APlayerController* PC = GetWorld()->GetFirstPlayerController())
	{
		UCombatTextWidget::CreateCombatText(PC, CombatTextWidgetClass, GetOwner(), Data);
	}
}

void UCombatTextComponent::AddStatusEffectText(const FString& Text)
{
	if (APlayerController* PC = GetWorld()->GetFirstPlayerController())
	{
		FCombatTextData Data;
		Data.CombatTextType = ECombatTextType::StatusEffect;
		Data.Text = FText::FromString(Text);
		Data.TextColor = StatusEffectTextColor;
		UCombatTextWidget::CreateCombatText(PC, CombatTextWidgetClass, GetOwner(), Data);
	}
}
