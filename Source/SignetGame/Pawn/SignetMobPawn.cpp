// Copyright Red Lotus Games, All Rights Reserved.


#include "SignetMobPawn.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "SignetGame/Abilities/SignetAbilitySystemComponent.h"
#include "SignetGame/Player/Components/TargetableInterface.h"
#include "SignetGame/Combat/CombatInterface.h"
#include "SignetGame/Combat/Components/CombatTextComponent.h"
#include "SignetGame/Combat/Components/HitFlashComponent.h"
#include "SignetGame/Data/AudioTypes.h"

ASignetMobPawn::ASignetMobPawn(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer.SetDefaultSubobjectClass<USignetAbilitySystemComponent>(TEXT("AbilitySystemComponent")))
{
	CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComponent"));
	if (CapsuleComponent)
	{
		CapsuleComponent->InitCapsuleSize(34.0f, 88.0f);
		CapsuleComponent->SetCollisionProfileName(TEXT("Pawn"));
		CapsuleComponent->SetCanEverAffectNavigation(true);

		RootComponent = CapsuleComponent;
	}

	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
	if (Mesh)
	{
		Mesh->SetupAttachment(CapsuleComponent);
		Mesh->SetCollisionProfileName(TEXT("CharacterMesh"));
		Mesh->SetGenerateOverlapEvents(true);
		Mesh->SetCanEverAffectNavigation(false);
	}

	HitFlashComp = CreateDefaultSubobject<UHitFlashComponent>(TEXT("HitFlashComp"));
	EffectComp = CreateDefaultSubobject<UEffectComponent>(TEXT("EffectComp"));
	CombatTextComp = CreateDefaultSubobject<UCombatTextComponent>(TEXT("CombatTextComp"));
}

void ASignetMobPawn::BeginPlay()
{
	Super::BeginPlay();
}

bool ASignetMobPawn::IsTargetable_Implementation() const
{
	return true;
}

bool ASignetMobPawn::IsLockable_Implementation() const
{
	return true;
}

FVector ASignetMobPawn::GetAimPoint_Implementation() const
{
	if (Mesh)
	{
		return Mesh->Bounds.Origin;
	}
	return GetActorLocation();
}

float ASignetMobPawn::GetHealth()
{
	// Implementation should probably use AbilitySystemComponent to get health attribute
	return 0.0f;
}

float ASignetMobPawn::GetMaxHealth()
{
	// Implementation should probably use AbilitySystemComponent to get max health attribute
	return 0.0f;
}

float ASignetMobPawn::GetPower()
{
	return 0.0f;
}

float ASignetMobPawn::GetMaxPower()
{
	return 0.0f;
}

float ASignetMobPawn::GetTP()
{
	return 0.0f;
}

void ASignetMobPawn::PlayHitReaction()
{
	if (EffectComp) EffectComp->SpawnEffectFromGlobalPool(EVFXType::Impact);
	
	FTimerHandle FlashTimer;
	GetWorld()->GetTimerManager().SetTimer(FlashTimer, this, &ASignetMobPawn::HitFlash, 0.1f, false);


	PlayImpactSoundAtLocation(GetActorLocation());
	PlayVocalization(EVocalizationType::DamageReact);

	
}

void ASignetMobPawn::HitFlash()
{
	if (HitFlashComp) HitFlashComp->TriggerHitFlash();
}

void ASignetMobPawn::PlayVocalization(const EVocalizationType VoiceType)
{
}

void ASignetMobPawn::PlayWeaponSwingAtLocation(const FVector& Location)
{
}

void ASignetMobPawn::PlayImpactSoundAtLocation(const FVector& Location)
{
}

void ASignetMobPawn::DoAttackSequence()
{
}

void ASignetMobPawn::DamageCombatText(float Amount, bool bCritical)
{
	if (CombatTextComp) CombatTextComp->AddDamageText(Amount, bCritical);
}

void ASignetMobPawn::HealingCombatText(float Amount, bool bCritical)
{
	if (CombatTextComp) CombatTextComp->AddHealingText(Amount, bCritical);
}

void ASignetMobPawn::StatusEffectText(const FString& Text)
{
	if (CombatTextComp) CombatTextComp->AddStatusEffectText(Text);
}
