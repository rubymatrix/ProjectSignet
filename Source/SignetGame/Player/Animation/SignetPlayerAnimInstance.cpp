// Copyright Red Lotus Games, All Rights Reserved.

#include "SignetPlayerAnimInstance.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "SignetGame/Abilities/TagCache.h"
#include "SignetGame/Player/SignetPlayerCharacter.h"

USignetPlayerAnimInstance::USignetPlayerAnimInstance()
{
	MainWeaponSkillTag = FTagCache::Get().Skill.None;
	SubWeaponSkillTag = FTagCache::Get().Skill.None;
	RangedWeaponSkillTag = FTagCache::Get().Skill.None;
	StateTag = FTagCache::Get().State.Idle;
	CastingTag = FGameplayTag::EmptyTag;
	GaitTag = AlsGaitTags::Running;
	RotationMode = AlsRotationModeTags::VelocityDirection;
	MoveDirectionTag = FTagCache::Get().Movement.Forward;
}

void USignetPlayerAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	SignetCharacter = Cast<ASignetPlayerCharacter>(GetOwningActor());

#if WITH_EDITOR
	if (const auto* World = GetWorld(); IsValid(World) && !World->IsGameWorld())
	{
		if (!IsValid(Character))
		{
			Character = GetMutableDefault<ASignetPlayerCharacter>();
		}
	}
#endif 
}

void USignetPlayerAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (SignetCharacter.IsValid())
	{
		GaitTag = Character->GetDesiredGait();
		RotationModeTag = Character->GetDesiredRotationMode();

		DetermineWeaponTags();
		DetermineStateTag();
		DetermineCastingTag();

		MoveDirectionTag = DetermineDirectionTag();
	}
}

bool USignetPlayerAnimInstance::IsMoving() const
{
	if (!SignetCharacter.IsValid() || !SignetCharacter->GetMovementComponent())
		return false;

	if (const UCharacterMovementComponent* MoveComp = SignetCharacter->GetCharacterMovement())
	{
		constexpr auto MinSpeed = 10.f;
		const FVector2D Velocity2D = FVector2D(MoveComp->Velocity.X, MoveComp->Velocity.Y);
		return Velocity2D.SizeSquared() > FMath::Square(MinSpeed);
	}
	return false;
}

FWeaponOverlay USignetPlayerAnimInstance::GetWeaponData(const FGameplayTag& InWeaponTag) const
{
	if (const auto Data = WeaponOverlayAnims.Find(InWeaponTag))
	{
		return *Data;
	}
	return {};
}

FLayeredSequence USignetPlayerAnimInstance::GetCastingSchedule(const FGameplayTag& InCastingTag) const
{
	if (const auto Data = CastingAnims.Find(InCastingTag))
	{
		return *Data;
	}
	return {};
}

FLayeredSequence USignetPlayerAnimInstance::GetCurrentCastingSchedule() const
{
	return GetCastingSchedule(CastingTag);
}

FWeaponOverlay USignetPlayerAnimInstance::GetCurrentWeaponData() const
{
	if (const auto Data = WeaponOverlayAnims.Find(MainWeaponSkillTag))
	{
		return *Data;
	}
	return {};
}

FLayeredSequence USignetPlayerAnimInstance::GetCurrentState() const
{
	if (StateTag == FGameplayTag::EmptyTag ||
		StateTag == FTagCache::Get().State.Cutscene ||
		StateTag == FTagCache::Get().State.Stunned ||
		StateTag == FTagCache::Get().State.Idle)
	{
		return Animations.Idle;
	}

	if (StateTag == FTagCache::Get().State.Engaged)
	{
		return GetCurrentWeaponData().Idle;
	}

	if (StateTag == FTagCache::Get().State.Dead)
	{
		return Animations.Dead;
	}

	if (StateTag == FTagCache::Get().State.Casting)
	{
		return GetCurrentCastingSchedule();
	}

	if (StateTag == FTagCache::Get().State.Sitting)
	{
		return Animations.Sitting;
	}

	if (StateTag == FTagCache::Get().State.Resting)
	{
		return Animations.Resting;
	}

	if (StateTag == FTagCache::Get().State.Crafting)
	{
		return Animations.Crafting;
	}

	return Animations.Idle;
}

