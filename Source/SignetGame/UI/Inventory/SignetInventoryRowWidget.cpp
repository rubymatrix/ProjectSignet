#include "SignetInventoryRowWidget.h"
#include "SignetInventoryListItem.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Components/Border.h"
#include "Components/Button.h"
#include "SignetGame/Inventory/SignetInventoryComponent.h"


void USignetInventoryRowWidget::NativeOnListItemObjectSet(UObject* ListItemObject)
{
	if (const auto Data = Cast<USignetInventoryListItem>(ListItemObject))
	{
		const auto Color = FSlateColor{FLinearColor::FromSRGBColor(FColor::White)};
		const auto ActiveColor = FSlateColor{FLinearColor::FromSRGBColor(FColor::FromHex("81F1A8"))};

		Item = Data;
		
		if (NameText)
		{
			NameText->SetText(Data->DisplayName);
			NameText->SetColorAndOpacity(Data->bIsEquipped ? ActiveColor : Color);
		}
			
		if (QtyText)
		{
			QtyText->SetText(FText::AsNumber(Data->Quantity));
			QtyText->SetColorAndOpacity(Data->bIsEquipped ? ActiveColor : Color);
			
			if (Data->Quantity <= 1)
			{
				QtyText->SetVisibility(ESlateVisibility::Collapsed);
			}
			else
			{
				QtyText->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
			}
		}
			
		if (IconImage)
		{
			IconImage->SetBrushFromTexture(Data->Icon, true);
		}
			

		if (IconOutline)
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

			switch (Data->ItemQuality)
			{
			case EItemQuality::HQ: Brush.OutlineSettings.Color = HQ; break;
			case EItemQuality::HQ2: Brush.OutlineSettings.Color = HQ2; break;
			case EItemQuality::HQ3: Brush.OutlineSettings.Color = HQ3; break;
			case EItemQuality::Normal: 
			default:
				Brush.OutlineSettings.Color = NQ;
			}

			IconOutline->SetBrush(Brush);
		}

		if (RootButton)
		{
			if (!RootButton->OnClicked.IsAlreadyBound(this, &ThisClass::HandleClicked))
			{
				RootButton->OnClicked.AddDynamic(this, &ThisClass::HandleClicked);
			}
		}
	}
}

void USignetInventoryRowWidget::HandleClicked()
{
	const auto InvComp = GetOwningPlayerPawn()->GetComponentByClass<USignetInventoryComponent>();
	if (!InvComp) return;
	
	if (Item.IsValid())
	{
		if (!InvComp->IsEquipped(Item->InstanceId))
		{
			InvComp->ServerEquipFromInstance(Item->GearSlot, Item->InstanceId);
		}
		else
		{
			InvComp->ServerUnequip(Item->GearSlot);
		}
	}
}

void USignetInventoryRowWidget::HandleRightClicked()
{
}
