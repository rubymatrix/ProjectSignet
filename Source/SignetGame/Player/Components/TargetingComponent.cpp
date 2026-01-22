// Copyright 2018-2021 Mickael Daniel. All Rights Reserved.

#include "TargetingComponent.h"

#include "AlsCameraComponent.h"
#include "CharacterAudioComponent.h"
#include "TargetableInterface.h"
#include "Utility/AlsCameraConstants.h"
#include "Components/WidgetComponent.h"
#include "EngineUtils.h"
#include "Camera/CameraComponent.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"
#include "Engine/World.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerController.h"

// Sets default values for this component's properties
UTargetingComponent::UTargetingComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	LockedOnWidgetClass = StaticLoadClass(UObject::StaticClass(), nullptr, TEXT("/Game/UI/WBP_LockOn.WBP_LockOn_C"));
	TargetableActors = APawn::StaticClass();
	TargetableCollisionChannel = ECollisionChannel::ECC_Pawn;
	MinimumDistanceToUpdateRotation = 150.0f;
}

// Called when the game starts
void UTargetingComponent::BeginPlay()
{
	Super::BeginPlay();
	OwnerActor = GetOwner();
	if (!OwnerActor)
	{
		// TS_LOG(Error, TEXT("[%s] TargetSystemComponent: Cannot get Owner reference ..."), *GetName());
		return;
	}

	OwnerPawn = Cast<APawn>(OwnerActor);
	if (!ensure(OwnerPawn))
	{
		// TS_LOG(Error, TEXT("[%s] TargetSystemComponent: Component is meant to be added to Pawn only ..."), *GetName());
		return;
	}

	SetupLocalPlayerController();
}

void UTargetingComponent::TickComponent(const float DeltaTime, const ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!bTargetLocked || !LockedOnTargetActor)
	{
		return;
	}

	if (!TargetIsTargetable(LockedOnTargetActor))
	{
		TargetLockOff();
		return;
	}

	if (bAllowControlRotation)
	{
		SetControlRotationOnTarget(LockedOnTargetActor);	
	}
	

	// Target Locked Off based on Distance
	if (GetDistanceFromCharacter(LockedOnTargetActor) > MinimumDistanceToEnable)
	{
		TargetLockOff();
	}

	if (ShouldBreakLineOfSight() && !bIsBreakingLineOfSight)
	{
		if (BreakLineOfSightDelay <= 0)
		{
			TargetLockOff();
		}
		else
		{
			bIsBreakingLineOfSight = true;
			GetWorld()->GetTimerManager().SetTimer(
				LineOfSightBreakTimerHandle,
				this,
				&UTargetingComponent::BreakLineOfSight,
				BreakLineOfSightDelay
			);
		}
	}
}

void UTargetingComponent::TargetActor()
{
	ClosestTargetDistance = MinimumDistanceToEnable;

	if (bTargetLocked)
	{
		TargetLockOff();
	}
	else
	{
		const TArray<AActor*> Actors = GetAllActorsOfClass(TargetableActors);
		LockedOnTargetActor = FindNearestTarget(Actors);
		TargetLockOn(LockedOnTargetActor);
	}
}

void UTargetingComponent::SetTargetActor(AActor* NewTarget)
{
	if (!IsValid(NewTarget))
	{
		TargetLockOff();
		return;
	}

	const bool bWasAllowControlRotation = bAllowControlRotation;
	if (bTargetLocked)
	{
		TargetLockOff();
	}

	LockedOnTargetActor = NewTarget;
	TargetLockOn(NewTarget);
	bAllowControlRotation = bWasAllowControlRotation;
}

