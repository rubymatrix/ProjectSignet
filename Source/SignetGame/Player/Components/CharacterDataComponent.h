// Copyright Red Lotus Games, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SignetGame/Abilities/ActionTypes.h"
#include "SignetGame/Data/AudioTypes.h"
#include "CharacterDataComponent.generated.h"

class UGameDataSubsystem;
struct FCharacterPartsRow;
class ASignetPlayerCharacter;




UCLASS(ClassGroup=(Signet), meta=(BlueprintSpawnableComponent))
class SIGNETGAME_API UCharacterDataComponent : public UActorComponent
{
	GENERATED_BODY()
	
public:

	UCharacterDataComponent();

	virtual void BeginPlay() override;
	
// Begin Actions

	UFUNCTION(BlueprintCallable, BlueprintPure)
	UAnimMontage* GetActionMontage(const EMontageType MontageType);

	UFUNCTION(BlueprintCallable, BlueprintPure)
	TArray<UAnimMontage*> GetAttackRoundMontages(const EMontageType AttackType, const int AttackCount);


// Begin Race Specific Sounds

	UFUNCTION(BlueprintCallable, BlueprintPure)
	USoundBase* GetVocalization(const EVocalizationType VocalType);
	
// Begin Equipment-specific Sounds

	UFUNCTION(BlueprintCallable, BlueprintPure)
	USoundBase* GetFootstepSound();
	
	
private:

	UPROPERTY(Transient)
	TWeakObjectPtr<ASignetPlayerCharacter> Character;

	UGameDataSubsystem* GetGameData();
	const FCharacterPartsRow* GetParts();
	

};
