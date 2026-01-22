// Copyright Epic Games, Inc. All Rights Reserved.

#include "SignetSplineFollowingComponent.h"
#include "AIController.h"
#include "Components/SplineComponent.h"
#include "Engine/Engine.h"
#include "GameFramework/Pawn.h"

USignetSplineFollowingComponent::USignetSplineFollowingComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	LookAheadDistance = 300.0f;

	AcceptanceRadius = 75.0f;

	StuckTimeout = 1.5f;

	ProgressEpsilon = 5.0f;

	bShowDebug = false;

	SplineLength = 0.0f;

	CurrentDistance = 0.0f;

	LastProgressDistance = 0.0f;

	TimeSinceProgress = 0.0f;

	bReverse = false;

	bIsFollowing = false;
}

void USignetSplineFollowingComponent::BeginPlay()
{
	Super::BeginPlay();

	CachedController = Cast<AAIController>(GetOwner());
}

void USignetSplineFollowingComponent::FollowPath(USplineComponent* SplineComp, bool bReversePath)
{
	if (!GetOwner() || !GetOwner()->HasAuthority())
	{
		return;
	}

	if (!IsValid(SplineComp))
	{
		StopFollowing();
		return;
	}

	ActiveSpline = SplineComp;
	SplineLength = SplineComp->GetSplineLength();
	bReverse = bReversePath;

	APawn* Pawn = ResolvePawn();
	if (!IsValid(Pawn))
	{
		StopFollowing();
		return;
	}

	const FVector PawnLocation = Pawn->GetActorLocation();
	const float InputKey = SplineComp->FindInputKeyClosestToWorldLocation(PawnLocation);
	CurrentDistance = SplineComp->GetDistanceAlongSplineAtSplineInputKey(InputKey);
	LastProgressDistance = CurrentDistance;
	TimeSinceProgress = 0.0f;

	bIsFollowing = true;
	UpdateMoveTarget();
}

void USignetSplineFollowingComponent::StopFollowing()
{
	if (GetOwner() && !GetOwner()->HasAuthority())
	{
		return;
	}

	AAIController* Controller = ResolveController();
	if (IsValid(Controller))
	{
		Controller->StopMovement();
	}

	bIsFollowing = false;
	ActiveSpline.Reset();
	SplineLength = 0.0f;
	CurrentDistance = 0.0f;
	LastProgressDistance = 0.0f;
	TimeSinceProgress = 0.0f;
}

bool USignetSplineFollowingComponent::IsFollowing() const
{
	return bIsFollowing;
}

void USignetSplineFollowingComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!GetOwner() || !GetOwner()->HasAuthority())
	{
		return;
	}

	if (!bIsFollowing)
	{
		return;
	}

	APawn* Pawn = ResolvePawn();
	USplineComponent* SplineComp = ActiveSpline.Get();
	if (!IsValid(Pawn) || !IsValid(SplineComp))
	{
		StopFollowing();
		return;
	}

	const FVector PawnLocation = Pawn->GetActorLocation();
	if (HasReachedEnd(PawnLocation))
	{
		bIsFollowing = false;
		USplineComponent* CompletedSpline = ActiveSpline.Get();
		StopFollowing();
		OnPathCompleted.Broadcast(CompletedSpline);
		return;
	}

	const float InputKey = SplineComp->FindInputKeyClosestToWorldLocation(PawnLocation);
	CurrentDistance = SplineComp->GetDistanceAlongSplineAtSplineInputKey(InputKey);

	if (FMath::Abs(CurrentDistance - LastProgressDistance) <= ProgressEpsilon)
	{
		TimeSinceProgress += DeltaTime;
	}
	else
	{
		TimeSinceProgress = 0.0f;
		LastProgressDistance = CurrentDistance;
	}

	UpdateMoveTarget();

	if (bShowDebug && GEngine)
	{
		const float Remaining = bReverse ? CurrentDistance : (SplineLength - CurrentDistance);
		GEngine->AddOnScreenDebugMessage(
			reinterpret_cast<uint64>(this),
			0.0f,
			FColor::Green,
			FString::Printf(TEXT("Spline %.1f/%.1f (Remaining %.1f)"), CurrentDistance, SplineLength, Remaining));
	}
}

AAIController* USignetSplineFollowingComponent::ResolveController() const
{
	AAIController* Controller = CachedController.Get();
	if (!IsValid(Controller))
	{
		Controller = Cast<AAIController>(GetOwner());
	}

	return Controller;
}

APawn* USignetSplineFollowingComponent::ResolvePawn() const
{
	AAIController* Controller = ResolveController();
	if (!IsValid(Controller))
	{
		return nullptr;
	}

	return Controller->GetPawn();
}

void USignetSplineFollowingComponent::UpdateMoveTarget()
{
	AAIController* Controller = ResolveController();
	USplineComponent* SplineComp = ActiveSpline.Get();
	APawn* Pawn = ResolvePawn();
	if (!IsValid(Controller) || !IsValid(SplineComp) || !IsValid(Pawn))
	{
		return;
	}

	const float Direction = bReverse ? -1.0f : 1.0f;
	const float EndDistance = bReverse ? 0.0f : SplineLength;
	const float DistanceToEnd = FMath::Abs(EndDistance - CurrentDistance);
	const float DesiredDistanceUnclamped = CurrentDistance + (Direction * LookAheadDistance);
	float DesiredDistance = FMath::Clamp(DesiredDistanceUnclamped, 0.0f, SplineLength);

	if (DistanceToEnd <= LookAheadDistance)
	{
		DesiredDistance = EndDistance;
	}

	const FVector DesiredLocation = SplineComp->GetLocationAtDistanceAlongSpline(DesiredDistance, ESplineCoordinateSpace::World);
	Controller->MoveToLocation(DesiredLocation, AcceptanceRadius);
}

bool USignetSplineFollowingComponent::HasReachedEnd(const FVector& PawnLocation) const
{
	USplineComponent* SplineComp = ActiveSpline.Get();
	if (!IsValid(SplineComp))
	{
		return true;
	}

	const float InputKey = SplineComp->FindInputKeyClosestToWorldLocation(PawnLocation);
	const float DistanceAlongSpline = SplineComp->GetDistanceAlongSplineAtSplineInputKey(InputKey);
	const float EndDistance = bReverse ? 0.0f : SplineLength;
	const float DistanceToEnd = FMath::Abs(EndDistance - DistanceAlongSpline);

	if (DistanceToEnd <= AcceptanceRadius)
	{
		return true;
	}

	if (TimeSinceProgress >= StuckTimeout && DistanceToEnd <= LookAheadDistance)
	{
		return true;
	}

	return false;
}
