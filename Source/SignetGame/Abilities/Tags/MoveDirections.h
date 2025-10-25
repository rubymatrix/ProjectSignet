#pragma once

#include "GameplayTagContainer.h"

struct FMoveDirectionTags
{
	FGameplayTag Forward;
	FGameplayTag Backward;
	FGameplayTag Left;
	FGameplayTag Right;

	void Initialize()
	{
		Forward = FGameplayTag::RequestGameplayTag("Move.Forward");
		Backward = FGameplayTag::RequestGameplayTag("Move.Backward");
		Left = FGameplayTag::RequestGameplayTag("Move.Left");
		Right = FGameplayTag::RequestGameplayTag("Move.Right");
	}
};