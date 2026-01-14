// Copyright Red Lotus Games, All Rights Reserved.


#include "SignetEditorSubsystem.h"

#include "SignetGame/Data/DataHelpers.h"

void USignetEditorSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	if (!CharacterPartsTableRef.IsNull())
	{
		CharacterPartsTable = CharacterPartsTableRef.LoadSynchronous();
		UE_LOG(LogTemp, Display, TEXT("Editor Data: Loaded CharacterPartsTable: %s"),
			*GetNameSafe(CharacterPartsTable));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Editor Data: CharacterPartsTableRef is null (check INI)."));
	}
}

void USignetEditorSubsystem::Deinitialize()
{
	Super::Deinitialize();
	CharacterPartsTable = nullptr;
}

const FCharacterPartsRow* USignetEditorSubsystem::FindCharacterParts(ERace Race)
{
	if (CharacterPartsTable == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Editor Data: CharacterPartsTable is null, unable to FindCharacterParts."));
		return nullptr;
	}
	return CharacterPartsTable->FindRow<FCharacterPartsRow>(Data::GetRowNameFromEnum(Race), TEXT("CharacterPartsLookup"));
}

const USoundBase* USignetEditorSubsystem::FindVocalization(EVocalizationType Vocalization, const ERace& Race, const EFace& Face)
{
	if (const auto Parts = FindCharacterParts(Race))
	{
		if (Parts->Voices.Num() < 1) return nullptr;
		
		auto Index = Data::GetVocalIndexFromFaceType(Face);
		if (!Parts->Voices.IsValidIndex(Index))
		{
			Index = 0;
		}

		if (const auto VoiceBank = Parts->Voices[Index]; VoiceBank.HasVocal(Vocalization))
		{
			const auto Sounds = VoiceBank.Vocals[Vocalization].Sounds;
			return Sounds[FMath::RandRange(0, Sounds.Num() - 1)];
		}
	}
	return nullptr;
}
