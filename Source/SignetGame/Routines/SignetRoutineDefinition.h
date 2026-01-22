// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GameplayTagContainer.h"
#include "SignetRoutineDefinition.generated.h"

class APawn;
class USplineComponent;

UENUM(BlueprintType)
enum class ESignetRoutineStepType : uint8
{
	FollowPath,
	PerformAction,
	Wait,
	Despawn
};

USTRUCT(BlueprintType)
struct SIGNETGAME_API FSignetRoutinePath
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Routine")
	TArray<FName> SplineActorTags;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Routine")
	bool bReversePath = false;
};

USTRUCT(BlueprintType)
struct SIGNETGAME_API FSignetRoutineAction
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Routine")
	FGameplayTag ActionTag;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Routine")
	float Duration = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Routine")
	bool bWaitForExternalCompletion = false;
};


UENUM(BlueprintType)
enum class ERoutineActionType : uint8 
{
	FaceTarget,
	PlayAnimation,
	Talk
};

USTRUCT(BlueprintType)
struct SIGNETGAME_API FSignetRoutineActionData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Routine")
	ERoutineActionType ActionType = ERoutineActionType::Talk;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Target")
	FName TargetID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Talk")
	FString Message;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="PlayAnimation")
	TSoftObjectPtr<UAnimMontage> Animation;

};

USTRUCT(BlueprintType)
struct SIGNETGAME_API FSignetRoutineStep
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Routine")
	ESignetRoutineStepType StepType = ESignetRoutineStepType::FollowPath;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Routine", meta=(EditCondition="StepType == ESignetRoutineStepType::FollowPath", EditConditionHides))
	FSignetRoutinePath Path;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Routine", meta=(EditCondition="StepType == ESignetRoutineStepType::PerformAction", EditConditionHides))
	FSignetRoutineAction Action;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Routine", meta=(EditCondition="StepType == ESignetRoutineStepType::Wait", EditConditionHides, ClampMin="0.0"))
	float WaitDuration = 0.0f;
};

UCLASS(BlueprintType)
class SIGNETGAME_API USignetRoutineDefinition : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Routine")
	TSubclassOf<APawn> PawnClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Routine")
	bool bDestroyPawnOnFinish = true;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Routine")
	bool bTeleportToStart = true;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Routine")
	TArray<FSignetRoutineStep> Steps;
};
