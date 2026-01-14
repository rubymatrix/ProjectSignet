// Copyright Red Lotus Games, All Rights Reserved.


#include "AnimNotifyState_DisableMovement.h"

#include "SignetGame/Player/SignetPlayerCharacter.h"
#include "SignetGame/Player/Components/SignetMovementComponent.h"

void UAnimNotifyState_DisableMovement::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                                   float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

	const auto Player = Cast<ASignetPlayerCharacter>(MeshComp->GetOwner());
	const auto bIsPlayer = Player != nullptr;

	if (bIsPlayer)
	{
		if (const auto CMC = Cast<USignetMovementComponent>(Player->GetCharacterMovement()))
		{
			CMC->bIsBound = true;
		}
	}
}

void UAnimNotifyState_DisableMovement::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);

	const auto Player = Cast<ASignetPlayerCharacter>(MeshComp->GetOwner());
	const auto bIsPlayer = Player != nullptr;

	if (bIsPlayer)
	{
		if (const auto CMC = Cast<USignetMovementComponent>(Player->GetCharacterMovement()))
		{
			CMC->bIsBound = false;
		}
	}
}

