// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SignetSplineFollowingComponent.generated.h"

class AAIController;
class APawn;
class USplineComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSignetSplinePathCompleted, USplineComponent*, SplineComponent);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class SIGNETGAME_API USignetSplineFollowingComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	USignetSplineFollowingComponent();

	UFUNCTION(BlueprintCallable, Category="Spline Following")
	void FollowPath(USplineComponent* SplineComp, bool bReversePath);

	UFUNCTION(BlueprintCallable, Category="Spline Following")
	void StopFollowing();

	UFUNCTION(BlueprintCallable, Category="Spline Following")
	bool IsFollowing() const;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Spline Following")
	float LookAheadDistance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Spline Following")
	float AcceptanceRadius;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Spline Following")
	float StuckTimeout;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Spline Following")
	float ProgressEpsilon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Spline Following")
	bool bShowDebug;

	UPROPERTY(BlueprintAssignable, Category="Spline Following")
	FSignetSplinePathCompleted OnPathCompleted;

protected:
	virtual void BeginPlay() override;

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	AAIController* ResolveController() const;

	APawn* ResolvePawn() const;

	void UpdateMoveTarget();

	bool HasReachedEnd(const FVector& PawnLocation) const;

	TWeakObjectPtr<AAIController> CachedController;

	TWeakObjectPtr<USplineComponent> ActiveSpline;

	float SplineLength;

	float CurrentDistance;

	float LastProgressDistance;

	float TimeSinceProgress;

	bool bReverse;

	bool bIsFollowing;
};
