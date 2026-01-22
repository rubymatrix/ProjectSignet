// Copyright Epic Games, Inc. All Rights Reserved.

#include "SignetFaceActorComponent.h"

#include "GameFramework/Actor.h"

USignetFaceActorComponent::USignetFaceActorComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled = true;

	RotationInterpSpeed = 6.0f;
	bYawOnly = true;
	MinDistanceToUpdate = 0.0f;

	TargetActor = nullptr;
}

void USignetFaceActorComponent::SetTargetActor(AActor* NewTargetActor)
{
	TargetActor = NewTargetActor;
}

void USignetFaceActorComponent::ClearTargetActor()
{
	TargetActor = nullptr;
}

AActor* USignetFaceActorComponent::GetTargetActor() const
{
	return TargetActor;
}

bool USignetFaceActorComponent::HasTargetActor() const
{
	return IsValid(TargetActor);
}

void USignetFaceActorComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!IsValid(TargetActor))
	{
		return;
	}

	AActor* OwnerActor = GetOwner();
	if (!IsValid(OwnerActor))
	{
		return;
	}

	const FVector OwnerLocation = OwnerActor->GetActorLocation();
	const FVector TargetLocation = TargetActor->GetActorLocation();
	const FVector ToTarget = TargetLocation - OwnerLocation;

	if (MinDistanceToUpdate > 0.0f && ToTarget.SizeSquared() < FMath::Square(MinDistanceToUpdate))
	{
		return;
	}

	FRotator DesiredRotation = ToTarget.Rotation();
	if (bYawOnly)
	{
		DesiredRotation.Pitch = 0.0f;
		DesiredRotation.Roll = 0.0f;
	}

	const FRotator CurrentRotation = OwnerActor->GetActorRotation();
	const float InterpSpeed = FMath::Max(0.0f, RotationInterpSpeed);
	const FRotator NewRotation = (InterpSpeed > 0.0f) ? FMath::RInterpTo(CurrentRotation, DesiredRotation, DeltaTime, InterpSpeed) : DesiredRotation;

	OwnerActor->SetActorRotation(NewRotation);
}

