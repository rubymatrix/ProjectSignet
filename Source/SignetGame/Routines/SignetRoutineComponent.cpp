// Copyright Epic Games, Inc. All Rights Reserved.

#include "SignetRoutineComponent.h"
#include "AIController.h"
#include "Components/SplineComponent.h"
#include "GameFramework/Pawn.h"
#include "Kismet/GameplayStatics.h"
#include "SignetSplineFollowingComponent.h"
#include "TimerManager.h"
#include "SignetGame/Util/Logging.h"

USignetRoutineComponent::USignetRoutineComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	SetIsReplicatedByDefault(true);

	ActiveStepIndex = INDEX_NONE;
	bWaitingForAction = false;
	bIsRunning = false;
	bOverrideReversePath = false;
	bOverrideReversePathValue = false;
}

void USignetRoutineComponent::BeginPlay()
{
	Super::BeginPlay();

	SplineFollower = GetOwner()->FindComponentByClass<USignetSplineFollowingComponent>();
	if (IsValid(SplineFollower))
	{
		SplineFollower->OnPathCompleted.AddDynamic(this, &USignetRoutineComponent::HandlePathCompleted);
	}

	if (bAutoStart && IsValid(RoutineDefinition))
	{
		StartRoutine(RoutineDefinition);
	}
}

void USignetRoutineComponent::StartRoutine(USignetRoutineDefinition* RoutineToRun)
{
	if (!GetOwner() || !GetOwner()->HasAuthority())
	{
		return;
	}

	USignetRoutineDefinition* Definition = IsValid(RoutineToRun) ? RoutineToRun : RoutineDefinition.Get();
	if (!IsValid(Definition))
	{
		UE_LOG(LogSignet, Warning, TEXT("RoutineComponent: Missing routine definition on %s."), *GetNameSafe(GetOwner()));
		return;
	}

	StopRoutine();

	ActiveRoutine = Definition;

	if (!EnsureControlledPawn())
	{
		UE_LOG(LogSignet, Warning, TEXT("RoutineComponent: No pawn available for routine on %s."), *GetNameSafe(GetOwner()));
		StopRoutine();
		return;
	}

	ActiveStepIndex = 0;
	bIsRunning = true;

	UE_LOG(LogSignet, Log, TEXT("RoutineComponent: Starting routine %s on %s."), *GetNameSafe(ActiveRoutine.Get()), *GetNameSafe(GetOwner()));
	ExecuteCurrentStep();
}

void USignetRoutineComponent::StopRoutine()
{
	if (GetOwner() && !GetOwner()->HasAuthority())
	{
		return;
	}

	ClearStepTimer();

	if (IsValid(SplineFollower))
	{
		SplineFollower->StopFollowing();
	}

	bIsRunning = false;
	bWaitingForAction = false;
	ActiveStepIndex = INDEX_NONE;

	if (IsValid(ActiveRoutine) && ActiveRoutine->bDestroyPawnOnFinish && SpawnedPawn.IsValid())
	{
		AAIController* Controller = GetOwningController();
		if (IsValid(Controller) && Controller->GetPawn() == SpawnedPawn.Get())
		{
			Controller->UnPossess();
		}

		SpawnedPawn->Destroy();
	}

	SpawnedPawn.Reset();
	ActiveRoutine = nullptr;
}

void USignetRoutineComponent::CompleteActiveAction()
{
	if (!bWaitingForAction)
	{
		return;
	}

	bWaitingForAction = false;
	AdvanceStep();
}

bool USignetRoutineComponent::IsRoutineActive() const
{
	return bIsRunning;
}

void USignetRoutineComponent::SetReversePathOverride(bool bEnabled, bool bReversePath)
{
	bOverrideReversePath = bEnabled;
	bOverrideReversePathValue = bReversePath;
}

