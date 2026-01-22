// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SignetRoutineSpawnerActor.generated.h"

class AAIController;
class USignetRoutineComponent;
class USignetRoutineDefinition;
class USignetSplineFollowingComponent;
class USplineComponent;

UCLASS(BlueprintType)
class SIGNETGAME_API ASignetRoutineSpawnerActor : public AActor
{
	GENERATED_BODY()

public:
	ASignetRoutineSpawnerActor();

	UFUNCTION(BlueprintCallable, Category="Routine Spawner")
	void StartSpawner();

	UFUNCTION(BlueprintCallable, Category="Routine Spawner")
	void StopSpawner(bool bDestroySpawnedControllers);

	UFUNCTION(BlueprintCallable, Category="Routine Spawner")
	int32 GetActivePawnCount() const;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Routine Spawner")
	TObjectPtr<USignetRoutineDefinition> RoutineDefinition;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Routine Spawner")
	TSubclassOf<AAIController> ControllerClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Routine Spawner")
	int32 MinPoolSize;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Routine Spawner", meta=(ClampMin="0"))
	int32 MaxPoolSize;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Routine Spawner", meta=(ClampMin="0.05"))
	float MaintainIntervalSeconds;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Routine Spawner", meta=(ClampMin="0.0"))
	float RespawnDelaySeconds;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Routine Spawner", meta=(ClampMin="0.0"))
	float SpawnCollisionRadius;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Routine Spawner")
	bool bAutoStart;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Routine Spawner")
	bool bVerbose;

protected:
	virtual void BeginPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

private:
	void VerboseScreenMessage(const FString& Message) const;

	void MaintainPool();

	bool EnsureControllerSetup(AAIController* Controller, USignetRoutineComponent*& OutRoutineComponent) const;

	bool ResolveInitialPathSpline(USplineComponent*& OutSpline, bool& bOutReverse) const;

	bool ChooseFreeSpawnTransform(FTransform& OutSpawnTransform, USplineComponent*& OutSpline, bool& bOutReverse) const;

	bool TryStartRoutineOnController(AAIController* Controller);

	void CleanupControllerList();

	int32 CountPawnsInternal() const;

	UPROPERTY()
	TArray<TObjectPtr<AAIController>> SpawnedControllers;

	FTimerHandle MaintainTimerHandle;

	double LastSpawnTimeSeconds;

	FRandomStream RandomStream;
};
