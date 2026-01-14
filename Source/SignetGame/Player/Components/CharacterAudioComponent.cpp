// Copyright Red Lotus Games, All Rights Reserved.


#include "CharacterAudioComponent.h"
#include "Kismet/GameplayStatics.h"
#include "SignetGame/Data/GameDataSubsystem.h"

UCharacterAudioComponent::UCharacterAudioComponent()
{

}

void UCharacterAudioComponent::PlayUISound(EUISound SoundType)
{
	if (const UWorld* World = GetWorld())
	{
		if (UGameDataSubsystem* GameDataSubsystem = World->GetGameInstance()->GetSubsystem<UGameDataSubsystem>())
		{
			if (USoundBase* Sound = GameDataSubsystem->GetUISound(SoundType))
			{
				FAudioChannelConfig Settings;
				if (const FAudioChannelConfig* FoundSettings = AudioSettingsMap.Find(EAudioType::UI))
				{
					Settings = *FoundSettings;
				}
				
				UGameplayStatics::PlaySound2D(World, Sound, Settings.VolumeMultiplier, Settings.PitchMultiplier, Settings.StartTime, Settings.ConcurrencySettings);
			}
		}
	}
}


