#pragma once
#include "CoreMinimal.h"
#include "NiagaraSystem.h"
#include "VFXTypes.generated.h"


UENUM(BlueprintType)
enum class EVFXType : uint8
{
	None,
	LevelUp,
	LevelDown,
	Impact,
	Impact_Critical
};

UENUM(BlueprintType)
enum class EVFXAttachmentType : uint8
{
	SelfOrigin,
	TargetOrigin,
	SelfSocket,
	TargetSocket,
	TargetVector
};

USTRUCT(BlueprintType)
struct SIGNETGAME_API FVFXSystem
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	EVFXAttachmentType AttachmentType = EVFXAttachmentType::SelfOrigin;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FName SocketName = NAME_None;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float VectorOffset = 75.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FVector Scale = FVector(1.f);;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UNiagaraSystem* System = nullptr;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UParticleSystem* LegacySystem = nullptr;
};

UCLASS()
class SIGNETGAME_API UVFXDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TMap<EVFXType, FVFXSystem> VFXSystems;
	
};