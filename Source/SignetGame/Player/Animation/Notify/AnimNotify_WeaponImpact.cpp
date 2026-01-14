// Copyright Red Lotus Games, All Rights Reserved.

#include "AnimNotify_WeaponImpact.h"
#include "SignetGame/Combat/CombatInterface.h"

void UAnimNotify_WeaponImpact::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if (!MeshComp) return;

	if (ICombatInterface* Combatant = Cast<ICombatInterface>(MeshComp->GetOwner()))
	{
		Combatant->PlayWeaponImpact();
	}
}
