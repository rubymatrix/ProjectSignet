// Copyright Red Lotus Games, All Rights Reserved.


#include "CombatTextComponent.h"

#include "SignetGame/UI/CombatText/CombatTextWidget.h"
#include "SignetGame/Player/SignetPlayerCharacter.h"
#include "SignetGame/Save/SignetSaveSubsystem.h"

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
		Data.ScreenOffset = StatusEffectScreenOffset;
		UCombatTextWidget::CreateCombatText(PC, StatusEffectWidgetClass, GetOwner(), Data);
	}
}

void UCombatTextComponent::AddSkillUpText(const FGameplayTag& SkillTag, float Amount)
{
	if (APlayerController* PC = GetWorld()->GetFirstPlayerController())
	{
		FCombatTextData Data;
		Data.CombatTextType = ECombatTextType::SkillUp;
		Data.TextColor = SkillUpTextColor;
		Data.ScreenOffset = SkillUpScreenOffset;

		FString SkillName = SkillTag.GetTagName().ToString();
		SkillName.RemoveFromStart(TEXT("Data.Skill."));

		float NewSkillRank = 0.0f;
		float OldSkillRank = 0.0f;
		if (const ASignetPlayerCharacter* Player = Cast<ASignetPlayerCharacter>(GetOwner()))
		{
			if (const UGameInstance* GameInstance = Player->GetGameInstance())
			{
				if (const USignetSaveSubsystem* SaveSubsystem = GameInstance->GetSubsystem<USignetSaveSubsystem>())
				{
					NewSkillRank = SaveSubsystem->GetSkillRank(SkillTag);
					OldSkillRank = NewSkillRank - Amount;
				}
			}
		}

		const float Epsilon = 0.001f;
		const int32 OldWhole = FMath::FloorToInt(OldSkillRank + Epsilon);
		const int32 NewWhole = FMath::FloorToInt(NewSkillRank + Epsilon);

		if (NewWhole > OldWhole)
		{
			Data.Text = FText::FromString(FString::Printf(TEXT("%s increase to %d!"), *SkillName, NewWhole));
		}
		else
		{
			Data.Text = FText::FromString(FString::Printf(TEXT("%s increased by %.1f"), *SkillName, Amount));
		}

		UCombatTextWidget::CreateCombatText(PC, SkillUpWidgetClass, GetOwner(), Data);
	}
}
