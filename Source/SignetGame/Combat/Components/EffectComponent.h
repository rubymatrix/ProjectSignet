// Copyright Red Lotus Games, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SignetGame/Data/VFXTypes.h"
#include "EffectComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class SIGNETGAME_API UEffectComponent : public UActorComponent
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable)
	void SpawnEffectFromGlobalPool(const EVFXType VFXType);

	UFUNCTION(BlueprintCallable)
	void SpawnEffectFromGlobalPoolWithParams(const EVFXType VFXType, const AActor* SourceActor, const AActor* TargetActor);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect")
	float DirectionOffset = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect")
	float HeightOffset = 0.f;

private:

	void Internal_SpawnEffectFromGlobalPool(const EVFXType VFXType, const AActor* SourceActor, const AActor* TargetActor);

	AActor* GetTargetActor() const;

	FTransform GetActorOriginTransform(const AActor* Actor) const;
	FTransform GetActorSocketTransform(const AActor* Actor, const FName SocketName) const;
	FTransform GetTargetVectorTransform(const FTransform& Origin, const FTransform& Destination, const float Offset, const float TargetHeight) const;
	
	void SpawnNiagaraSystemAtLocation(UNiagaraSystem* System, const FTransform& Transform);
	
	
};
