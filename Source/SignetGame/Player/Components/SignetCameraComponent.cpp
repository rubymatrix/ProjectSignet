// Copyright Red Lotus Games, All Rights Reserved.

#include "SignetCameraComponent.h"
#include "SignetGame/Player/SignetPlayerCharacter.h"
#include "TargetingComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "SignetGame/Player/Animation/SignetCameraAnimationInstance.h"

USignetCameraComponent::USignetCameraComponent()
{
}

void USignetCameraComponent::ApplyLockOnFraming(FMinimalViewInfo& OutResult, float DeltaTime)
{
    // ASignetPlayerCharacter* SignetCharacter = Cast<ASignetPlayerCharacter>(GetOwner());
    // if (!SignetCharacter)
    // {
    //     return;
    // }
    //
    // UTargetingComponent* TargetingComp = SignetCharacter->GetTargetingComponent();
    // if (!TargetingComp || !TargetingComp->IsLockedOn() || !TargetingComp->GetTarget())
    // {
    //     // Decay manual offsets if not locked on
    //     ManualYawOffset = FMath::FInterpTo(ManualYawOffset, 0.0f, DeltaTime, ManualReturnSpeed);
    //     ManualPitchOffset = FMath::FInterpTo(ManualPitchOffset, 0.0f, DeltaTime, ManualReturnSpeed);
    //     return;
    // }
    //
    // // Default Lock Framing
    // FVector TargetAimPoint = TargetingComp->GetTargetAimPoint();
    //
    // // Desired rotation to look at target from camera position
    // FRotator LookAtRot = UKismetMathLibrary::FindLookAtRotation(OutResult.Location, TargetAimPoint);
    //
    // // We want to bias the current rotation toward LookAtRot, but with limits
    // // Baseline rotation (ALS behind player)
    // FRotator BaselineRot = OutResult.Rotation;
    //
    // // Compute deviation from baseline
    // FRotator DeltaRot = (LookAtRot - BaselineRot).GetNormalized();
    //
    // // Clamp auto-offsets
    // DeltaRot.Yaw = FMath::Clamp(DeltaRot.Yaw, -MaxAutoYawOffsetDeg, MaxAutoYawOffsetDeg);
    // DeltaRot.Pitch = FMath::Clamp(DeltaRot.Pitch, -MaxAutoPitchOffsetDeg, MaxAutoPitchOffsetDeg);
    //
    // // Decay manual offsets if no recent input (simplified for now)
    // ManualYawOffset = FMath::FInterpTo(ManualYawOffset, 0.0f, DeltaTime, ManualReturnSpeed);
    // ManualPitchOffset = FMath::FInterpTo(ManualPitchOffset, 0.0f, DeltaTime, ManualReturnSpeed);
    //
    // FRotator FinalTargetRot = BaselineRot;
    // FinalTargetRot.Yaw += DeltaRot.Yaw + ManualYawOffset;
    // FinalTargetRot.Pitch += DeltaRot.Pitch + ManualPitchOffset;
    // FinalTargetRot.Normalize();
    //
    // // Smoothly interpolate the view result
    // OutResult.Rotation = FMath::RInterpTo(OutResult.Rotation, FinalTargetRot, DeltaTime, CameraRotSmoothing);
}

void USignetCameraComponent::AddManualOffset(float DeltaYaw, float DeltaPitch)
{
    ManualYawOffset = FMath::Clamp(ManualYawOffset + DeltaYaw, -ManualYawMax, ManualYawMax);
    ManualPitchOffset = FMath::Clamp(ManualPitchOffset + DeltaPitch, -ManualPitchMax, ManualPitchMax);
}

void USignetCameraComponent::NotifyEngaged(bool bIsNewlyEngaged)
{
    if (const auto Inst = Cast<USignetCameraAnimationInstance>(GetAnimInstance()))
    {
        Inst->NotifyEngaged(bIsNewlyEngaged);
    }
}
