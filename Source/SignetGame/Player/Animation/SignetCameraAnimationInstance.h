// Copyright Red Lotus Games, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AlsCameraAnimationInstance.h"
#include "SignetGame/Player/Components/TargetingComponent.h"
#include "SignetCameraAnimationInstance.generated.h"


class UAbilitySystemComponent;

USTRUCT(BlueprintType)
struct SIGNETGAME_API FCameraConfig
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
	float DefaultZoom = -325.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
	float MaxZoom = -325.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
	float MinZoom = -325.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
	float ZoomInterpolationSpeed = 10.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
	float OffsetInterpolationSpeed = 10.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
	float CombatOffset = 150.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
	float DefaultCombatZoom = -200.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
	float CameraHeight = 125.f;
	
};

USTRUCT(BlueprintType)
struct SIGNETGAME_API FCameraStateProperties
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
	float Zoom = -325.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
	float Offset = 0.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
	float Height = 125.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
	float TargetRotation = 0.f;
};


/**
 * 
 */
UCLASS()
class SIGNETGAME_API USignetCameraAnimationInstance : public UAlsCameraAnimationInstance
{
	GENERATED_BODY()

	float TargetCameraZoom = -325.f;

	float TargetOffset = 0.f;

	float TargetHeight = 125.f;

protected:

	

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "State", Transient)
	FCameraConfig CameraConfig;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "State", Transient)
	FCameraStateProperties CameraState;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "State", Transient)
	TWeakObjectPtr<UAbilitySystemComponent> OwnerComp;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "State", Transient)
	TWeakObjectPtr<UTargetingComponent> TargetComp;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "State", Transient)
	FGameplayTag StateTag;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "State", Transient)
	bool bIsTargetLockedOn = false;
	

public:

	virtual void NativeInitializeAnimation() override;

	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	virtual void AdjustZoom(float ZoomDelta);

	virtual void NotifyEngaged(bool bIsNewlyEngaged);
};
