// Copyright Red Lotus Games, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AnimNotify_WeaponSound.generated.h"

/**
 * 
 */
UCLASS()
class SIGNETGAME_API UAnimNotify_WeaponSound : public UAnimNotify
{
	GENERATED_BODY()

public:

	virtual FString GetNotifyName_Implementation() const override;

#if WITH_EDITOR
	virtual void OnAnimNotifyCreatedInEditor(FAnimNotifyEvent& NotifyEvent) override;
#endif

	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

private:

	void SpawnSoundForEditor(USkeletalMeshComponent* MeshComp);
};
