#pragma once

#include "CoreMinimal.h"
#include "GameplayEffect.h"
#include "GameplayTagContainer.h"
#include "GameplayEffectComponents/TargetTagsGameplayEffectComponent.h"
#include "SignetGame/Abilities/TagCache.h"
#include "GE_State_Stunned.generated.h"

UCLASS()
class SIGNETGAME_API UGE_State_Stunned : public UGameplayEffect
{
	GENERATED_BODY()

public:

	UGE_State_Stunned()
	{
		DurationPolicy = EGameplayEffectDurationType::Infinite;

		const auto TargetTagsComp = CreateDefaultSubobject<UTargetTagsGameplayEffectComponent>(TEXT("TargetTagsComp"));

		FInheritedTagContainer Container;
		Container.Added.AddTag(FTagCache::Get().State.Stunned);
		TargetTagsComp->SetAndApplyTargetTagChanges(Container);

		GEComponents.Add(TargetTagsComp);
	}
};