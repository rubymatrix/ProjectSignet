// Copyright Red Lotus Games, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HitFlashComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class SIGNETGAME_API UHitFlashComponent : public UActorComponent
{
	GENERATED_BODY()
	
public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float FlashDuration = 0.025f;
	
	UHitFlashComponent();

protected:

	bool bMaterialsCreated = false;
	FTimerHandle HitFlashTimer;

	UPROPERTY(Transient)
	TArray<UMaterialInstanceDynamic*> HitFlashMaterials;
	
	virtual void BeginPlay() override;

public:
	
	UFUNCTION(BlueprintCallable)
	void TriggerHitFlash(float Duration = 0.f);

private:

	void StartFlashing(float Duration = 0.f);
	void StopFlashing();
	
};
