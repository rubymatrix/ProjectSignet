// Copyright Red Lotus Games, All Rights Reserved.

#include "AnimNotify_WeaponWhoosh.h"
#include "SignetGame/Combat/CombatInterface.h"

void UAnimNotify_WeaponWhoosh::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if (!MeshComp) return;

	if (ICombatInterface* Combatant = Cast<ICombatInterface>(MeshComp->GetOwner()))
	{
		Combatant->AdvanceAttackResult();
		Combatant->PlayWeaponWhoosh();
	}
}
