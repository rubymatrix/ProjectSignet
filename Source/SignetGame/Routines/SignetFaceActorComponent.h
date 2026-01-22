// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SignetFaceActorComponent.generated.h"

class AActor;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class SIGNETGAME_API USignetFaceActorComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	USignetFaceActorComponent();

	UFUNCTION(BlueprintCallable, Category="Facing")
	void SetTargetActor(AActor* NewTargetActor);

	UFUNCTION(BlueprintCallable, Category="Facing")
	void ClearTargetActor();

	UFUNCTION(BlueprintPure, Category="Facing")
	AActor* GetTargetActor() const;

	UFUNCTION(BlueprintPure, Category="Facing")
	bool HasTargetActor() const;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Facing", meta=(ClampMin="0.0"))
	float RotationInterpSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Facing")
	bool bYawOnly;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Facing", meta=(ClampMin="0.0"))
	float MinDistanceToUpdate;

protected:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	UPROPERTY(Transient)
	TObjectPtr<AActor> TargetActor;
};
