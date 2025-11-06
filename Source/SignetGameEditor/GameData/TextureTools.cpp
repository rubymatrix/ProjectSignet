// Copyright Red Lotus Games, All Rights Reserved.


#include "TextureTools.h"
#include "Editor.h"
#include "EditorUtilityLibrary.h"
#include "AssetRegistry/AssetRegistryModule.h"
#include "Misc/MessageDialog.h"

void UTextureTools::SetSelectedTexturesToUIGroup()
{
	TArray<UObject*> SelectedAssets = UEditorUtilityLibrary::GetSelectedAssets();

	int32 UpdatedCount = 0;

	for (UObject* Asset : SelectedAssets)
	{
		if (UTexture* Texture = Cast<UTexture>(Asset))
		{
			if (Texture->LODGroup != TEXTUREGROUP_UI)
			{
				Texture->Modify();
				Texture->LODGroup = TEXTUREGROUP_UI;
				Texture->PostEditChange();
				Texture->MarkPackageDirty();
				UpdatedCount++;
			}
		}
	}

	if (UpdatedCount > 0)
	{
		FText Message = FText::Format(
			NSLOCTEXT("EditorTextureTools", "SetSelectedTexturesToUIGroup",
			"Updated {0} texture(s) to LOD Group: UI."), UpdatedCount);

		FMessageDialog::Open(EAppMsgType::Ok, Message);
	}
	else
	{
		FMessageDialog::Open(EAppMsgType::Ok, 
			NSLOCTEXT("EditorTextureTools", "NoTexturesUpdated", "No textures were updated."));
	}
}