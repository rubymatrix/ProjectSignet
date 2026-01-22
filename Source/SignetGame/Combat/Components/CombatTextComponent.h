// Copyright Red Lotus Games, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Components/ActorComponent.h"
#include "CombatTextComponent.generated.h"


UCLASS(ClassGroup=(SignetGame), meta=(BlueprintSpawnableComponent))
class SIGNETGAME_API UCombatTextComponent : public UActorComponent
{
	GENERATED_BODY()

public:

	UCombatTextComponent();

	UFUNCTION(BlueprintCallable)
	void AddDamageText(float Amount, bool bCritical = false);

	UFUNCTION(BlueprintCallable)
	void AddHealingText(float Amount, bool bCritical = false);

	UFUNCTION(BlueprintCallable)
	void AddStatusEffectText(const FString& Text);

	UFUNCTION(BlueprintCallable)
	void AddSkillUpText(const FGameplayTag& SkillTag, float Amount);

protected:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat Text")
	TSubclassOf<class UCombatTextWidget> CombatTextWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat Text")
	TSubclassOf<class UCombatTextWidget> StatusEffectWidgetClass;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat Text")
	TSubclassOf<class UCombatTextWidget> SkillUpWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat Text")
	FSlateColor DefaultTextColor = FLinearColor::White;;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat Text")
	FSlateColor CriticalTextColor = FLinearColor::Red;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat Text")
	FSlateColor HealingTextColor = FLinearColor::Green;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat Text")
	FSlateColor StatusEffectTextColor = FLinearColor::Yellow;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat Text")
	FSlateColor SkillUpTextColor = FLinearColor(0.4f, 1.0f, 0.4f);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat Text")
	FVector2D SkillUpScreenOffset = FVector2D(100.0f, 0.0f);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat Text")
	FVector2D StatusEffectScreenOffset = FVector2D(0.f, 0.0f);
};
