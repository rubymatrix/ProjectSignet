// Copyright Red Lotus Games, All Rights Reserved.


#include "CameraShakeComponent.h"
#include "GameFramework/Character.h"

void UCameraShakeComponent::PlayCameraShake(const ECameraShakeType& ShakeType, float Intensity)
{
	const auto Owner = Cast<ACharacter>(GetOwner());
	if (!Owner) return;

	const auto Controller = Cast<APlayerController>(Owner->GetController());
	if (!Controller) return;

	const auto CameraManager = Controller->PlayerCameraManager;
	if (!CameraManager) return;

	if (const auto ShakeClass = CameraShakeClasses.Find(ShakeType))
	{
		CameraManager->StartCameraShake(*ShakeClass, Intensity);
	}
}
