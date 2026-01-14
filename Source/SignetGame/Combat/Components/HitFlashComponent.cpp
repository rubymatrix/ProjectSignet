// Copyright Red Lotus Games, All Rights Reserved.

#include "HitFlashComponent.h"

#include "Kismet/GameplayStatics.h"
#include "SignetGame/Inventory/InventoryTypes.h"

UHitFlashComponent::UHitFlashComponent()
{
}

void UHitFlashComponent::BeginPlay()
{
	Super::BeginPlay();

	for (const auto& Comp : GetOwner()->GetComponents())
	{
		if (const auto Mesh = Cast<USkeletalMeshComponent>(Comp))
		{
			const auto Materials = Mesh->GetMaterials();
			for (auto i = 0; i < Materials.Num(); ++i)
			{
				const auto Material = Materials[i];
				if (Cast<UMaterialInstance>(Material) == nullptr)
					continue;

				// It's a regular material instance
				if (const auto Mi = Cast<UMaterialInstance>(Material))
				{
					const auto Mid = UMaterialInstanceDynamic::Create(Mi, this, NAME_None);
					HitFlashMaterials.Add(Mid);
					Mesh->SetMaterial(i, Mid);
				}
			}
		}
	}
}

void UHitFlashComponent::TriggerHitFlash(float Duration)
{
	const auto World = GetWorld();
	if (!World) return;

	if (HitFlashTimer.IsValid() && World->GetTimerManager().IsTimerActive(HitFlashTimer))
	{
		return;		
	}

	StartFlashing();
	World->GetTimerManager().SetTimer(HitFlashTimer, this, &UHitFlashComponent::StopFlashing, Duration > 0.f ? Duration : FlashDuration, false);
}

void UHitFlashComponent::StartFlashing(float Duration)
{
	for (const auto& Material : HitFlashMaterials)
	{
		Material->SetScalarParameterValue(TEXT("HitFlashEnabled"), 1.f);
		Material->SetScalarParameterValue(TEXT("HitFlashStartTime"), UGameplayStatics::GetTimeSeconds(GetWorld()));
		Material->SetScalarParameterValue(TEXT("HitFlashDuration"), Duration > 0.f ? Duration : FlashDuration);
	}
}

void UHitFlashComponent::StopFlashing()
{
	for (const auto& Material : HitFlashMaterials)
	{
		Material->SetScalarParameterValue(TEXT("HitFlashEnabled"), 0.f);
	}
}
