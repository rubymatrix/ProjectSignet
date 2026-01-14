// Copyright Red Lotus Games, All Rights Reserved.

#include "GA_Attack.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "SignetGame/Abilities/SignetAbilitySystemComponent.h"
#include "SignetGame/Combat/CombatInterface.h"
#include "SignetGame/Combat/CombatTypes.h"
#include "SignetGame/Combat/Components/EffectComponent.h"
#include "SignetGame/Player/Components/CharacterDataComponent.h"
#include "SignetGame/Player/SignetPlayerCharacter.h"
#include "SignetGame/Inventory/SignetInventoryComponent.h"
#include "TimerManager.h"
#include "Algo/RandomShuffle.h"
#include "SignetGame/Abilities/TagCache.h"

UGA_Attack::UGA_Attack()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::LocalPredicted;

	SetAssetTags(FGameplayTagContainer(FTagCache::Get().Ability.Attack));

	ActivationRequiredTags.AddTag(FTagCache::Get().State.Engaged);
	ActivationBlockedTags.AddTag(FTagCache::Get().State.Dead);
	ActivationBlockedTags.AddTag(FTagCache::Get().State.Stunned);

	ActionType = EMontageType::Attack;
}

void UGA_Attack::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	USignetAbilitySystemComponent* SignetASC = Cast<USignetAbilitySystemComponent>(ActorInfo->AbilitySystemComponent.Get());
	if (!SignetASC)
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	AActor* Target = SignetASC->GetAutoAttackTarget();
	// We allow activation even if target is null (shadow boxing) but usually auto-attack loop checks this.

	// 1. Calculate Results
	TArray<FAttackResult> Results = CalculateAttackResults(Target);

	// 2. Push Results to Character
	if (ICombatInterface* Combatant = Cast<ICombatInterface>(ActorInfo->AvatarActor.Get()))
	{
		Combatant->PushAttackResults(Results);
	}

	// 3. Get Montages
	if (UCharacterDataComponent* CDC = ActorInfo->AvatarActor->FindComponentByClass<UCharacterDataComponent>())
	{
		AttackMontages.Empty();

		bool bHasOffhandOrKicks = false;
		for (const auto& Result : Results)
		{
			if (bHasOffhandOrKicks) continue;

			if (Result.AttackDirection == EPhysicalAttackDirection::Left)
				bHasOffhandOrKicks = true;
			if (Result.AttackType == EPhysicalAttackType::Kick)
				bHasOffhandOrKicks = true;
		}

		if (bHasOffhandOrKicks)
		{
			for (const auto& Result : Results)
			{
				bool bIsKick = Result.AttackType == EPhysicalAttackType::Kick;
				bool bIsSub = Result.AttackDirection == EPhysicalAttackDirection::Left;
				// Determine montage type
				EMontageType TargetMontageType = EMontageType::Attack;
				if (bIsKick) TargetMontageType = EMontageType::KickAttack;
				if (bIsSub) TargetMontageType = EMontageType::SubAttack;
			
				const auto MontageResults = CDC->GetAttackRoundMontages(TargetMontageType, 1);

				if (MontageResults.Num() > 0)
				{
					AttackMontages.Add(MontageResults[0]);	
				}
			}
		}
		else
		{
			AttackMontages = CDC->GetAttackRoundMontages(EMontageType::Attack, Results.Num());
			Algo::RandomShuffle(AttackMontages);
		}
	}

	if (AttackMontages.Num() == 0)
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	// 4. Start playing montages
	CurrentMontageIndex = 0;
	PlayNextMontage();
}

void UGA_Attack::EndAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateEndAbility, bool bWasCancelled)
{
	if (UWorld* World = GetWorld())
	{
		World->GetTimerManager().ClearTimer(ImpactTimerHandle);
	}

	if (ICombatInterface* Combatant = Cast<ICombatInterface>(ActorInfo->AvatarActor.Get()))
	{
		Combatant->ClearAttackResults();
	}

	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UGA_Attack::PlayNextMontage()
{
	if (!AttackMontages.IsValidIndex(CurrentMontageIndex))
	{
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
		return;
	}

	UAnimMontage* Montage = AttackMontages[CurrentMontageIndex];
	const float PlayRate = 1.15f;
	UAbilityTask_PlayMontageAndWait* CurrentMontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, NAME_None, Montage, PlayRate, NAME_None, true, 1.0f);

	if (!CurrentMontageTask)
	{
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
		return;
	}

	CurrentMontageTask->OnBlendOut.AddDynamic(this, &UGA_Attack::OnCurrentMontageCompleted);
	CurrentMontageTask->OnInterrupted.AddDynamic(this, &UGA_Attack::OnCurrentMontageInterruptedOrCancelled);
	CurrentMontageTask->OnCancelled.AddDynamic(this, &UGA_Attack::OnCurrentMontageInterruptedOrCancelled);
	CurrentMontageTask->ReadyForActivation();
	
	if (UWorld* World = GetWorld())
	{
		const float WhooshDelay = Montage->GetPlayLength() / (2.75f * PlayRate);
		const float ImpactDelay = Montage->GetPlayLength() / (2.25f * PlayRate);

		World->GetTimerManager().SetTimer(VocalsTimerHandle, this, &UGA_Attack::TriggerVocals, WhooshDelay, false);
		// World->GetTimerManager().SetTimer(WhooshTimerHandle, this, &UGA_Attack::TriggerWhooshEffects, WhooshDelay, false);
		World->GetTimerManager().SetTimer(ImpactTimerHandle, this, &UGA_Attack::TriggerImpactEffects, ImpactDelay, false);
	}
}

