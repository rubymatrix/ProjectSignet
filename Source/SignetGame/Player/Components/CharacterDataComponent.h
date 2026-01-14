// Copyright Red Lotus Games, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SignetGame/Abilities/ActionTypes.h"
#include "SignetGame/Data/AudioTypes.h"
#include "SignetGame/Combat/CombatTypes.h"
#include "SignetGame/Data/CharacterParts.h"
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

	UAnimMontage* GetMovingAttackMontage(const EActionDirection Direction);


// Begin Race Specific Sounds

	UFUNCTION(BlueprintCallable, BlueprintPure)
	USoundBase* GetVocalization(const EVocalizationType VocalType);
	
// Begin Equipment-specific Sounds

	UFUNCTION(BlueprintCallable, BlueprintPure)
	USoundBase* GetFootstepSound();
	
	UFUNCTION(BlueprintCallable, BlueprintPure)
	USoundBase* GetWeaponWhooshSound(EPhysicalAttackDirection Direction);

	UFUNCTION(BlueprintCallable, BlueprintPure)
	USoundBase* GetWeaponImpactSound(const FAttackResult& Result);
	
	
private:

	UPROPERTY(Transient)
	TWeakObjectPtr<ASignetPlayerCharacter> Character;

	UGameDataSubsystem* GetGameData();
	const FCharacterPartsRow* GetParts();
	int GetFaceVoiceIndex();
	FVoiceBank GetVoiceBank();

	bool IsWeaponAction(const EMontageType ActionType);

};
