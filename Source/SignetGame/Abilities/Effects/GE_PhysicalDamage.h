// Copyright Red Lotus Games, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffect.h"
#include "GE_PhysicalDamage.generated.h"

/**
 * Gameplay Effect for applying physical damage.
 * Configured to modify the Damage meta-attribute.
 */
UCLASS()
class SIGNETGAME_API UGE_PhysicalDamage : public UGameplayEffect
{
	GENERATED_BODY()

public:
	UGE_PhysicalDamage();
};
