// Copyright Red Lotus Games, All Rights Reserved.


#include "CombatTextWidget.h"

#include "Animation/WidgetAnimation.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"

void UCombatTextWidget::NativeConstruct()
{
	Super::NativeConstruct();

	CapturePosition();

	if (FCTAnimation)
	{
		FWidgetAnimationDynamicEvent AnimationFinishedDelegate;
		AnimationFinishedDelegate.BindDynamic(this, &UCombatTextWidget::AnimationFinished);
		BindToAnimationFinished(FCTAnimation, AnimationFinishedDelegate);
	}
}

void UCombatTextWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (bUseViewportCenter)
	{
		UpdateScreenPosition();
		return;
	}

	if (InitialWorldPosition == FVector::ZeroVector)
	{
		CapturePosition();
	}
	else
	{
		UpdateScreenPosition();
	}
}

void UCombatTextWidget::TriggerAnimation()
{
	if (!FCTAnimation) return;
	PlayAnimation(FCTAnimation);
}

void UCombatTextWidget::CapturePosition()
{
	if (TargetActor == nullptr) return;

	const auto Owner = GetOwningPlayer();
	if (!Owner) return;

	const auto ActorLocation = TargetActor->GetActorLocation();
	auto InitialOffset = FVector{
		FMath::RandRange(HorizontalSpawnRange.X, HorizontalSpawnRange.Y),
		FMath::RandRange(VerticalSpawnRange.X, VerticalSpawnRange.Y),
		0.f
	};
	InitialWorldPosition = ActorLocation + InitialOffset;
}

void UCombatTextWidget::UpdateScreenPosition()
{
	if (!TextBlock) return;
	
	const auto Owner = GetOwningPlayer();
	if (!Owner) return;

	if (bUseViewportCenter)
	{
		int32 SizeX = 0;
		int32 SizeY = 0;
		Owner->GetViewportSize(SizeX, SizeY);
		ScreenPosition = FVector2D(static_cast<float>(SizeX) * 0.5f, static_cast<float>(SizeY) * 0.5f);
	}
	else
	{
		UGameplayStatics::ProjectWorldToScreen(Owner, InitialWorldPosition, ScreenPosition, true);
	}

	if (const auto CanvasSlot = Cast<UCanvasPanelSlot>(TextBlock->Slot))
	{
		CanvasSlot->SetPosition(ScreenPosition + ScreenOffset);
	}
}

void UCombatTextWidget::AnimationFinished()
{
	RemoveFromParent();
}

UCombatTextWidget* UCombatTextWidget::CreateCombatText(APlayerController* OwningPlayer, TSubclassOf<UCombatTextWidget> WidgetClass, AActor* InTargetActor, const FCombatTextData& InData)
{
	if (!OwningPlayer || !WidgetClass || !InTargetActor)
	{
		return nullptr;
	}

	UCombatTextWidget* CombatTextWidget = CreateWidget<UCombatTextWidget>(OwningPlayer, WidgetClass);
	if (CombatTextWidget)
	{
		CombatTextWidget->TargetActor = InTargetActor;
		CombatTextWidget->ScreenOffset = InData.ScreenOffset;
		CombatTextWidget->CombatTextType = InData.CombatTextType;

		if (const APawn* Pawn = Cast<APawn>(InTargetActor))
		{
			CombatTextWidget->bUseViewportCenter = Pawn->IsLocallyControlled();
		}
		if (CombatTextWidget->TextBlock)
		{
			CombatTextWidget->TextBlock->SetText(InData.Text);
			CombatTextWidget->TextBlock->SetColorAndOpacity(InData.TextColor);
		}

		if (InData.bIsCritical)
		{
			CombatTextWidget->SetRenderScale({ 1.25f, 1.25f });
		}
		
		CombatTextWidget->AddToViewport();
		CombatTextWidget->TriggerAnimation();
	}

	return CombatTextWidget;
}
