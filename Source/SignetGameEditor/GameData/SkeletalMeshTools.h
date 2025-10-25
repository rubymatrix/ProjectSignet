// Copyright Red Lotus Games, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Engine/SkeletalMesh.h"
#include "SkeletalMeshTools.generated.h"

/**
 * 
 */
UCLASS()
class SIGNETGAMEEDITOR_API USkeletalMeshTools : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	
	UFUNCTION(BlueprintCallable, Category="Signet|Skeleton|Editor", meta=(DevelopmentOnly))
	static void ReplaceSkeletonOnSelectedMeshes(USkeleton* NewSkeleton, bool bAutoMergeBonesToSkeleton = true, bool bSaveAssets = true);
	
	UFUNCTION(BlueprintCallable, Category="Signet|Skeleton|Editor", meta=(DevelopmentOnly))
	static bool ReplaceSkeletonOnMesh(USkeletalMesh* Mesh, USkeleton* NewSkeleton, bool bAutoMergeBonesToSkeleton = true, bool bSaveAsset = true);

private:
	static bool IsMeshCompatibleWithSkeleton(const USkeletalMesh* Mesh, const USkeleton* NewSkeleton);
	static void SavePackageFor(UObject* Asset);
};
