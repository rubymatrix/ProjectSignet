// Copyright Red Lotus Games, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "TextureTools.generated.h"

/**
 * 
 */
UCLASS()
class SIGNETGAMEEDITOR_API UTextureTools : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	UFUNCTION(CallInEditor, Category="Signet|Editor|Textures")
	static void SetSelectedTexturesToUIGroup();
};
