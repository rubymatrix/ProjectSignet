// Copyright Red Lotus Games, All Rights Reserved.

#include "EffectComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraSystem.h"
#include "Components/CapsuleComponent.h"
#include "SignetGame/Data/GameDataSubsystem.h"
#include "SignetGame/Player/Components/TargetingComponent.h"

void UEffectComponent::SpawnEffectFromGlobalPool(const EVFXType VFXType)
{
	Internal_SpawnEffectFromGlobalPool(VFXType, GetOwner(), GetTargetActor());
}

void UEffectComponent::SpawnEffectFromGlobalPoolWithParams(const EVFXType VFXType, const AActor* SourceActor,
	const AActor* TargetActor)
{
	Internal_SpawnEffectFromGlobalPool(VFXType, SourceActor, TargetActor);
}

void UEffectComponent::Internal_SpawnEffectFromGlobalPool(const EVFXType VFXType, const AActor* SourceActor,
	const AActor* TargetActor)
{
	const auto World = GetWorld();
	if (World == nullptr) return;

	const auto Data = World->GetGameInstance()->GetSubsystem<UGameDataSubsystem>();
	if (Data == nullptr) return;

	if (const auto System = Data->GetGlobalVFX(VFXType))
	{
		FTransform SpawnTransform;

		switch (System->AttachmentType)
		{
		case EVFXAttachmentType::SelfOrigin:
			SpawnTransform = GetActorOriginTransform(SourceActor);
			break;
		case EVFXAttachmentType::TargetOrigin:
			SpawnTransform = GetActorOriginTransform(TargetActor);
			break;
		case EVFXAttachmentType::SelfSocket:
			SpawnTransform = GetActorSocketTransform(SourceActor, System->SocketName);
			break;
		case EVFXAttachmentType::TargetSocket:
			SpawnTransform = GetActorSocketTransform(TargetActor, System->SocketName);
			break;
		case EVFXAttachmentType::TargetVector:
			{
				float TargetHeight = HeightOffset;
				if (TargetActor)
				{
					if (const auto Capsule = SourceActor->FindComponentByClass<UCapsuleComponent>())
					{
						TargetHeight += Capsule->GetScaledCapsuleHalfHeight() / 2;
					}
					else
					{
						TargetHeight += SourceActor->GetSimpleCollisionHalfHeight() / 2;
					}
				}
				SpawnTransform = GetTargetVectorTransform(GetActorOriginTransform(SourceActor), GetActorOriginTransform(TargetActor), System->VectorOffset + DirectionOffset, TargetHeight);
				SpawnTransform.MultiplyScale3D(System->Scale);
			}
			break;
		default:
			SpawnTransform = GetActorOriginTransform(SourceActor);
			break;
		}

		if (System->System)
		{
			SpawnNiagaraSystemAtLocation(System->System, SpawnTransform);
		}
	}
}

AActor* UEffectComponent::GetTargetActor() const
{
	if (const auto Targeting = GetOwner()->GetComponentByClass<UTargetingComponent>())
	{
		return Targeting->GetLockedOnTargetActor();
	}
	return nullptr;
}

FTransform UEffectComponent::GetActorOriginTransform(const AActor* Actor) const
{
	if (Actor)
	{
		return Actor->GetActorTransform();
	}
	return GetOwner()->GetActorTransform();
}

FTransform UEffectComponent::GetActorSocketTransform(const AActor* Actor, const FName SocketName) const
{
	if (Actor)
	{
		if (const auto Skel = Actor->FindComponentByClass<USkeletalMeshComponent>())
		{
			return Skel->GetSocketTransform(SocketName);
		}
		return Actor->GetActorTransform();
	}

	return GetOwner()->GetActorTransform();
}

FTransform UEffectComponent::GetTargetVectorTransform(const FTransform& Origin, const FTransform& Destination, const float Offset, const float TargetHeight) const
{
	const FVector OriginLocation = Origin.GetLocation();
	FVector DestinationLocation = Destination.GetLocation();

	// Adjust the Z-axis to be half the height of the target model
	DestinationLocation.Z += TargetHeight;
	
	const FVector Direction = (OriginLocation - DestinationLocation).GetSafeNormal();
	
	// Determine a point close to the destination that is directly between the origin and the destination
	// We'll use the provided offset from the destination towards the origin
	const FVector VectorLocation = DestinationLocation + (Direction * Offset);

	// The transform should be facing the origin
	const FRotator VectorRotation = Direction.Rotation();
	
	return FTransform(VectorRotation, VectorLocation);
}

void UEffectComponent::SpawnNiagaraSystemAtLocation(UNiagaraSystem* System, const FTransform& Transform)
{
	if (System == nullptr) return;

	UNiagaraFunctionLibrary::SpawnSystemAtLocation(
		GetWorld(),
		System,
		Transform.GetLocation(),
		Transform.Rotator(),
		Transform.GetScale3D()
	);
}