void UTargetingComponent::TargetActorWithAxisInput(const float AxisValue)
{
	// If we're not locked on, do nothing
	if (!bTargetLocked || !LockedOnTargetActor)
	{
		return;
	}

	// If we're not allowed to switch target, do nothing
	if (!ShouldSwitchTargetActor(AxisValue))
	{
		return;
	}

	// If we're switching target, do nothing for a set amount of time
	if (bIsSwitchingTarget)
	{
		return;
	}

	AActor* CurrentTarget = LockedOnTargetActor;

	// Get All Actors of Class
	TArray<AActor*> Actors = GetAllActorsOfClass(TargetableActors);

	// Filter actors in 180 degree cone and that are reachable/visible
	TArray<AActor*> ValidTargets;
	for (AActor* Actor : Actors)
	{
		const float Angle = FMath::Abs(GetAngleUsingCameraRotation(Actor));
		if (Angle > 90.0f)
		{
			continue;
		}

		TArray<AActor*> ActorsToIgnore;
		const bool bHit = LineTraceForActor(Actor, ActorsToIgnore);
		if (bHit && IsInViewport(Actor))
		{
			ValidTargets.Add(Actor);
		}
	}

	if (ValidTargets.Num() <= 1)
	{
		return;
	}

	// Sort targets by angle from left to right (negative angles are right, positive are left in my implementation)
	// Wait, let me re-verify my sign convention in GetAngleUsingCameraRotation:
	// RightDotProduct < 0.0f => AngleDegrees *= -1.0f;
	// If target is to the right, RightDotProduct is positive (> 0).
	// If RightDotProduct < 0, it means it's to the LEFT.
	// So Left is negative, Right is positive.
	
	// Let's sort from left to right: negative to positive.
	ValidTargets.Sort([this](const AActor& A, const AActor& B) {
		return GetAngleUsingCameraRotation(&A) < GetAngleUsingCameraRotation(&B);
	});

	int32 CurrentIndex = ValidTargets.Find(CurrentTarget);
	if (CurrentIndex == INDEX_NONE)
	{
		// Current target might not be in the cone anymore or something, find closest index by angle
		float CurrentAngle = GetAngleUsingCameraRotation(CurrentTarget);
		float MinAngleDiff = FLT_MAX;
		for (int32 i = 0; i < ValidTargets.Num(); ++i)
		{
			float Diff = FMath::Abs(GetAngleUsingCameraRotation(ValidTargets[i]) - CurrentAngle);
			if (Diff < MinAngleDiff)
			{
				MinAngleDiff = Diff;
				CurrentIndex = i;
			}
		}
	}

	// moving right if the axis value is positive, or left if it's negative
	// AxisValue > 0 => Move Right => Increase index (since we sorted left to right)
	// AxisValue < 0 => Move Left => Decrease index
	
	int32 NextIndex = CurrentIndex + (AxisValue > 0 ? 1 : -1);
	
	// Looping logic
	if (NextIndex < 0)
	{
		NextIndex = ValidTargets.Num() - 1;
	}
	else if (NextIndex >= ValidTargets.Num())
	{
		NextIndex = 0;
	}

	AActor* ActorToTarget = ValidTargets[NextIndex];

	if (ActorToTarget && ActorToTarget != CurrentTarget)
	{
		if (SwitchingTargetTimerHandle.IsValid())
		{
			SwitchingTargetTimerHandle.Invalidate();
		}

		const bool bWasAllowControlRotation = bAllowControlRotation;
		TargetLockOff();
		LockedOnTargetActor = ActorToTarget;
		TargetLockOn(ActorToTarget);
		bAllowControlRotation = bWasAllowControlRotation;

		GetWorld()->GetTimerManager().SetTimer(
			SwitchingTargetTimerHandle,
			this,
			&UTargetingComponent::ResetIsSwitchingTarget,
			// Less sticky if still switching
			bIsSwitchingTarget ? 0.1f : 0.2f
		);

		bIsSwitchingTarget = true;
	}
}

bool UTargetingComponent::GetTargetLockedStatus()
{
	return bTargetLocked;
}

AActor* UTargetingComponent::GetLockedOnTargetActor() const
{
	return LockedOnTargetActor;
}

bool UTargetingComponent::IsLocked() const
{
	return bTargetLocked && LockedOnTargetActor;
}


