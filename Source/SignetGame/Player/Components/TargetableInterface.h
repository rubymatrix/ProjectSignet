// Copyright Red Lotus Games, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "TargetableInterface.generated.h"

UINTERFACE(MinimalAPI, BlueprintType)
class UTargetableInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * Interface for actors that can be targeted by the targeting system.
 */
class SIGNETGAME_API ITargetableInterface
{
	GENERATED_BODY()

public:
	/** Returns true if this actor is currently targetable. */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Targeting")
	bool IsTargetable() const;

	/** Returns true if this actor can be locked onto. */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Targeting")
	bool IsLockable() const;

	/** Returns the point the camera and player should look at. */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Targeting")
	FVector GetAimPoint() const;

    /** Called when this actor is targeted. */
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Targeting")
    void OnTargeted(AActor* Targeter);

    /** Called when this actor is untargeted. */
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Targeting")
    void OnUntargeted(AActor* Targeter);
};