FLayeredSequence USignetPlayerAnimInstance::GetCurrentWalkAnim() const
{
	if (StateTag == FTagCache::Get().State.Engaged) return GetCurrentWeaponData().Walking;
	return Animations.Walking;
}

FLayeredSequence USignetPlayerAnimInstance::GetCurrentRunAnim() const
{
	if (StateTag == FTagCache::Get().State.Engaged) return GetCurrentWeaponData().Running;
	return Animations.Running;
}

FLayeredSequence USignetPlayerAnimInstance::GetCurrentIdleAnim() const
{
	if (StateTag == FTagCache::Get().State.Engaged) return GetCurrentWeaponData().Idle;
	return Animations.Idle;
}

void USignetPlayerAnimInstance::DetermineStateTag()
{
	if (!SignetCharacter.IsValid() || SignetCharacter->GetAbilitySystemComponent() == nullptr)
		return;

	if (const auto Asc = SignetCharacter->GetAbilitySystemComponent())
	{
		for (const auto& Tag : FTagCache::Get().State.AllTags)
		{
			if (Asc->HasMatchingGameplayTag(Tag))
			{
				StateTag = Tag;
				return;
			}
		}	
	}

	StateTag = FTagCache::Get().State.Idle;
}

void USignetPlayerAnimInstance::DetermineCastingTag()
{
	if (!SignetCharacter.IsValid() || SignetCharacter->GetAbilitySystemComponent() == nullptr) return;

	if (const auto Asc = SignetCharacter->GetAbilitySystemComponent())
	{
		for (auto& Tag : FTagCache::Get().Casting.AllTags)
		{
			if (Asc->HasMatchingGameplayTag(Tag))
			{
				CastingTag = Tag;
				return;
			}
		}
	}

	CastingTag = FGameplayTag::EmptyTag;
}

void USignetPlayerAnimInstance::DetermineWeaponTags()
{
	// TODO: Implement after Inventory Component
}

FGameplayTag USignetPlayerAnimInstance::DetermineDirectionTag()
{
	if (!SignetCharacter.IsValid() || !SignetCharacter->GetMovementComponent())
	{
		return FTagCache::Get().Movement.Forward;
	}

	const float MinSpeed = 10.f;

	FVector V = SignetCharacter->GetVelocity();
	V.Z = 0.f;
	if (V.Size() < MinSpeed)
	{
		return FTagCache::Get().Movement.Forward;
	}

	// Normalize
	const FVector Dir = V.GetSafeNormal2D();
	FVector Forward   = SignetCharacter->GetActorForwardVector();
	Forward.Z = 0.f;
	Forward.Normalize();

	// Compute signed angle between forward and velocity
	const float AngleDeg = FMath::RadiansToDegrees(
		FMath::Atan2(
			FVector::CrossProduct(Forward, Dir).Z,
			FVector::DotProduct(Forward, Dir)
		)
	);

	// Map angle ranges to cardinal tags
	if (AngleDeg >= -46.f && AngleDeg <= 46.f)
	{
		return FTagCache::Get().Movement.Forward;
	}
	if (AngleDeg > 46.f && AngleDeg < 134.f)
	{
		return FTagCache::Get().Movement.Right;
	}
	if (AngleDeg >= 133.f || AngleDeg <= -133.f)
	{
		return FTagCache::Get().Movement.Backward;
	}
	if (AngleDeg < -45.f && AngleDeg > -135.f)
	{
		return FTagCache::Get().Movement.Left;
	}

	return FTagCache::Get().Movement.Forward;
}