float UTargetingComponent::GetAngleUsingCameraRotation(const AActor* ActorToLook) const
{
	FVector CameraLocation;
	FRotator CameraRotation;
	if (!GetCameraLocationAndRotation(CameraLocation, CameraRotation))
	{
		// Fallback to CharacterRotation if no Camera can be found
		return GetAngleUsingCharacterRotation(ActorToLook);
	}

	const FVector TargetLocation = GetTargetLocation(ActorToLook);
	const FVector DirectionToTarget = (TargetLocation - CameraLocation).GetSafeNormal();
	const FVector CameraForward = CameraRotation.Vector();

	// Calculate the angle in degrees
	const float DotProduct = FVector::DotProduct(CameraForward, DirectionToTarget);
	const float AngleRadians = FMath::Acos(FMath::Clamp(DotProduct, -1.0f, 1.0f));
	float AngleDegrees = FMath::RadiansToDegrees(AngleRadians);

	// Determine if the target is to the left or right to give it a sign
	const FVector CameraRight = FRotationMatrix(CameraRotation).GetScaledAxis(EAxis::Y);
	const float RightDotProduct = FVector::DotProduct(CameraRight, DirectionToTarget);

	if (RightDotProduct < 0.0f)
	{
		AngleDegrees *= -1.0f;
	}

	return AngleDegrees;
}

float UTargetingComponent::GetAngleUsingCharacterRotation(const AActor* ActorToLook) const
{
	const FVector CharacterLocation = OwnerActor->GetActorLocation();
	const FVector TargetLocation = GetTargetLocation(ActorToLook);
	const FVector DirectionToTarget = (TargetLocation - CharacterLocation).GetSafeNormal();
	const FVector CharacterForward = OwnerActor->GetActorForwardVector();

	// Calculate the angle in degrees
	const float DotProduct = FVector::DotProduct(CharacterForward, DirectionToTarget);
	const float AngleRadians = FMath::Acos(FMath::Clamp(DotProduct, -1.0f, 1.0f));
	float AngleDegrees = FMath::RadiansToDegrees(AngleRadians);

	// Determine if the target is to the left or right to give it a sign
	const FVector CharacterRight = OwnerActor->GetActorRightVector();
	const float RightDotProduct = FVector::DotProduct(CharacterRight, DirectionToTarget);

	if (RightDotProduct < 0.0f)
	{
		AngleDegrees *= -1.0f;
	}

	return AngleDegrees;
}

FRotator UTargetingComponent::FindLookAtRotation(const FVector Start, const FVector Target)
{
	return FRotationMatrix::MakeFromX(Target - Start).Rotator();
}

void UTargetingComponent::ResetIsSwitchingTarget()
{
	bIsSwitchingTarget = false;
	bDesireToSwitch = false;
}

bool UTargetingComponent::ShouldSwitchTargetActor(const float AxisValue)
{
	// Sticky feeling computation
	if (bEnableStickyTarget)
	{
		StartRotatingStack += (AxisValue != 0.0f) ? (AxisValue * AxisMultiplier) : (StartRotatingStack > 0.0f ? -AxisMultiplier : AxisMultiplier);

		if (AxisValue == 0.0f && FMath::Abs(StartRotatingStack) <= AxisMultiplier)
		{
			StartRotatingStack = 0.0f;
		}

		// If Axis value does not exceeds configured threshold, do nothing
		if (FMath::Abs(StartRotatingStack) < StickyRotationThreshold)
		{
			bDesireToSwitch = false;
			return false;
		}

		//Sticky when switching target.
		const float Product = StartRotatingStack * AxisValue;
		if (Product > 0.0f)
		{
			StartRotatingStack = (StartRotatingStack > 0.0f) ? StickyRotationThreshold : -StickyRotationThreshold;
		}
		else if (Product < 0.0f)
		{
			StartRotatingStack = StartRotatingStack * -1.0f;
		}

		bDesireToSwitch = true;

		return true;
	}

	// Non Sticky feeling, check Axis value exceeds threshold
	return FMath::Abs(AxisValue) > StartRotatingThreshold;
}

