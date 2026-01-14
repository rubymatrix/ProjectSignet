#pragma once
#include "CoreMinimal.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "AudioTypes.generated.h"


UENUM(BlueprintType)
enum class EVocalizationType : uint8
{
	Attack,
	DamageReact,
	EvadeReact,
	Death
};

UENUM(BlueprintType)
enum class EGroundMaterialType : uint8
{
	Dirt,
	Sand,
	Grass,
	Gravel,
	Stone,
	Carpet,
	Water
};

UENUM(BlueprintType)
enum class EWeaponSoundSlot : uint8
{
	Slot1,
	Slot2,
	Slot3,
	Slot4
};

UENUM(BlueprintType)
enum class EArmorSoundClass : uint8
{
	Cloth,
	Leather,
	Mail,
	Plate
};

USTRUCT(BlueprintType)
struct SIGNETGAME_API FVoiceSet
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<USoundBase*> Sounds;
};

USTRUCT(BlueprintType)
struct SIGNETGAME_API FVoiceBank
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TMap<EVocalizationType, FVoiceSet> Vocals;

	bool HasVocal(const EVocalizationType VocalType) const
	{
		if (const auto VoiceSet = Vocals.Find(VocalType))
		{
			return VoiceSet->Sounds.Num() > 0;
		}
		return false;
	}
};

USTRUCT(Blueprintable)
struct SIGNETGAME_API FFootstepBank
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TMap<TEnumAsByte<EPhysicalSurface>, USoundBase*> FootstepMaterialMap;
};

USTRUCT(BlueprintType)
struct SIGNETGAME_API FImpactSoundBank
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TMap<EWeaponSoundSlot, USoundBase*> ImpactSoundMap;
};

USTRUCT(BlueprintType)
struct SIGNETGAME_API FWhooshBank
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TMap<EWeaponSoundSlot, USoundBase*> WhooshSoundMap;
};


USTRUCT(BlueprintType)
struct SIGNETGAME_API FWeaponSoundBankTableRow : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<USoundBase> EngageSound;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<USoundBase> DisengageSound;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FImpactSoundBank ImpactSounds;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FWhooshBank WhooshSounds;
};

UCLASS()
class SIGNETGAME_API UFootstepBankDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TMap<EArmorSoundClass, FFootstepBank> FootstepSounds;
	
};



UENUM(BlueprintType)
enum class EUISound : uint8
{
	None,
	OpenMenu,
	CloseMenu,
	MenuMovement,
	OpenActionMenu,
	CloseActionMenu,
	Confirm,
	Target,
	Cancel,
	LevelUp,
	LevelDown,
	QuestComplete,
	MissionComplete,
	RankUp
};

UCLASS()
class SIGNETGAME_API UUISoundAsset : public UDataAsset
{
	GENERATED_BODY()

public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TMap<EUISound, USoundBase*> UISounds;
	
};


UENUM(BlueprintType)
enum class EAudioType : uint8
{
	BGM,
	Ambience,
	SoundEffects,
	UI
};

USTRUCT(BlueprintType)
struct SIGNETGAME_API FAudioChannelConfig
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Audio")
	float VolumeMultiplier = 1.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Audio")
	float PitchMultiplier = 1.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Audio")
	float StartTime = 0.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Audio")
	class USoundConcurrency* ConcurrencySettings = nullptr;
};




