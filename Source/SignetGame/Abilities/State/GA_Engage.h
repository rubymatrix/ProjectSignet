#pragma once

#include "CoreMinimal.h"
#include "StateAbility.h"
#include "SignetGame/Abilities/TagCache.h"
#include "SignetGame/Player/SignetPlayerCharacter.h"
#include "SignetGame/Player/Components/SignetCameraComponent.h"
#include "SignetGame/Player/Components/TargetingComponent.h"
#include "GA_Engage.generated.h"

UCLASS()
class SIGNETGAME_API UGA_Engage : public UStateAbility
{
    GENERATED_BODY()

public:

    UGA_Engage() 
    {
        InstancingPolicy  = EGameplayAbilityInstancingPolicy::InstancedPerActor;
        NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::LocalPredicted;

        SetAssetTags(FGameplayTagContainer(FTagCache::Get().Ability.Engage));

        ActivationRequiredTags.AddTag(FTagCache::Get().State.Idle);
        ActivationBlockedTags.AddTag(FTagCache::Get().State.Dead);
        ActivationBlockedTags.AddTag(FTagCache::Get().State.Engaged);
        ActivationBlockedTags.AddTag(FTagCache::Get().State.Sitting);
        ActivationBlockedTags.AddTag(FTagCache::Get().State.Resting);
        ActivationBlockedTags.AddTag(FTagCache::Get().State.Crafting);
        ActivationBlockedTags.AddTag(FTagCache::Get().State.Mounted);
        ActivationBlockedTags.AddTag(FTagCache::Get().State.Cutscene);
        ActivationBlockedTags.AddTag(FTagCache::Get().State.Stunned);

        // Config
        ActionType = EMontageType::Engage;
        TargetStateTag = FTagCache::Get().State.Engaged;
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
                Targeting->bAllowControlRotation = true;
            }

            Avatar->SetDesiredRotationMode(AlsRotationModeTags::ViewDirection);

            if (const auto Cam = Avatar->GetComponentByClass<USignetCameraComponent>())
            {
                Cam->NotifyEngaged(true);
            }
            
        }
    }

};