void UTargetingComponent::TargetLockOn(AActor* TargetToLockOn)
{
	if (!IsValid(TargetToLockOn))
	{
		return;
	}

	// Recast PlayerController in case it wasn't already setup on Begin Play (local split screen)
	SetupLocalPlayerController();

	bTargetLocked = true;
	if (bShouldDrawLockedOnWidget)
	{
		CreateAndAttachTargetLockedOnWidgetComponent(TargetToLockOn);
	}

	if (bShouldControlRotation)
	{
		ControlRotation(true);
	}

	if (bAdjustPitchBasedOnDistanceToTarget || bIgnoreLookInput)
	{
		if (IsValid(OwnerPlayerController))
		{
			OwnerPlayerController->SetIgnoreLookInput(true);
		}
	}

	if (OnTargetLockedOn.IsBound())
	{
		OnTargetLockedOn.Broadcast(TargetToLockOn);
	}

	if (const auto AudioComp = GetOwner()->GetComponentByClass<UCharacterAudioComponent>())
	{
		AudioComp->PlayUISound(EUISound::Target);
	}
}

void UTargetingComponent::TargetLockOff()
{
	// Recast PlayerController in case it wasn't already setup on Begin Play (local split screen)
	SetupLocalPlayerController();

	bTargetLocked = false;
	if (TargetLockedOnWidgetComponent)
	{
		TargetLockedOnWidgetComponent->DestroyComponent();
	}

	if (LockedOnTargetActor)
	{
		if (bShouldControlRotation)
		{
			ControlRotation(false);
		}

		if (IsValid(OwnerPlayerController))
		{
			OwnerPlayerController->ResetIgnoreLookInput();
		}

		if (OnTargetLockedOff.IsBound())
		{
			OnTargetLockedOff.Broadcast(LockedOnTargetActor);
		}

		// if (const auto AudioComp = GetOwner()->GetComponentByClass<UCharacterAudioComponent>())
		// {
		// 	AudioComp->PlayUISound(EUISound::Cancel);
		// }
	}

	bAllowControlRotation = false;
	LockedOnTargetActor = nullptr;
}

void UTargetingComponent::CreateAndAttachTargetLockedOnWidgetComponent(AActor* TargetActor)
{
	if (!LockedOnWidgetClass)
	{
		// TS_LOG(Error, TEXT("TargetSystemComponent: Cannot get LockedOnWidgetClass, please ensure it is a valid reference in the Component Properties."));
		return;
	}

	TargetLockedOnWidgetComponent = NewObject<UWidgetComponent>(TargetActor, MakeUniqueObjectName(TargetActor, UWidgetComponent::StaticClass(), FName("TargetLockOn")));
	TargetLockedOnWidgetComponent->SetWidgetClass(LockedOnWidgetClass);

	UMeshComponent* MeshComponent = TargetActor->FindComponentByClass<UMeshComponent>();
	USceneComponent* ParentComponent = MeshComponent && LockedOnWidgetParentSocket != NAME_None ? MeshComponent : TargetActor->GetRootComponent();

	if (IsValid(OwnerPlayerController))
	{
		TargetLockedOnWidgetComponent->SetOwnerPlayer(OwnerPlayerController->GetLocalPlayer());
	}

	TargetLockedOnWidgetComponent->ComponentTags.Add(FName("TargetSystem.LockOnWidget"));
	TargetLockedOnWidgetComponent->SetWidgetSpace(EWidgetSpace::Screen);
	TargetLockedOnWidgetComponent->SetupAttachment(ParentComponent, LockedOnWidgetParentSocket);
	TargetLockedOnWidgetComponent->SetRelativeLocation(LockedOnWidgetRelativeLocation);
	TargetLockedOnWidgetComponent->SetDrawSize(FVector2D(LockedOnWidgetDrawSize, LockedOnWidgetDrawSize));
	TargetLockedOnWidgetComponent->SetVisibility(true);
	TargetLockedOnWidgetComponent->RegisterComponent();
}

