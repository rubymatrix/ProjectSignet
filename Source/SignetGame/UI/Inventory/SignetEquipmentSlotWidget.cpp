// Copyright Red Lotus Games, All Rights Reserved.

#include "SignetEquipmentSlotWidget.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Components/Border.h"

void USignetEquipmentSlotWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (RootButton)
	{
		if (!RootButton->OnClicked.IsAlreadyBound(this, &USignetEquipmentSlotWidget::HandleClicked))
		{
			RootButton->OnClicked.AddDynamic(this, &USignetEquipmentSlotWidget::HandleClicked);
		}
	}

	if (FocusBorder)   { FocusBorder->SetVisibility(ESlateVisibility::Collapsed); }
	if (SelectedBorder){ SelectedBorder->SetVisibility(ESlateVisibility::Collapsed); }
}

USignetEquipmentSlotWidget::USignetEquipmentSlotWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	SetIsFocusable(true);
}

void USignetEquipmentSlotWidget::SetSlotData(EGearSlot InSlot, const FInventoryItem* DefOrNull)
{
	GearSlot = InSlot;

	// Show slot name if empty
	FString Friendly =
		UEnum::GetDisplayValueAsText(InSlot).ToString(); // uses UMETA(DisplayName)

	// Remove Left/Right from rings and earrings
	Friendly = Friendly.Replace(TEXT("Left "), TEXT(""));
	Friendly = Friendly.Replace(TEXT("Right "), TEXT(""));

	if (DefOrNull)
	{
		bHasItem = true;

		if (SlotLabel)
		{
			SlotLabel->SetText(FText::FromString(DefOrNull->En.Len() ? DefOrNull->En : Friendly));
			SlotLabel->SetVisibility(DefOrNull->Icon != nullptr ? ESlateVisibility::Collapsed : ESlateVisibility::HitTestInvisible);
		}
		if (IconImage)
		{
			IconImage->SetBrushFromTexture(Cast<UTexture2D>(DefOrNull->Icon), true);
			IconImage->SetVisibility(ESlateVisibility::HitTestInvisible);
		}
		if (ItemQualityBorder)
		{
			FSlateColor NQ{FLinearColor::FromSRGBColor(FColor::FromHex("636363CC"))};
			FSlateColor HQ{FLinearColor::FromSRGBColor(FColor::White)};
			FSlateColor HQ2{FLinearColor::FromSRGBColor(FColor::FromHex("81C4FFFF"))};
			FSlateColor HQ3{FLinearColor::FromSRGBColor(FColor::FromHex("FFFBA6FF"))};
			
			FSlateBrush Brush;

			Brush.DrawAs = ESlateBrushDrawType::Type::RoundedBox;
			Brush.OutlineSettings.Color = NQ;
			Brush.OutlineSettings.Width = 2.f;
			Brush.OutlineSettings.CornerRadii = FVector4{8.f, 8.f, 8.f, 8.f};
			Brush.OutlineSettings.RoundingType = ESlateBrushRoundingType::Type::FixedRadius;

			switch (DefOrNull->ItemQuality)
			{
			case EItemQuality::HQ: Brush.OutlineSettings.Color = HQ; break;
			case EItemQuality::HQ2: Brush.OutlineSettings.Color = HQ2; break;
			case EItemQuality::HQ3: Brush.OutlineSettings.Color = HQ3; break;
			case EItemQuality::Normal: 
			default:
				Brush.OutlineSettings.Color = NQ;
			}

			ItemQualityBorder->SetBrush(Brush);
		}
	}
	else
	{
		bHasItem = false;

		if (SlotLabel)
		{
			SlotLabel->SetText(FText::FromString(Friendly));
			SlotLabel->SetVisibility(ESlateVisibility::HitTestInvisible);
		}
		if (IconImage)
		{
			IconImage->SetBrushFromTexture(nullptr);
			IconImage->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
}

void USignetEquipmentSlotWidget::SetSelected(bool bInSelected)
{
	if (SelectedBorder)
	{
		SelectedBorder->SetVisibility(bInSelected ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
	}
}

FReply USignetEquipmentSlotWidget::NativeOnFocusReceived(const FGeometry& InGeometry, const FFocusEvent& InFocusEvent)
{
	if (FocusBorder)
	{
		FocusBorder->SetVisibility(ESlateVisibility::Visible);
	}
	return Super::NativeOnFocusReceived(InGeometry, InFocusEvent);
}

void USignetEquipmentSlotWidget::NativeOnFocusLost(const FFocusEvent& InFocusEvent)
{
	if (FocusBorder)
	{
		FocusBorder->SetVisibility(ESlateVisibility::Collapsed);
	}
	Super::NativeOnFocusLost(InFocusEvent);
}

FReply USignetEquipmentSlotWidget::NativeOnKeyDown(const FGeometry& MyGeometry, const FKeyEvent& InKeyEvent)
{
	// Enter / Gamepad FaceButtonBottom acts like click
	const FKey K = InKeyEvent.GetKey();
	if (K == EKeys::Enter || K == EKeys::Virtual_Accept)
	{
		HandleClicked();
		return FReply::Handled();
	}
	return Super::NativeOnKeyDown(MyGeometry, InKeyEvent);
}

void USignetEquipmentSlotWidget::HandleClicked()
{
	OnSlotClicked.Broadcast(GearSlot);
}