#pragma once

#include "CoreMinimal.h"
#include "GameplayEffect.h"
#include "GameplayTagContainer.h"
#include "GameplayEffectComponents/TargetTagsGameplayEffectComponent.h"
#include "SignetGame/Abilities/TagCache.h"
#include "GE_State_Crafting.generated.h"

UCLASS()
class SIGNETGAME_API UGE_State_Crafting : public UGameplayEffect
{
	GENERATED_BODY()

public:

	UGE_State_Crafting()
	{
		DurationPolicy = EGameplayEffectDurationType::Infinite;

		const auto TargetTagsComp = CreateDefaultSubobject<UTargetTagsGameplayEffectComponent>(TEXT("TargetTagsComp"));

		FInheritedTagContainer Container;
		Container.Added.AddTag(FTagCache::Get().State.Crafting);
		TargetTagsComp->SetAndApplyTargetTagChanges(Container);

		GEComponents.Add(TargetTagsComp);
	}
};