TArray<AActor*> UTargetingComponent::GetAllActorsOfClass(const TSubclassOf<AActor> ActorClass) const
{
	TArray<AActor*> Actors;
	for (TActorIterator<AActor> ActorIterator(GetWorld(), ActorClass); ActorIterator; ++ActorIterator)
	{
		AActor* Actor = *ActorIterator;
		const bool bIsTargetable = TargetIsTargetable(Actor);
		if (bIsTargetable)
		{
			Actors.Add(Actor);
		}
	}

	return Actors;
}

bool UTargetingComponent::TargetIsTargetable(const AActor* Actor)
{
	const bool bIsImplemented = Actor->GetClass()->ImplementsInterface(UTargetableInterface::StaticClass());
	if (bIsImplemented)
	{
		return ITargetableInterface::Execute_IsTargetable(Actor);
	}

	return true;
}

void UTargetingComponent::SetupLocalPlayerController()
{
	if (!IsValid(OwnerPawn))
	{
		// TS_LOG(Error, TEXT("[%s] TargetSystemComponent: Component is meant to be added to Pawn only ..."), *GetName());
		return;
	}

	OwnerPlayerController = Cast<APlayerController>(OwnerPawn->GetController());
}

AActor* UTargetingComponent::FindNearestTarget(TArray<AActor*> Actors) const
{
	TArray<AActor*> ActorsHit;

	// Find all actors we can line trace to and are in the 180 degree cone
	for (AActor* Actor : Actors)
	{
		const float Angle = FMath::Abs(GetAngleUsingCameraRotation(Actor));
		if (Angle > 90.0f)
		{
			continue;
		}

		TArray<AActor*> ActorsToIgnore;
		const bool bHit = LineTraceForActor(Actor, ActorsToIgnore);
		if (bHit && IsInViewport(Actor))
		{
			ActorsHit.Add(Actor);
		}
	}

	// From the hit actors, check distance and angle to return the "best" one
	// "Closest target that is most directly in front"
	if (ActorsHit.Num() == 0)
	{
		return nullptr;
	}

	float BestScore = FLT_MAX;
	AActor* Target = nullptr;
	for (AActor* Actor : ActorsHit)
	{
		const float Distance = GetDistanceFromCharacter(Actor);
		const float Angle = FMath::Abs(GetAngleUsingCameraRotation(Actor));

		// Normalize distance and angle to some extent, or just use a weighted sum
		// Higher importance to angle since it says "most directly in front"
		// Distance is up to MinimumDistanceToEnable (e.g. 1200)
		// Angle is up to 90
		
		// Simple scoring: prioritize closer and more centered.
		// We can use distance * (1 + angle/90) or something similar.
		// Or just angle + (distance / MaxDistance) * some_weight
		
		const float NormalizedDistance = Distance / MinimumDistanceToEnable;
		const float NormalizedAngle = Angle / 90.0f;
		
		// Weighting: 1.0 for angle, 0.5 for distance? 
		// "Closest target that is most directly in front" suggests angle might be more important.
		const float Score = NormalizedAngle + (NormalizedDistance * 0.5f);

		if (Score < BestScore)
		{
			BestScore = Score;
			Target = Actor;
		}
	}

	return Target;
}


bool UTargetingComponent::LineTraceForActor(const AActor* OtherActor, const TArray<AActor*>& ActorsToIgnore) const
{
	FHitResult HitResult;
	const bool bHit = LineTrace(HitResult, OtherActor, ActorsToIgnore);
	if (bHit)
	{
		const AActor* HitActor = HitResult.GetActor();
		if (HitActor == OtherActor)
		{
			return true;
		}
	}

	return false;
}

