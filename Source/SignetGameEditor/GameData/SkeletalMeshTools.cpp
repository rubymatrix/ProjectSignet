// Copyright Red Lotus Games, All Rights Reserved.


#include "SkeletalMeshTools.h"
#include "ContentBrowserModule.h"
#include "IContentBrowserSingleton.h"
#include "AssetRegistry/AssetRegistryModule.h"
#include "Editor.h"
#include "ScopedTransaction.h"
#include "Misc/ScopedSlowTask.h"
#include "Misc/MessageDialog.h"
#include "HAL/PlatformApplicationMisc.h"
#include "FileHelpers.h"
#include "SignetGameEditor.h"
#include "Logging/MessageLog.h"

#define LOCTEXT_NAMESPACE "Signet.SkeletalMeshTools"

bool USkeletalMeshTools::IsMeshCompatibleWithSkeleton(const USkeletalMesh* Mesh, const USkeleton* NewSkeleton)
{
	if (!Mesh || !NewSkeleton) return false;

	// Quick sanity: require root bone name to exist in target skeleton.
	const FReferenceSkeleton& MeshRefSkel = Mesh->GetRefSkeleton();
	const FReferenceSkeleton& TargetRefSkel = NewSkeleton->GetReferenceSkeleton();

	if (MeshRefSkel.GetNum() == 0 || TargetRefSkel.GetNum() == 0)
	{
		return false;
	}

	const FName MeshRoot = MeshRefSkel.GetBoneName(0);
	const int32 TargetRootIdx = TargetRefSkel.FindBoneIndex(MeshRoot);
	return TargetRootIdx != INDEX_NONE;
}

void USkeletalMeshTools::SavePackageFor(UObject* Asset)
{
	if (!Asset) return;
	UPackage* Pkg = Asset->GetOutermost();
	if (!Pkg) return;

	Pkg->SetDirtyFlag(true);
	TArray<UPackage*> PackagesToSave { Pkg };
	FEditorFileUtils::PromptForCheckoutAndSave(
		PackagesToSave,
		/*bCheckDirty*/ false,
		/*bPromptToSave*/ false
	);
}

bool USkeletalMeshTools::ReplaceSkeletonOnMesh(USkeletalMesh* Mesh, USkeleton* NewSkeleton, bool bAutoMergeBonesToSkeleton, bool bSaveAsset)
{
	if (!Mesh || !NewSkeleton)
	{
		UE_LOG(LogSignetGameEditor, Error, TEXT("ReplaceSkeletonOnMesh: Mesh or NewSkeleton is null."));
		return false;
	}

	if (Mesh->GetSkeleton() == NewSkeleton)
	{
		UE_LOG(LogSignetGameEditor, Display, TEXT("%s already uses %s."), *Mesh->GetName(), *NewSkeleton->GetName());
		return true;
	}
	
	if (bAutoMergeBonesToSkeleton)
	{
		NewSkeleton->MergeAllBonesToBoneTree(Mesh);
	}
	
	if (!IsMeshCompatibleWithSkeleton(Mesh, NewSkeleton))
	{
		UE_LOG(LogSignetGameEditor, Warning, TEXT("Root bone of %s does not exist in target Skeleton %s. Aborting."),
			*Mesh->GetName(), *NewSkeleton->GetName());
		return false;
	}

	const FScopedTransaction Tx(LOCTEXT("ReplaceSkeletonOnMeshTx", "Replace Skeleton on SkeletalMesh"));
	Mesh->Modify();

	// Assign and refresh
	Mesh->SetSkeleton(NewSkeleton);

	// Rebuild dependent data
	Mesh->InvalidateDeriveDataCacheGUID();
	Mesh->MarkPackageDirty();
	
	if (FSkeletalMeshRenderData* RenderData = Mesh->GetResourceForRendering())
	{
		(void)RenderData;
	}

	Mesh->PostEditChange();
	Mesh->MarkPackageDirty();

	if (bSaveAsset)
	{
		SavePackageFor(Mesh);
		SavePackageFor(NewSkeleton);
	}

	UE_LOG(LogSignetGameEditor, Display, TEXT("Reassigned %s to Skeleton %s."), *Mesh->GetName(), *NewSkeleton->GetName());
	return true;
}

void USkeletalMeshTools::ReplaceSkeletonOnSelectedMeshes(USkeleton* NewSkeleton, bool bAutoMergeBonesToSkeleton, bool bSaveAssets)
{
	if (!NewSkeleton)
	{
		FMessageDialog::Open(EAppMsgType::Ok, LOCTEXT("NoSkeletonProvided", "No Skeleton specified."));
		return;
	}

	// Gather selected assets from Content Browser
	TArray<FAssetData> SelectedAssets;
	{
		FContentBrowserModule& CBModule = FModuleManager::LoadModuleChecked<FContentBrowserModule>("ContentBrowser");
		IContentBrowserSingleton& CB = CBModule.Get();
		CB.GetSelectedAssets(SelectedAssets);
	}

	TArray<USkeletalMesh*> Meshes;
	for (const FAssetData& AD : SelectedAssets)
	{
		if (USkeletalMesh* SM = Cast<USkeletalMesh>(AD.GetAsset()))
		{
			Meshes.Add(SM);
		}
	}

	if (Meshes.Num() == 0)
	{
		FMessageDialog::Open(EAppMsgType::Ok, LOCTEXT("NoSkeletalMeshes", "No Skeletal Meshes selected."));
		return;
	}

	FScopedSlowTask SlowTask(
		static_cast<float>(Meshes.Num()),
		FText::Format(LOCTEXT("ReplacingSkeletonsFmt", "Replacing Skeleton with {0}..."), FText::FromString(NewSkeleton->GetName()))
	);
	SlowTask.MakeDialog(true);

	int32 Successes = 0;
	for (USkeletalMesh* Mesh : Meshes)
	{
		SlowTask.EnterProgressFrame(1.f, FText::FromString(Mesh->GetName()));
		if (!Mesh) continue;

		const bool bOK = ReplaceSkeletonOnMesh(Mesh, NewSkeleton, bAutoMergeBonesToSkeleton, bSaveAssets);
		if (bOK) { ++Successes; }
	}

	const FText DoneMsg = FText::Format(
		LOCTEXT("ReplaceDone", "Skeleton replacement complete.\nSuccess: {0}/{1}"),
		FText::AsNumber(Successes),
		FText::AsNumber(Meshes.Num())
	);
	FMessageDialog::Open(EAppMsgType::Ok, DoneMsg);
}

#undef LOCTEXT_NAMESPACE