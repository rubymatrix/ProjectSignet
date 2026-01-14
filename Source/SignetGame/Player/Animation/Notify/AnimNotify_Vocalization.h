// Copyright Red Lotus Games, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "SignetGame/Data/AudioTypes.h"
#include "SignetGame/Save/SignetSaveTypes.h"
#include "AnimNotify_Vocalization.generated.h"

enum class EFace : uint8;
/**
 * 
 */
UCLASS()
class SIGNETGAME_API UAnimNotify_Vocalization : public UAnimNotify
{
	GENERATED_BODY()

public:
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Editor")
	ERace EditorRace = ERace::HumeMale;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Editor")
	EFace EditorFaceIndex = EFace::F1A;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Editor")
	bool bDisableInEditor = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Editor")
	TObjectPtr<UDataTable> CharacterPartsTable;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EVocalizationType VocalType;

	virtual FString GetNotifyName_Implementation() const override;

#if WITH_EDITOR
	virtual void OnAnimNotifyCreatedInEditor(FAnimNotifyEvent& NotifyEvent) override;
#endif

	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

private:

	int FaceToIndex() const;
	
};