bool UTargetingComponent::LineTrace(FHitResult& OutHitResult, const AActor* OtherActor, const TArray<AActor*>& ActorsToIgnore) const
{
	FCollisionQueryParams Params = FCollisionQueryParams(FName("LineTraceSingle"));

	TArray<AActor*> IgnoredActors;
	IgnoredActors.Init(OwnerActor, 1);
	IgnoredActors += ActorsToIgnore;

	Params.AddIgnoredActors(IgnoredActors);

	FVector StartLocation = OwnerActor->GetActorLocation();
	FVector CameraLocation;
	FRotator CameraRotation;
	if (GetCameraLocationAndRotation(CameraLocation, CameraRotation))
	{
		StartLocation = CameraLocation;
	}
	
	return GetWorld()->LineTraceSingleByChannel(
		OutHitResult,
		StartLocation,
		GetTargetLocation(OtherActor),
		TargetableCollisionChannel,
		Params
	);
}

FRotator UTargetingComponent::GetControlRotationOnTarget(const AActor* OtherActor) const
{
	if (!IsValid(OwnerPlayerController))
	{
		// TS_LOG(Warning, TEXT("UTargetingComponent::GetControlRotationOnTarget - OwnerPlayerController is not valid ..."))
		return FRotator::ZeroRotator;
	}

	const FRotator ControlRotation = OwnerPlayerController->GetControlRotation();

	FVector CameraLocation;
	FRotator CameraRotation;
	if (!GetCameraLocationAndRotation(CameraLocation, CameraRotation))
	{
		CameraLocation = OwnerActor->GetActorLocation();
	}

	const FVector TargetLocation = GetTargetLocation(OtherActor);

	if (FVector::Dist(CameraLocation, TargetLocation) < MinimumDistanceToUpdateRotation)
	{
		return ControlRotation;
	}

	// Find look at rotation
	const FRotator LookRotation = FRotationMatrix::MakeFromX(TargetLocation - CameraLocation).Rotator();
	const FRotator StableLookRotation = FRotationMatrix::MakeFromX(TargetLocation - OwnerActor->GetActorLocation()).Rotator();

	float Yaw = StableLookRotation.Yaw;
	if (bShouldControlRotation)
	{
		Yaw = LookRotation.Yaw;
	}

	// Account for camera side offset to frame the target correctly.
	if (bShouldControlRotation || !bAllowControlRotation)
	{
		if (const UAlsCameraComponent* AlsCamera = OwnerActor->FindComponentByClass<UAlsCameraComponent>())
		{
			if (const UAnimInstance* AnimInstance = AlsCamera->GetAnimInstance())
			{
				const float SideOffset = AnimInstance->GetCurveValue(UAlsCameraConstants::CameraOffsetYCurveName());
				if (!FMath::IsNearlyZero(SideOffset))
				{
					const float DistanceToTarget = FVector::Dist(OwnerActor->GetActorLocation(), TargetLocation);
					if (DistanceToTarget > KINDA_SMALL_NUMBER)
					{
						// Calculate the angle needed to compensate for the side offset
						// SideOffset = DistanceToTarget * sin(Angle)
						// Angle = asin(SideOffset / DistanceToTarget)
						const float OffsetAngleRadians = FMath::Asin(FMath::Clamp(SideOffset / DistanceToTarget, -1.0f, 1.0f));
						Yaw -= FMath::RadiansToDegrees(OffsetAngleRadians);
					}
				}
			}
		}
	}

	float Pitch = LookRotation.Pitch;
	FRotator TargetRotation;
	if (bAdjustPitchBasedOnDistanceToTarget)
	{
		const float DistanceToTarget = GetDistanceFromCharacter(OtherActor);
		const float PitchInRange = (DistanceToTarget * PitchDistanceCoefficient + PitchDistanceOffset) * -1.0f;
		const float PitchOffset = FMath::Clamp(PitchInRange, PitchMin, PitchMax);

		Pitch = Pitch + PitchOffset;
		TargetRotation = FRotator(Pitch, Yaw, ControlRotation.Roll);
	}
	else
	{
		if (bIgnoreLookInput)
		{
			TargetRotation = FRotator(Pitch, Yaw, ControlRotation.Roll);
		}
		else
		{
			TargetRotation = FRotator(ControlRotation.Pitch, Yaw, ControlRotation.Roll);
		}
	}

	return FMath::RInterpTo(ControlRotation, TargetRotation, GetWorld()->GetDeltaSeconds(), 9.0f);
}

