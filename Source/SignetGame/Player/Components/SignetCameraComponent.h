// Copyright Red Lotus Games, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AlsCameraComponent.h"
#include "SignetCameraComponent.generated.h"

UCLASS(ClassGroup=Signet)
class SIGNETGAME_API USignetCameraComponent : public UAlsCameraComponent
{
	GENERATED_BODY()

public:
	USignetCameraComponent();

	void ApplyLockOnFraming(FMinimalViewInfo& OutResult, float DeltaTime);

	void AddManualOffset(float DeltaYaw, float DeltaPitch);

	void NotifyEngaged(bool bIsNewlyEngaged);

protected:
	/** Max auto yaw offset toward target. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Signet|Camera|TargetLock")
	float MaxAutoYawOffsetDeg = 15.0f;

	/** Max auto pitch offset toward target. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Signet|Camera|TargetLock")
	float MaxAutoPitchOffsetDeg = 10.0f;

	/** Smoothing for camera rotation. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Signet|Camera|TargetLock")
	float CameraRotSmoothing = 10.0f;

	/** Manual yaw offset from lock-on baseline. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Signet|Camera|TargetLock")
	float ManualYawOffset = 0.0f;

	/** Manual pitch offset from lock-on baseline. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Signet|Camera|TargetLock")
	float ManualPitchOffset = 0.0f;

	/** Max manual yaw deviation. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Signet|Camera|TargetLock")
	float ManualYawMax = 30.0f;

	/** Max manual pitch deviation. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Signet|Camera|TargetLock")
	float ManualPitchMax = 20.0f;

	/** Speed at which manual offsets return to zero. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Signet|Camera|TargetLock")
	float ManualReturnSpeed = 5.0f;
};