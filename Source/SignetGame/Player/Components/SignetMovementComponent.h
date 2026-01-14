// Copyright 2025, Red Lotus Games

#pragma once

#include "CoreMinimal.h"
#include "AlsCharacterMovementComponent.h"
#include "GameplayTagContainer.h"
#include "SignetMovementComponent.generated.h"

class ASignetPlayerCharacter;
class USignetAbilitySystemComponent;

UCLASS()
class SIGNETGAME_API USignetMovementComponent : public UAlsCharacterMovementComponent
{
	GENERATED_BODY()

	FGameplayTagContainer MovementBlockingTags;

public:

	USignetMovementComponent();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Movement Modifiers")
	bool bIsBound;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Movement Modifiers", meta=(ClampMin="0.0",ClampMax="1.0"))
	float GravityMovementScale;
	
	virtual float GetMaxSpeed() const override;

	virtual FVector ConsumeInputVector() override;

private:

	USignetAbilitySystemComponent* GetASC() const;
	ASignetPlayerCharacter* GetSignetCharacter() const;
};
