// Copyright Red Lotus Games, All Rights Reserved.


#include "AnimNotify_Vocalization.h"

#include "Kismet/GameplayStatics.h"
#include "SignetGame/Combat/CombatInterface.h"
#include "SignetGame/Data/DataHelpers.h"
#include "SignetGame/Data/GameDataSubsystem.h"
#include "SignetGame/Player/Components/CharacterDataComponent.h"
#include "Utility/AlsEnumUtility.h"

FString UAnimNotify_Vocalization::GetNotifyName_Implementation() const
{
	TStringBuilder<64> NotifyNameBuilder{InPlace, TEXTVIEW("Vocalization: "), AlsEnumUtility::GetNameStringByValue(VocalType)};
	return FString{NotifyNameBuilder};
}

#if WITH_EDITOR
void UAnimNotify_Vocalization::OnAnimNotifyCreatedInEditor(FAnimNotifyEvent& NotifyEvent)
{
	Super::OnAnimNotifyCreatedInEditor(NotifyEvent);
	NotifyEvent.bTriggerOnDedicatedServer = false;
}
#endif

void UAnimNotify_Vocalization::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	// Check if it's editor preview, if so, spawn the sound 2D
	const auto World = MeshComp->GetWorld();
	if (World->WorldType == EWorldType::EditorPreview)
	{
		if (bDisableInEditor) return;
		if (!World) return;

#if WITH_EDITOR

		const auto RowName = Data::GetRowNameFromEnum(EditorRace);
		if (!RowName.IsValid()) return;
		if (!CharacterPartsTable) return;

		if (const auto Parts = CharacterPartsTable->FindRow<FCharacterPartsRow>(RowName, TEXT("PartsRowLookup")))
		{
			if (Parts->Voices.Num() < 1) return;

			FVoiceBank VoiceBank;
			if (Parts->Voices.Num() < FaceToIndex())
			{
				VoiceBank = Parts->Voices[0];
			}
			else
			{
				VoiceBank = Parts->Voices[FaceToIndex()];
			}

			if (!VoiceBank.HasVocal(VocalType)) return;

			const auto Sound = VoiceBank.Vocals[VocalType].Sounds[0];

			UGameplayStatics::SpawnSound2D(World, Sound);
		}

#endif
		
	}
	else
	{
		// Forward the command to play the vocalization on the combatant
		if (const auto Combatant = Cast<ICombatInterface>(MeshComp->GetOwner()))
		{
			Combatant->PlayVocalization(VocalType);
		}
	}
}

int UAnimNotify_Vocalization::FaceToIndex() const
{
	switch (EditorFaceIndex)
	{
	case EFace::F1A:
	case EFace::F1B: return 0;
	case EFace::F2A:
	case EFace::F2B: return 1;
	case EFace::F3A:
	case EFace::F3B: return 2;
	case EFace::F4A:
	case EFace::F4B: return 3;
	case EFace::F5A:
	case EFace::F5B: return 4;
	case EFace::F6A:
	case EFace::F6B: return 5;
	case EFace::F7A:
	case EFace::F7B: return 6;
	case EFace::F8A:
	case EFace::F8B: return 7;
	default: return 0;
	}
}

