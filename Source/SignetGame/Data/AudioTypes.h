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