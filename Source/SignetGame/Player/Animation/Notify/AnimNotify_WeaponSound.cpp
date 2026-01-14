// Copyright Red Lotus Games, All Rights Reserved.


#include "AnimNotify_WeaponSound.h"

FString UAnimNotify_WeaponSound::GetNotifyName_Implementation() const
{
	return Super::GetNotifyName_Implementation();
}

void UAnimNotify_WeaponSound::OnAnimNotifyCreatedInEditor(FAnimNotifyEvent& NotifyEvent)
{
	Super::OnAnimNotifyCreatedInEditor(NotifyEvent);
}

void UAnimNotify_WeaponSound::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);
}

void UAnimNotify_WeaponSound::SpawnSoundForEditor(USkeletalMeshComponent* MeshComp)
{
}
