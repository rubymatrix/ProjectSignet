// Copyright Red Lotus Games, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CombatTextWidget.generated.h"

class UTextBlock;

UENUM(BlueprintType)
enum class ECombatTextType : uint8
{
	Damage,
	Healing,
	StatusEffect,
	SkillUp
};

USTRUCT(BlueprintType)
struct FCombatTextData
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere)
	bool bIsCritical = false;
	
	UPROPERTY(VisibleAnywhere)
	ECombatTextType CombatTextType;

	UPROPERTY(VisibleAnywhere)
	FText Text;

	UPROPERTY(VisibleAnywhere)
	FSlateColor TextColor;

	UPROPERTY(VisibleAnywhere)
	FVector2D ScreenOffset = FVector2D::ZeroVector;
};


/**
 * 
 */
UCLASS()
class SIGNETGAME_API UCombatTextWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FVector2D HorizontalSpawnRange = FVector2D(25.f, 50.f);
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FVector2D VerticalSpawnRange = FVector2D(-50.f, 50.f);

	UPROPERTY(Transient)
	TObjectPtr<AActor> TargetActor;
	
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, meta=(BindWidget))
	TObjectPtr<UTextBlock> TextBlock;

	UPROPERTY(BlueprintReadOnly, Transient, meta=(BindWidgetAnim))
	TObjectPtr<UWidgetAnimation> FCTAnimation;

	virtual void NativeConstruct() override;
	
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

public:

	UPROPERTY(Transient)
	ECombatTextType CombatTextType;

	void TriggerAnimation();

private:

	UPROPERTY(Transient)
	FVector InitialWorldPosition;

	UPROPERTY(Transient)
	FVector2D ScreenPosition;

	UPROPERTY(Transient)
	FVector2D ScreenOffset;

	UPROPERTY(Transient)
	bool bUseViewportCenter = false;
	
	void CapturePosition();

	void UpdateScreenPosition();

	UFUNCTION()
	void AnimationFinished();

public:
	
	static UCombatTextWidget* CreateCombatText(APlayerController* OwningPlayer, TSubclassOf<UCombatTextWidget> WidgetClass, AActor* InTargetActor, const FCombatTextData& InData);
};
