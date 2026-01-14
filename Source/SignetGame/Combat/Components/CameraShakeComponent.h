// Copyright Red Lotus Games, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CameraShakeComponent.generated.h"

UENUM(BlueprintType)
enum class ECameraShakeType : uint8
{
	CriticalImpact,
	MajorImpact,
	SmallFootstep,
	LargeFootstep,
};

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class SIGNETGAME_API UCameraShakeComponent : public UActorComponent
{
	GENERATED_BODY()

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TMap<ECameraShakeType, TSubclassOf<UCameraShakeBase>> CameraShakeClasses;

public:

	UFUNCTION(BlueprintCallable)
	void PlayCameraShake(const ECameraShakeType& ShakeType, float Intensity = 1.f);

};