void UTargetingComponent::SetControlRotationOnTarget(AActor* TargetActor) const
{
	if (!IsValid(OwnerPlayerController))
	{
		return;
	}

	const FRotator ControlRotation = GetControlRotationOnTarget(TargetActor);
	if (OnTargetSetRotation.IsBound())
	{
		OnTargetSetRotation.Broadcast(TargetActor, ControlRotation);
	}
	else
	{
		OwnerPlayerController->SetControlRotation(ControlRotation);
	}
}

float UTargetingComponent::GetDistanceFromCharacter(const AActor* OtherActor) const
{
	FVector CameraLocation;
	FRotator CameraRotation;
	if (GetCameraLocationAndRotation(CameraLocation, CameraRotation))
	{
		return FVector::Dist(CameraLocation, GetTargetLocation(OtherActor));
	}
	
	return FVector::Dist(OwnerActor->GetActorLocation(), GetTargetLocation(OtherActor));
}

bool UTargetingComponent::GetCameraLocationAndRotation(FVector& OutLocation, FRotator& OutRotation) const
{
	if (IsValid(OwnerPlayerController) && IsValid(OwnerPlayerController->PlayerCameraManager))
	{
		OutLocation = OwnerPlayerController->PlayerCameraManager->GetCameraLocation();
		OutRotation = OwnerPlayerController->PlayerCameraManager->GetCameraRotation();
		return true;
	}

	if (const UCameraComponent* CameraComponent = OwnerActor->FindComponentByClass<UCameraComponent>())
	{
		OutLocation = CameraComponent->GetComponentLocation();
		OutRotation = CameraComponent->GetComponentRotation();
		return true;
	}

	return false;
}

FVector UTargetingComponent::GetTargetLocation(const AActor* OtherActor) const
{
	if (!IsValid(OtherActor))
	{
		return FVector::ZeroVector;
	}

	if (OtherActor->Implements<UTargetableInterface>())
	{
		return ITargetableInterface::Execute_GetAimPoint(OtherActor);
	}

	return OtherActor->GetActorLocation();
}

bool UTargetingComponent::ShouldBreakLineOfSight() const
{
	if (!LockedOnTargetActor)
	{
		return true;
	}

	TArray<AActor*> ActorsToIgnore = GetAllActorsOfClass(TargetableActors);
	ActorsToIgnore.Remove(LockedOnTargetActor);

	FHitResult HitResult;
	const bool bHit = LineTrace(HitResult, LockedOnTargetActor, ActorsToIgnore);
	if (bHit && HitResult.GetActor() != LockedOnTargetActor)
	{
		return true;
	}

	return false;
}

void UTargetingComponent::BreakLineOfSight()
{
	bIsBreakingLineOfSight = false;
	if (ShouldBreakLineOfSight())
	{
		TargetLockOff();
	}
}

void UTargetingComponent::ControlRotation(const bool ShouldControlRotation) const
{
	if (!IsValid(OwnerPawn))
	{
		return;
	}

	OwnerPawn->bUseControllerRotationYaw = ShouldControlRotation;

	UCharacterMovementComponent* CharacterMovementComponent = OwnerPawn->FindComponentByClass<UCharacterMovementComponent>();
	if (CharacterMovementComponent)
	{
		CharacterMovementComponent->bOrientRotationToMovement = !ShouldControlRotation;
	}
}

bool UTargetingComponent::IsInViewport(const AActor* TargetActor) const
{
	if (!IsValid(OwnerPlayerController))
	{
		return true;
	}

	FVector2D ScreenLocation;
	OwnerPlayerController->ProjectWorldLocationToScreen(GetTargetLocation(TargetActor), ScreenLocation);

	FVector2D ViewportSize;
	GetWorld()->GetGameViewport()->GetViewportSize(ViewportSize);

	return ScreenLocation.X > 0 && ScreenLocation.Y > 0 && ScreenLocation.X < ViewportSize.X && ScreenLocation.Y < ViewportSize.Y;
}
