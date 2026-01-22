// Copyright Red Lotus Games, All Rights Reserved.

#include "GA_Attack.h"
#include "GameplayEffect.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "SignetGame/Abilities/Effects/GE_PhysicalDamage.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "SignetGame/Abilities/SignetAbilitySystemComponent.h"
#include "SignetGame/Combat/CombatInterface.h"
#include "SignetGame/Combat/CombatTypes.h"
#include "SignetGame/Combat/Components/EffectComponent.h"
#include "SignetGame/Combat/SignetCombatStatics.h"
#include "SignetGame/Player/Components/CharacterDataComponent.h"
#include "SignetGame/Player/SignetPlayerCharacter.h"
#include "SignetGame/Inventory/SignetInventoryComponent.h"
#include "TimerManager.h"
#include "Algo/RandomShuffle.h"
#include "SignetGame/Abilities/TagCache.h"
#include "SignetGame/Abilities/Attributes/SignetCombatAttributeSet.h"
#include "SignetGame/Abilities/Attributes/SignetPrimaryAttributeSet.h"
#include "SignetGame/Combat/SkillRankStatics.h"
#include "SignetGame/Util/Logging.h"

UGA_Attack::UGA_Attack()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::LocalPredicted;

	SetAssetTags(FGameplayTagContainer(FTagCache::Get().Ability.Attack));

	ActivationRequiredTags.AddTag(FTagCache::Get().State.Engaged);
	ActivationBlockedTags.AddTag(FTagCache::Get().State.Dead);
	ActivationBlockedTags.AddTag(FTagCache::Get().State.Stunned);

	ActionType = EMontageType::Attack;
	DamageEffectClass = UGE_PhysicalDamage::StaticClass();
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
	if (Target && ActorInfo->AvatarActor.IsValid())
	{
		const FVector AttackerLocation = ActorInfo->AvatarActor->GetActorLocation();
		const FVector TargetLocation = Target->GetActorLocation();
		const FVector ToTarget = FVector(TargetLocation.X - AttackerLocation.X, TargetLocation.Y - AttackerLocation.Y, 0.0f);
		const FVector Forward = FVector(ActorInfo->AvatarActor->GetActorForwardVector().X, ActorInfo->AvatarActor->GetActorForwardVector().Y, 0.0f);
		const float DistanceSq2D = ToTarget.SizeSquared();
		const float MaxAttackDistance = 150.0f;
		const float MinForwardDot = 0.0f;

		if (DistanceSq2D > MaxAttackDistance * MaxAttackDistance)
		{
			UE_LOG(LogSignet, Warning, TEXT("GA_Attack blocked: out of range. Dist=%.2f Max=%.2f Target=%s"),
				FMath::Sqrt(DistanceSq2D), MaxAttackDistance, *GetNameSafe(Target));
			EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
			return;
		}

		if (!ToTarget.IsNearlyZero() && !Forward.IsNearlyZero())
		{
			const FVector ToTargetNormalized = ToTarget.GetSafeNormal();
			const FVector ForwardNormalized = Forward.GetSafeNormal();
			const float ForwardDot = FVector::DotProduct(ForwardNormalized, ToTargetNormalized);

			if (ForwardDot < MinForwardDot)
			{
				UE_LOG(LogSignet, Warning, TEXT("GA_Attack blocked: target behind. Dot=%.3f Target=%s"),
					ForwardDot, *GetNameSafe(Target));
				EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
				return;
			}
		}
	}
	else
	{
		UE_LOG(LogSignet, Warning, TEXT("GA_Attack: no valid target or avatar. Target=%s Avatar=%s"),
			*GetNameSafe(Target), *GetNameSafe(ActorInfo ? ActorInfo->AvatarActor.Get() : nullptr));
	}

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
			if (ASignetPlayerCharacter* Character = Cast<ASignetPlayerCharacter>(Attacker))
			{
				const USignetInventoryComponent* Inventory = Character->GetInventoryComponent();
				UAbilitySystemComponent* TargetAsc = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Target);
				if (Inventory && TargetAsc)
				{
					const int32 VictimLevel = static_cast<int32>(TargetAsc->GetNumericAttribute(USignetPrimaryAttributeSet::GetJobLevelAttribute()));
					USkillRankStatics::TrySkillUp(Character, Inventory->GetEquippedWeaponSkill(), VictimLevel);
				}
			}

			if (Result.bIsHit)
			{
				// Hit Flash on Target
				if (ICombatInterface* TargetCombatant = Cast<ICombatInterface>(Target))
				{
					TargetCombatant->HitFlash();
					TargetCombatant->PlayVocalization(EVocalizationType::DamageReact);
					TargetCombatant->DamageCombatText((float)Result.Damage, Result.bIsCritical);

					// Apply Damage GameplayEffect
					UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Target);
					if (DamageEffectClass && TargetASC)
					{
						FGameplayEffectContextHandle Context = SignetASC->MakeEffectContext();
						Context.AddInstigator(Attacker, Attacker);
						
						FGameplayEffectSpecHandle SpecHandle = SignetASC->MakeOutgoingSpec(DamageEffectClass, 1.f, Context);
						if (SpecHandle.IsValid())
						{
							static const FGameplayTag DamageAmountTag = FGameplayTag::RequestGameplayTag(TEXT("Data.Damage"), false);
							if (DamageAmountTag.IsValid())
							{
								SpecHandle.Data.Get()->SetSetByCallerMagnitude(DamageAmountTag, (float)Result.Damage);
							}
							else
							{
								// Fallback: many projects use "Damage" name for SetByCaller if tags aren't used
								SpecHandle.Data.Get()->SetSetByCallerMagnitude(FName("Damage"), (float)Result.Damage);
							}
							
							SignetASC->ApplyGameplayEffectSpecToTarget(*SpecHandle.Data.Get(), TargetASC);
						}
					}

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

	AActor* Attacker = GetAvatarActorFromActorInfo();
	USignetAbilitySystemComponent* AttackerAsc = Cast<USignetAbilitySystemComponent>(GetAbilitySystemComponentFromActorInfo());
	USignetAbilitySystemComponent* TargetAsc = Target ? Cast<USignetAbilitySystemComponent>(Target->GetComponentByClass(USignetAbilitySystemComponent::StaticClass())) : nullptr;

	if (!AttackerAsc || !TargetAsc) return Results;

	const FSkillTags& SkillTags = FTagCache::Get().Skill;
	FGameplayTag WeaponSkill = SkillTags.None;
	bool bIsH2H = false;
	bool bDualWield = false;

	if (const ASignetPlayerCharacter* Character = Cast<ASignetPlayerCharacter>(Attacker))
	{
		if (const USignetInventoryComponent* Inventory = Character->GetInventoryComponent())
		{
			WeaponSkill = Inventory->GetEquippedWeaponSkill();
			bIsH2H = (WeaponSkill == SkillTags.H2H || WeaponSkill == SkillTags.None);
			bDualWield = (Inventory->GetEquippedItemID(EGearSlot::Sub) != 0);
		}
	}

	// 1. Determine Number of Attacks (Multi-hit logic)
	int32 MainHandHits = 1;
	int32 SubHandHits = 0;

	// Ported multi-hit logic from CAttackRound::CreateAttacks
	float TripleAttackRate = AttackerAsc->GetNumericAttribute(USignetCombatAttributeSet::GetTripleAttackRateAttribute());
	float DoubleAttackRate = AttackerAsc->GetNumericAttribute(USignetCombatAttributeSet::GetDoubleAttackRateAttribute());
	float QuadAttackRate = AttackerAsc->GetNumericAttribute(USignetCombatAttributeSet::GetQuadAttackRateAttribute());

	EPhysicalAttackType RoundType = EPhysicalAttackType::Normal;

	if (FMath::RandRange(0.0f, 100.0f) < QuadAttackRate)
	{
		MainHandHits = 4;
		RoundType = EPhysicalAttackType::Quad;
	}
	else if (FMath::RandRange(0.0f, 100.0f) < TripleAttackRate)
	{
		MainHandHits = 3;
		RoundType = EPhysicalAttackType::Triple;
	}
	else if (FMath::RandRange(0.0f, 100.0f) < DoubleAttackRate)
	{
		MainHandHits = 2;
		RoundType = EPhysicalAttackType::Double;
	}

	if (bIsH2H)
	{
		// H2H always swings twice if not a single swing mob
		SubHandHits = 1;
	}
	else if (bDualWield)
	{
		SubHandHits = 1;
	}

	// 2. Generate Swings
	auto CreateSwing = [&](EGearSlot Slot, EPhysicalAttackDirection Direction, bool bIsFirstSwingInRound)
	{
		FAttackResult Result;
		Result.AttackDirection = Direction;
		Result.AttackType = RoundType;
		Result.bIsFirstSwing = bIsFirstSwingInRound;

		// Hit Rate
		float HitRate = USignetCombatStatics::CalculateHitRate(AttackerAsc, TargetAsc);
		Result.bIsHit = (FMath::RandRange(0.0f, 100.0f) < HitRate);

		if (Result.bIsHit)
		{
			// Anticipation (Third Eye)
			if (USignetCombatStatics::CheckAnticipated(AttackerAsc, TargetAsc))
			{
				Result.bIsHit = false;
				Result.bIsAnticipated = true;
			}
			else
			{
				// Critical Hit
				float CritRate = AttackerAsc->GetNumericAttribute(USignetCombatAttributeSet::GetCriticalHitRateAttribute());
				Result.bIsCritical = (FMath::RandRange(0.0f, 100.0f) < CritRate);

				// Damage Ratio (pDIF)
				float pDIF = USignetCombatStatics::CalculateDamageRatio(AttackerAsc, TargetAsc, Result.bIsCritical, 1.0f, WeaponSkill, Slot);

				// Base Damage
				Result.Damage = USignetCombatStatics::CalculateBaseDamage(AttackerAsc, TargetAsc, Slot, RoundType, bIsH2H, bIsFirstSwingInRound, false, false, pDIF);

				// Block/Guard
				if (USignetCombatStatics::CheckShieldBlock(AttackerAsc, TargetAsc))
				{
					Result.bIsBlocked = true;
					Result.Damage /= 2; // Shield block reduces damage
				}
			}
		}

		return Result;
	};

		// Main Hand Swings
	for (int32 i = 0; i < MainHandHits; ++i)
	{
		FAttackResult Result = CreateSwing(EGearSlot::Main, EPhysicalAttackDirection::Right, i == 0);
		Results.Add(Result);
	}

	// Sub Hand Swings
	for (int32 i = 0; i < SubHandHits; ++i)
	{
		FAttackResult Result = CreateSwing(EGearSlot::Sub, EPhysicalAttackDirection::Left, Results.Num() == 0);
		Results.Add(Result);
	}

	// Kick Attacks
	float KickRate = USignetCombatStatics::CalculateKickAttackRate(AttackerAsc);
	if (bIsH2H && FMath::RandRange(0.0f, 100.0f) < KickRate)
	{
		FAttackResult KickResult = CreateSwing(EGearSlot::Main, EPhysicalAttackDirection::Right, false);
		KickResult.AttackType = EPhysicalAttackType::Kick;
		Results.Add(KickResult);
	}

	return Results;
}
