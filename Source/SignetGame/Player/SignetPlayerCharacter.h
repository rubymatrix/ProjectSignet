// Copyright Red Lotus Games, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AlsCharacter.h"
#include "SignetPlayerCharacter.generated.h"


#define HIDDEN_PROPERTY() UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta=(HideInDetailPanel=true))

UCLASS()
class SIGNETGAME_API ASignetPlayerCharacter : public AAlsCharacter
{
	GENERATED_BODY()

// Begin Visual Mesh Systems
protected:

	
	HIDDEN_PROPERTY() TObjectPtr<USkeletalMeshComponent> MainMesh;
	HIDDEN_PROPERTY() TObjectPtr<USkeletalMeshComponent> SubMesh;
	HIDDEN_PROPERTY() TObjectPtr<USkeletalMeshComponent> RangedMesh;
	HIDDEN_PROPERTY() TObjectPtr<USkeletalMeshComponent> FaceMesh;
	HIDDEN_PROPERTY() TObjectPtr<USkeletalMeshComponent> HeadMesh;
	HIDDEN_PROPERTY() TObjectPtr<USkeletalMeshComponent> BodyMesh;
	HIDDEN_PROPERTY() TObjectPtr<USkeletalMeshComponent> HandsMesh;
	HIDDEN_PROPERTY() TObjectPtr<USkeletalMeshComponent> LegsMesh;
	HIDDEN_PROPERTY() TObjectPtr<USkeletalMeshComponent> FeetMesh;


	
public:

	ASignetPlayerCharacter();

protected:
	
	virtual void BeginPlay() override;
	
};

#undef HIDDEN_PROPERTY