#pragma once

#include "CoreMinimal.h"
#include "MobDataAsset.generated.h"


UCLASS()
class SIGNETGAME_API UMobDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere)
	FString MobName;

	UPROPERTY(EditAnywhere)
	TObjectPtr<USkeletalMesh> SkeletalMesh;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UAnimInstance> AnimInstanceClass;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UDataTable> StatsTable;
};

