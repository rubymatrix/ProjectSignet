// Copyright Red Lotus Games, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "ModularGameplayActors/GSCModularPawn.h"
#include "SignetGame/Combat/CombatInterface.h"
#include "SignetGame/Combat/Components/EffectComponent.h"
#include "SignetGame/Player/Components/TargetableInterface.h"
#include "SignetMobPawn.generated.h"

class UCombatTextComponent;
class UHitFlashComponent;
class USkeletalMeshComponent;
class UCapsuleComponent;

/**
 * SignetMobPawn - A modular pawn for mobs in Signet, implements targeting and combat interfaces.
 */
UCLASS()
class SIGNETGAME_API ASignetMobPawn : public AGSCModularPawn, public ITargetableInterface, public ICombatInterface
{
	GENERATED_BODY()

public:
	ASignetMobPawn(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	/** The capsule collider for this pawn. */
	UPROPERTY(Category=Pawn, VisibleAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess = "true"))
	TObjectPtr<UCapsuleComponent> CapsuleComponent;

	/** The main skeletal mesh associated with this pawn. */
	UPROPERTY(Category=Pawn, VisibleAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess = "true"))
	TObjectPtr<USkeletalMeshComponent> Mesh;

	UPROPERTY(Category=Pawn, VisibleAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess = "true"))
	TObjectPtr<UHitFlashComponent> HitFlashComp;

	UPROPERTY(Category=Pawn, VisibleAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess = "true"))
	TObjectPtr<UEffectComponent> EffectComp;

	UPROPERTY(Category=Pawn, VisibleAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess = "true"))
	TObjectPtr<UCombatTextComponent> CombatTextComp;

	//~ Begin ITargetableInterface
	virtual bool IsTargetable_Implementation() const override;
	virtual bool IsLockable_Implementation() const override;
	virtual FVector GetAimPoint_Implementation() const override;
	//~ End ITargetableInterface

	//~ Begin ICombatInterface
	virtual float GetHealth() override;
	virtual float GetMaxHealth() override;
	virtual float GetPower() override;
	virtual float GetMaxPower() override;
	virtual float GetTP() override;
	virtual void PlayHitReaction() override;
	virtual void HitFlash() override;
	virtual void PlayVocalization(const EVocalizationType VoiceType) override;
	virtual void PlayWeaponSwingAtLocation(const FVector& Location) override;
	virtual void PlayImpactSoundAtLocation(const FVector& Location) override;
	virtual void DoAttackSequence() override;
	virtual void DamageCombatText(float Amount, bool bCritical = false) override;
	virtual void HealingCombatText(float Amount, bool bCritical = false) override;
	virtual void StatusEffectText(const FString& Text) override;
	//~ End ICombatInterface

protected:
	virtual void BeginPlay() override;

};
