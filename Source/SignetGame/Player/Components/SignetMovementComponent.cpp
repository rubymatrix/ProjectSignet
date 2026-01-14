// Copyright 2025, Red Lotus Games

#include "SignetMovementComponent.h"
#include "SignetGame/Abilities/TagCache.h"
#include "SignetGame/Abilities/SignetAbilitySystemComponent.h"
#include "SignetGame/Player/SignetPlayerCharacter.h"

USignetMovementComponent::USignetMovementComponent()
{
	bIsBound             = false;
	GravityMovementScale = 1.0f;

	MovementBlockingTags.AddTag(FTagCache::Get().State.Dead);
	MovementBlockingTags.AddTag(FTagCache::Get().State.Resting);
	MovementBlockingTags.AddTag(FTagCache::Get().State.Sitting);
	MovementBlockingTags.AddTag(FTagCache::Get().State.Stunned);
	MovementBlockingTags.AddTag(FTagCache::Get().State.Cutscene);
	MovementBlockingTags.AddTag(FTagCache::Get().Effect.Bound);
	MovementBlockingTags.AddTag(FTagCache::Get().Effect.Stunned);
	MovementBlockingTags.AddTag(FTagCache::Get().Effect.KO);
	MovementBlockingTags.AddTag(FTagCache::Get().Effect.Sleep);
}

float USignetMovementComponent::GetMaxSpeed() const
{
	const auto BaseMoveSpeed = Super::GetMaxSpeed();
	
	if (const auto Asc = GetASC())
	{
		if (Asc->HasAnyEffectsWithTags(MovementBlockingTags) || bIsBound)
		{
			return 0.f;
		}
	}

	if (bIsBound)
	{
		return 0.f;
	}

	float MoveMod = 1.f;
	if (const auto Char = GetSignetCharacter())
	{
		MoveMod = Char->GetMoveSpeedMultiplier();
	}

	return BaseMoveSpeed * MoveMod;
}

FVector USignetMovementComponent::ConsumeInputVector()
{
	auto Value = Super::ConsumeInputVector();
	
	if (const auto Asc = GetASC())
	{
		if (Asc->HasAnyEffectsWithTags(MovementBlockingTags) || bIsBound)
		{
			Value = FVector::ZeroVector;
		}
	}
	return Value; 
}

USignetAbilitySystemComponent* USignetMovementComponent::GetASC() const
{
	const auto Pawn = GetSignetCharacter();
	if (!Pawn) return nullptr;
	return Pawn->GetSignetAsc();
}

ASignetPlayerCharacter* USignetMovementComponent::GetSignetCharacter() const
{
	return Cast<ASignetPlayerCharacter>(GetOwner());
}
