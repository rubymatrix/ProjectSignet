// Copyright Red Lotus Games, All Rights Reserved.

#include "SignetPlayerCharacter.h"


ASignetPlayerCharacter::ASignetPlayerCharacter()
{
	auto CreateMeshObject = [&](TObjectPtr<USkeletalMeshComponent> MeshComp, FName InName, FName Tag)
	{
		MeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(InName);
		MeshComp->SetupAttachment(GetMesh());
		MeshComp->SetVisibility(false);
		MeshComp->SetComponentTickEnabled(false);
		MeshComp->ComponentTags.Add(Tag);
	};

	CreateMeshObject(MainMesh, TEXT("MainMesh"), TEXT("Main"));
	CreateMeshObject(SubMesh, TEXT("SubMesh"), TEXT("Sub"));
	CreateMeshObject(RangedMesh, TEXT("RangedMesh"), TEXT("Ranged"));
	CreateMeshObject()
}

void ASignetPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}
