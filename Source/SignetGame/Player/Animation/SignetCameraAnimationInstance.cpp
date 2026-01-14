// Copyright Red Lotus Games, All Rights Reserved.

#include "SignetCameraAnimationInstance.h"
#include "SignetGame/Abilities/SignetAbilitySystemComponent.h"


void USignetCameraAnimationInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	if (const auto Owner = TryGetPawnOwner())
	{
		OwnerComp = Owner->GetComponentByClass<UAbilitySystemComponent>();
		TargetComp = Owner->GetComponentByClass<UTargetingComponent>();
	}

	TargetCameraZoom = CameraConfig.DefaultZoom;
}

void USignetCameraAnimationInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (OwnerComp.IsValid())
	{
		if (const auto A = Cast<USignetAbilitySystemComponent>(OwnerComp.Get()))
		{
			StateTag = A->GetCurrentState();
		}
	}

	if (TargetComp.IsValid())
	{
		bIsTargetLockedOn = TargetComp->bAllowControlRotation;
		TargetOffset = bIsTargetLockedOn ? CameraConfig.CombatOffset : 0.f;
	}

	if (CameraState.Offset != TargetOffset)
	{
		CameraState.Offset = FMath::FInterpTo(CameraState.Offset, TargetOffset, DeltaSeconds, CameraConfig.OffsetInterpolationSpeed);
	}

	if (CameraState.Zoom != TargetCameraZoom)
	{
		CameraState.Zoom = FMath::FInterpTo(CameraState.Zoom, TargetCameraZoom, DeltaSeconds, CameraConfig.ZoomInterpolationSpeed);
	}

	if (CameraState.Height != TargetHeight)
	{
		CameraState.Height = FMath::FInterpTo(CameraState.Height, TargetHeight, DeltaSeconds, CameraConfig.OffsetInterpolationSpeed);
	}
}

void USignetCameraAnimationInstance::AdjustZoom(float ZoomDelta)
{
	TargetCameraZoom = FMath::Clamp(CameraState.Zoom + ZoomDelta, CameraConfig.MinZoom, CameraConfig.MaxZoom);
}

void USignetCameraAnimationInstance::NotifyEngaged(bool bIsNewlyEngaged)
{
	TargetCameraZoom = bIsNewlyEngaged ? CameraConfig.DefaultCombatZoom : CameraConfig.DefaultZoom;
}