void UGA_Attack::TriggerImpactEffects()
{
	AActor* Attacker = GetAvatarActorFromActorInfo();
	ICombatInterface* AttackerCombatant = Cast<ICombatInterface>(Attacker);
	
	if (!AttackerCombatant) return;

	// 1. Advance to the result corresponding to this montage
	AttackerCombatant->AdvanceAttackResult();

	FAttackResult Result;
	if (AttackerCombatant->GetCurrentAttackResult(Result))
	{
		// 2. Play Sound (and any internal logic in PlayWeaponImpact)
		if (Result.bIsHit)  AttackerCombatant->PlayWeaponImpact();
		else TriggerWhooshEffects();

		// 3. Trigger Hit Flash and VFX if it's a hit
		
		USignetAbilitySystemComponent* SignetASC = Cast<USignetAbilitySystemComponent>(GetAbilitySystemComponentFromActorInfo());
		AActor* Target = SignetASC ? SignetASC->GetAutoAttackTarget() : nullptr;

		if (Target)
		{
			if (Result.bIsHit)
			{
				// Hit Flash on Target
				if (ICombatInterface* TargetCombatant = Cast<ICombatInterface>(Target))
				{
					TargetCombatant->HitFlash();
					TargetCombatant->PlayVocalization(EVocalizationType::DamageReact);
					TargetCombatant->DamageCombatText(FMath::RandRange(20.f, 80.f), Result.bIsCritical);
					if (Result.bIsCritical)
					{
						AttackerCombatant->CameraShake(ECameraShakeType::CriticalImpact, 1.f);
					}
				}

				// Impact VFX via EffectComponent
				if (UEffectComponent* EffectComp = Target->FindComponentByClass<UEffectComponent>())
				{
					const EVFXType VFXType = Result.bIsCritical ? EVFXType::Impact_Critical : EVFXType::Impact;
					EffectComp->SpawnEffectFromGlobalPoolWithParams(VFXType, Attacker, Target);
				}
			}
			else
			{
				if (ICombatInterface* TargetCombatant = Cast<ICombatInterface>(Target))
				{
					TargetCombatant->PlayVocalization(EVocalizationType::EvadeReact);
					TargetCombatant->StatusEffectText(TEXT("Miss"));
				}
			}
		}
	}
}

void UGA_Attack::TriggerVocals()
{
	AActor* Attacker = GetAvatarActorFromActorInfo();
	ICombatInterface* AttackerCombatant = Cast<ICombatInterface>(Attacker);
	
	if (!AttackerCombatant) return;
	
	AttackerCombatant->PlayVocalization(EVocalizationType::Attack);
}

void UGA_Attack::TriggerWhooshEffects()
{
	AActor* Attacker = GetAvatarActorFromActorInfo();
	ICombatInterface* AttackerCombatant = Cast<ICombatInterface>(Attacker);
	
	if (!AttackerCombatant) return;
	
	AttackerCombatant->PlayWeaponWhoosh();
}

void UGA_Attack::OnCurrentMontageCompleted()
{
	// Prevent double trigger from OnBlendOut and OnCompleted if one task fires both
	if (AttackMontages.IsValidIndex(CurrentMontageIndex))
	{
		CurrentMontageIndex++;
		PlayNextMontage();
	}
}

void UGA_Attack::OnCurrentMontageInterruptedOrCancelled()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
}

TArray<FAttackResult> UGA_Attack::CalculateAttackResults(AActor* Target)
{
	TArray<FAttackResult> Results;

	// Simplified logic for now: 1-3 hits based on some randomness
	// In a real implementation, this would query stats, traits (Double Attack, etc.)
	
	int32 NumHits = 1;
	const float MultiHitChance = 0.50f; // 20% chance for extra hit
	
	if (FMath::FRand() < MultiHitChance) NumHits++;
	if (FMath::FRand() < MultiHitChance * 0.5f) NumHits++;

	// Determine if we should alternate hands
	bool bAlternateHands = true;
	if (const ASignetPlayerCharacter* Character = Cast<ASignetPlayerCharacter>(GetAvatarActorFromActorInfo()))
	{
		if (const USignetInventoryComponent* Inventory = Character->GetInventoryComponent())
		{
			const FGameplayTag WeaponSkill = Inventory->GetEquippedWeaponSkill();
			const FSkillTags& SkillTags = FTagCache::Get().Skill;
			
			if (WeaponSkill != SkillTags.None && WeaponSkill != SkillTags.H2H)
			{
				bAlternateHands = false;
			}

			if (NumHits < 2 && (WeaponSkill == SkillTags.H2H || WeaponSkill == SkillTags.None))
			{
				NumHits = 2;
			}
		}
	}

	for (int32 i = 0; i < NumHits; ++i)
	{
		FAttackResult Result;
		Result.bIsHit = (FMath::FRand() < 0.80f); // 80% base hit rate
		Result.bIsCritical = (FMath::FRand() < 0.4f); // 40% crit rate
		Result.AttackType = (NumHits > 1) ? EPhysicalAttackType::Double : EPhysicalAttackType::Normal;
		if (NumHits > 2) Result.AttackType = EPhysicalAttackType::Triple;
		
		// Alternate hands if multi-hit and weapon allows it
		if (bAlternateHands)
		{
			Result.AttackDirection = (i % 2 == 0) ? EPhysicalAttackDirection::Right : EPhysicalAttackDirection::Left;
		}
		else
		{
			Result.AttackDirection = EPhysicalAttackDirection::Right;
		}
		
		Results.Add(Result);
	}

	return Results;
}
