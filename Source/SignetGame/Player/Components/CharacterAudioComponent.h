// Copyright Red Lotus Games, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SignetGame/Data/AudioTypes.h"
#include "CharacterAudioComponent.generated.h"




UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class SIGNETGAME_API UCharacterAudioComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	
	UCharacterAudioComponent();

	UPROPERTY(EditDefaultsOnly, Category = "Audio")
	TMap<EAudioType, FAudioChannelConfig> AudioSettingsMap;

	UFUNCTION(BlueprintCallable, Category = "Audio")
	void PlayUISound(EUISound SoundType);

};
