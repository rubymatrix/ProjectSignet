#pragma once

#include "CoreMinimal.h"
#include "StateAbility.h"
#include "SignetGame/Abilities/TagCache.h"
#include "SignetGame/Player/SignetPlayerCharacter.h"
#include "SignetGame/Player/Components/SignetCameraComponent.h"
#include "SignetGame/Player/Components/TargetingComponent.h"
#include "GA_Disengage.generated.h"

UCLASS()
class SIGNETGAME_API UGA_Disengage : public UStateAbility
{
	GENERATED_BODY()

public:

	UGA_Disengage() 
	{
		InstancingPolicy   = EGameplayAbilityInstancingPolicy::InstancedPerActor;
		NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::LocalPredicted;
	
		SetAssetTags(FGameplayTagContainer(FTagCache::Get().Ability.Disengage));
	
		ActivationRequiredTags.AddTag(FTagCache::Get().State.Engaged);
		ActivationBlockedTags.AddTag(FTagCache::Get().State.Idle);

		// Config
		ActionType = EMontageType::Disengage;
		TargetStateTag = FTagCache::Get().State.Idle;
		bTransitionState = true;
		bTransitionBeforeMontage = true;
	}

protected:
	
	virtual void OnMontageBlendIn() override
	{
		Super::OnMontageBlendIn();

		if (const auto Avatar = Cast<ASignetPlayerCharacter>(GetAvatarActorFromActorInfo()))
		{
			if (const auto Targeting = Avatar->GetComponentByClass<UTargetingComponent>())
			{
				Targeting->bAllowControlRotation = false;
			}

			Avatar->SetDesiredRotationMode(AlsRotationModeTags::VelocityDirection);

			if (const auto Cam = Avatar->GetComponentByClass<USignetCameraComponent>())
			{
				Cam->NotifyEngaged(false);
			}
		}
	}

};