void USignetRoutineComponent::ExecuteCurrentStep()
{
	if (!bIsRunning || !IsValid(ActiveRoutine))
	{
		return;
	}

	if (!ActiveRoutine->Steps.IsValidIndex(ActiveStepIndex))
	{
		FinishRoutine();
		return;
	}

	const FSignetRoutineStep& Step = ActiveRoutine->Steps[ActiveStepIndex];
	switch (Step.StepType)
	{
	case ESignetRoutineStepType::FollowPath:
	{
		UE_LOG(LogSignet, Log, TEXT("RoutineComponent: Step %d FollowPath on %s."), ActiveStepIndex, *GetNameSafe(GetOwner()));
		if (!IsValid(SplineFollower))
		{
			UE_LOG(LogSignet, Warning, TEXT("RoutineComponent: Missing spline follower on %s."), *GetNameSafe(GetOwner()));
			AdvanceStep();
			return;
		}

		USplineComponent* Spline = ChooseSplineFromTags(Step.Path.SplineActorTags);
		if (!IsValid(Spline))
		{
			UE_LOG(LogSignet, Warning, TEXT("RoutineComponent: No spline available for step %d on %s."), ActiveStepIndex, *GetNameSafe(GetOwner()));
			AdvanceStep();
			return;
		}

		const bool bReversePath = bOverrideReversePath ? bOverrideReversePathValue : Step.Path.bReversePath;
		SplineFollower->FollowPath(Spline, bReversePath);
		break;
	}
	case ESignetRoutineStepType::PerformAction:
	{
		APawn* Pawn = GetControlledPawn();
		bWaitingForAction = true;
		UE_LOG(LogSignet, Log, TEXT("RoutineComponent: Step %d PerformAction %s on %s."), ActiveStepIndex, *Step.Action.ActionTag.ToString(), *GetNameSafe(GetOwner()));
		OnRoutineActionStarted.Broadcast(Step.Action.ActionTag, Pawn);
		MulticastActionStarted(Step.Action.ActionTag, Pawn);

		if (!Step.Action.bWaitForExternalCompletion)
		{
			if (Step.Action.Duration > 0.0f)
			{
				GetWorld()->GetTimerManager().SetTimer(StepTimerHandle, this, &USignetRoutineComponent::CompleteActiveAction, Step.Action.Duration, false);
			}
			else
			{
				CompleteActiveAction();
			}
		}
		break;
	}
	case ESignetRoutineStepType::Wait:
	{
		UE_LOG(LogSignet, Log, TEXT("RoutineComponent: Step %d Wait %.2f on %s."), ActiveStepIndex, Step.WaitDuration, *GetNameSafe(GetOwner()));
		if (Step.WaitDuration > 0.0f)
		{
			GetWorld()->GetTimerManager().SetTimer(StepTimerHandle, this, &USignetRoutineComponent::AdvanceStep, Step.WaitDuration, false);
		}
		else
		{
			AdvanceStep();
		}
		break;
	}
	case ESignetRoutineStepType::Despawn:
	default:
		UE_LOG(LogSignet, Log, TEXT("RoutineComponent: Step %d Despawn on %s."), ActiveStepIndex, *GetNameSafe(GetOwner()));
		FinishRoutine();
		break;
	}
}

void USignetRoutineComponent::AdvanceStep()
{
	if (!bIsRunning)
	{
		return;
	}

	ClearStepTimer();

	ActiveStepIndex++;
	ExecuteCurrentStep();
}

void USignetRoutineComponent::HandlePathCompleted(USplineComponent* SplineComponent)
{
	if (!bIsRunning || !IsValid(ActiveRoutine))
	{
		return;
	}

	if (!ActiveRoutine->Steps.IsValidIndex(ActiveStepIndex))
	{
		return;
	}

	const FSignetRoutineStep& Step = ActiveRoutine->Steps[ActiveStepIndex];
	if (Step.StepType != ESignetRoutineStepType::FollowPath)
	{
		return;
	}

	AdvanceStep();
}

void USignetRoutineComponent::MulticastActionStarted_Implementation(FGameplayTag ActionTag, APawn* Pawn)
{
	if (!GetOwner() || GetOwner()->HasAuthority())
	{
		return;
	}

	OnRoutineActionStarted.Broadcast(ActionTag, Pawn);
}

