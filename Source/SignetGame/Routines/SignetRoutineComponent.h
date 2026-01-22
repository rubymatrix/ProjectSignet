// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameplayTagContainer.h"
#include "SignetRoutineDefinition.h"
#include "SignetRoutineComponent.generated.h"

class AAIController;
class APawn;
class USignetSplineFollowingComponent;
class USplineComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FSignetRoutineActionStarted, FGameplayTag, ActionTag, APawn*, Pawn);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FSignetRoutineCompleted);

UCLASS(ClassGroup=(Signet), meta=(BlueprintSpawnableComponent))
class SIGNETGAME_API USignetRoutineComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	USignetRoutineComponent();

	UFUNCTION(BlueprintCallable, Category="Routine")
	void StartRoutine(USignetRoutineDefinition* RoutineToRun);

	UFUNCTION(BlueprintCallable, Category="Routine")
	void StopRoutine();

	UFUNCTION(BlueprintCallable, Category="Routine")
	void CompleteActiveAction();

	UFUNCTION(BlueprintCallable, Category="Routine")
	bool IsRoutineActive() const;

	UFUNCTION(BlueprintCallable, Category="Routine")
	void SetReversePathOverride(bool bEnabled, bool bReversePath);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Routine")
	TObjectPtr<USignetRoutineDefinition> RoutineDefinition;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Routine")
	TArray<TObjectPtr<AActor>> StartLocations;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Routine")
	bool bAutoStart = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Routine")
	bool bOverrideReversePath;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Routine")
	bool bOverrideReversePathValue;

	UPROPERTY(BlueprintAssignable, Category="Routine")
	FSignetRoutineActionStarted OnRoutineActionStarted;

	UPROPERTY(BlueprintAssignable, Category="Routine")
	FSignetRoutineCompleted OnRoutineCompleted;

protected:
	virtual void BeginPlay() override;

private:
	UFUNCTION(NetMulticast, Reliable)
	void MulticastActionStarted(FGameplayTag ActionTag, APawn* Pawn);

	void ExecuteCurrentStep();
	void AdvanceStep();

	UFUNCTION()
	void HandlePathCompleted(USplineComponent* SplineComponent);

	bool EnsureControlledPawn();
	bool ResolveStartTransform(FTransform& OutTransform) const;
	bool ApplyStartTransform(APawn* Pawn) const;
	APawn* GetControlledPawn() const;
	AAIController* GetOwningController() const;
	USplineComponent* ChooseSplineFromTags(const TArray<FName>& Tags) const;
	void FinishRoutine();
	void ClearStepTimer();

	UPROPERTY()
	TObjectPtr<USignetSplineFollowingComponent> SplineFollower;

	UPROPERTY()
	TObjectPtr<USignetRoutineDefinition> ActiveRoutine;

	TWeakObjectPtr<APawn> SpawnedPawn;

	FTimerHandle StepTimerHandle;

	int32 ActiveStepIndex;

	bool bWaitingForAction;
	bool bIsRunning;
};