bool USignetRoutineComponent::EnsureControlledPawn()
{
	AAIController* Controller = GetOwningController();
	if (!IsValid(Controller))
	{
		return false;
	}

	APawn* Pawn = Controller->GetPawn();
	if (IsValid(Pawn))
	{
		if (IsValid(ActiveRoutine) && ActiveRoutine->bTeleportToStart)
		{
			ApplyStartTransform(Pawn);
		}

		return true;
	}

	if (!IsValid(ActiveRoutine) || !ActiveRoutine->PawnClass)
	{
		return false;
	}

	FTransform SpawnTransform;
	if (!ResolveStartTransform(SpawnTransform))
	{
		SpawnTransform = GetOwner()->GetActorTransform();
	}

	UWorld* World = GetWorld();
	if (!IsValid(World))
	{
		return false;
	}

	APawn* Spawned = World->SpawnActor<APawn>(ActiveRoutine->PawnClass, SpawnTransform);
	if (!IsValid(Spawned))
	{
		return false;
	}

	Controller->Possess(Spawned);
	SpawnedPawn = Spawned;

	if (ActiveRoutine->bTeleportToStart)
	{
		ApplyStartTransform(Spawned);
	}

	return true;
}

bool USignetRoutineComponent::ResolveStartTransform(FTransform& OutTransform) const
{
	OutTransform = FTransform::Identity;

	TArray<AActor*> ValidStarts;
	for (AActor* StartActor : StartLocations)
	{
		if (IsValid(StartActor))
		{
			ValidStarts.Add(StartActor);
		}
	}

	if (ValidStarts.IsEmpty())
	{
		return false;
	}

	const int32 Index = FMath::RandRange(0, ValidStarts.Num() - 1);
	OutTransform = ValidStarts[Index]->GetActorTransform();
	return true;
}

bool USignetRoutineComponent::ApplyStartTransform(APawn* Pawn) const
{
	if (!IsValid(Pawn))
	{
		return false;
	}

	FTransform StartTransform;
	if (!ResolveStartTransform(StartTransform))
	{
		return false;
	}

	Pawn->SetActorTransform(StartTransform, false, nullptr, ETeleportType::TeleportPhysics);
	return true;
}

APawn* USignetRoutineComponent::GetControlledPawn() const
{
	AAIController* Controller = GetOwningController();
	if (!IsValid(Controller))
	{
		return nullptr;
	}

	return Controller->GetPawn();
}

AAIController* USignetRoutineComponent::GetOwningController() const
{
	return Cast<AAIController>(GetOwner());
}

USplineComponent* USignetRoutineComponent::ChooseSplineFromTags(const TArray<FName>& Tags) const
{
	UWorld* World = GetWorld();
	if (!IsValid(World))
	{
		return nullptr;
	}

	TArray<FName> ValidTags;
	ValidTags.Reserve(Tags.Num());
	for (const FName& Tag : Tags)
	{
		if (Tag.IsNone())
		{
			continue;
		}

		ValidTags.Add(Tag);
	}

	if (ValidTags.IsEmpty())
	{
		return nullptr;
	}

	const int32 RandomStartIndex = FMath::RandRange(0, ValidTags.Num() - 1);
	for (int32 Offset = 0; Offset < ValidTags.Num(); Offset++)
	{
		const int32 TagIndex = (RandomStartIndex + Offset) % ValidTags.Num();
		const FName Tag = ValidTags[TagIndex];

		TArray<AActor*> TaggedActors;
		UGameplayStatics::GetAllActorsWithTag(World, Tag, TaggedActors);

		TArray<USplineComponent*> ValidSplines;
		ValidSplines.Reserve(TaggedActors.Num());
		for (AActor* Actor : TaggedActors)
		{
			if (!IsValid(Actor))
			{
				continue;
			}

			USplineComponent* Spline = Actor->FindComponentByClass<USplineComponent>();
			if (IsValid(Spline))
			{
				ValidSplines.Add(Spline);
			}
		}

		if (!ValidSplines.IsEmpty())
		{
			const int32 SplineIndex = FMath::RandRange(0, ValidSplines.Num() - 1);
			return ValidSplines[SplineIndex];
		}
	}

	return nullptr;
}

void USignetRoutineComponent::FinishRoutine()
{
	UE_LOG(LogSignet, Log, TEXT("RoutineComponent: Routine complete on %s."), *GetNameSafe(GetOwner()));
	OnRoutineCompleted.Broadcast();
	StopRoutine();
}

void USignetRoutineComponent::ClearStepTimer()
{
	if (UWorld* World = GetWorld())
	{
		World->GetTimerManager().ClearTimer(StepTimerHandle);
	}
}
