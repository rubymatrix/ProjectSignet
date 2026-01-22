// Copyright Epic Games, Inc. All Rights Reserved.

#include "SignetRoutineSpawnerActor.h"

#include "AIController.h"
#include "CollisionQueryParams.h"
#include "Components/SplineComponent.h"
#include "Engine/Engine.h"
#include "Engine/EngineTypes.h"
#include "Engine/OverlapResult.h"
#include "Engine/World.h"
#include "GameFramework/Pawn.h"
#include "HAL/PlatformTime.h"
#include "Kismet/GameplayStatics.h"
#include "SignetRoutineComponent.h"
#include "SignetRoutineDefinition.h"
#include "SignetSplineFollowingComponent.h"
#include "TimerManager.h"

ASignetRoutineSpawnerActor::ASignetRoutineSpawnerActor()
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = false;

	MinPoolSize = 0;
	MaxPoolSize = 5;
	MaintainIntervalSeconds = 1.0f;
	RespawnDelaySeconds = 0.0f;
	SpawnCollisionRadius = 120.0f;
	bAutoStart = true;
	bVerbose = false;

	ControllerClass = AAIController::StaticClass();

	LastSpawnTimeSeconds = -1.0;
}

void ASignetRoutineSpawnerActor::VerboseScreenMessage(const FString& Message) const
{
	if (!bVerbose)
	{
		return;
	}

	if (!GEngine)
	{
		return;
	}

	const FString FullMessage = FString::Printf(TEXT("[%s] %s"), *GetNameSafe(this), *Message);
	GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Cyan, FullMessage);
}

void ASignetRoutineSpawnerActor::BeginPlay()
{
	Super::BeginPlay();

	RandomStream.Initialize(static_cast<int32>((FPlatformTime::Cycles64() & 0x7fffffff) ^ static_cast<uint64>(GetUniqueID())));

	if (bAutoStart)
	{
		VerboseScreenMessage(TEXT("AutoStart -> StartSpawner"));
		StartSpawner();
	}
}

void ASignetRoutineSpawnerActor::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	StopSpawner(true);

	Super::EndPlay(EndPlayReason);
}

void ASignetRoutineSpawnerActor::StartSpawner()
{
	if (!HasAuthority())
	{
		return;
	}

	if (!GetWorld())
	{
		return;
	}

	GetWorld()->GetTimerManager().ClearTimer(MaintainTimerHandle);
	GetWorld()->GetTimerManager().SetTimer(MaintainTimerHandle, this, &ASignetRoutineSpawnerActor::MaintainPool, MaintainIntervalSeconds, true);

	VerboseScreenMessage(FString::Printf(TEXT("StartSpawner (Min=%d Max=%d Interval=%.2f)"), MinPoolSize, MaxPoolSize, MaintainIntervalSeconds));
	MaintainPool();
}

void ASignetRoutineSpawnerActor::StopSpawner(bool bDestroySpawnedControllers)
{
	if (!HasAuthority())
	{
		return;
	}

	if (UWorld* World = GetWorld())
	{
		World->GetTimerManager().ClearTimer(MaintainTimerHandle);
	}

	if (bDestroySpawnedControllers)
	{
		VerboseScreenMessage(FString::Printf(TEXT("StopSpawner destroying %d controller(s)"), SpawnedControllers.Num()));
		for (AAIController* Controller : SpawnedControllers)
		{
			if (IsValid(Controller))
			{
				Controller->Destroy();
			}
		}
		SpawnedControllers.Reset();
	}
}

int32 ASignetRoutineSpawnerActor::GetActivePawnCount() const
{
	return CountPawnsInternal();
}

void ASignetRoutineSpawnerActor::MaintainPool()
{
	if (!HasAuthority())
	{
		return;
	}

	if (!IsValid(RoutineDefinition))
	{
		return;
	}

	if (MaxPoolSize < MinPoolSize)
	{
		MaxPoolSize = MinPoolSize;
	}

	CleanupControllerList();

	const int32 ActivePawnCount = CountPawnsInternal();
	if (ActivePawnCount >= MinPoolSize)
	{
		return;
	}

	UWorld* World = GetWorld();
	const double NowSeconds = IsValid(World) ? World->GetTimeSeconds() : 0.0;
	if (RespawnDelaySeconds > 0.0f && LastSpawnTimeSeconds >= 0.0 && (NowSeconds - LastSpawnTimeSeconds) < RespawnDelaySeconds)
	{
		return;
	}

	const int32 DesiredSpawnCount = FMath::Min(MinPoolSize - ActivePawnCount, MaxPoolSize - ActivePawnCount);
	for (int32 i = 0; i < DesiredSpawnCount; i++)
	{
		bool bStartedExisting = false;
		for (AAIController* ExistingController : SpawnedControllers)
		{
			if (!IsValid(ExistingController))
			{
				continue;
			}

			if (IsValid(ExistingController->GetPawn()))
			{
				continue;
			}

			if (TryStartRoutineOnController(ExistingController))
			{
				bStartedExisting = true;
				LastSpawnTimeSeconds = NowSeconds;
				VerboseScreenMessage(FString::Printf(TEXT("Reused controller %s -> started routine"), *GetNameSafe(ExistingController)));
				break;
			}
		}

		if (bStartedExisting)
		{
			if (RespawnDelaySeconds > 0.0f)
			{
				return;
			}
			continue;
		}

		if (SpawnedControllers.Num() >= MaxPoolSize)
		{
			return;
		}

		if (!IsValid(World))
		{
			return;
		}

		UClass* SpawnControllerClass = ControllerClass ? ControllerClass.Get() : AAIController::StaticClass();
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		AAIController* NewController = World->SpawnActor<AAIController>(SpawnControllerClass, GetActorTransform(), SpawnParams);
		if (!IsValid(NewController))
		{
			return;
		}

		SpawnedControllers.Add(NewController);
		VerboseScreenMessage(FString::Printf(TEXT("Spawned controller %s (%s)"), *GetNameSafe(NewController), *GetNameSafe(SpawnControllerClass)));
		if (TryStartRoutineOnController(NewController))
		{
			LastSpawnTimeSeconds = NowSeconds;
			if (RespawnDelaySeconds > 0.0f)
			{
				return;
			}
		}
	}
}

bool ASignetRoutineSpawnerActor::EnsureControllerSetup(AAIController* Controller, USignetRoutineComponent*& OutRoutineComponent) const
{
	OutRoutineComponent = nullptr;

	if (!IsValid(Controller))
	{
		return false;
	}

	USignetSplineFollowingComponent* SplineFollowing = Controller->FindComponentByClass<USignetSplineFollowingComponent>();
	if (!IsValid(SplineFollowing))
	{
		SplineFollowing = NewObject<USignetSplineFollowingComponent>(Controller);
		if (!IsValid(SplineFollowing))
		{
			return false;
		}
		SplineFollowing->RegisterComponent();
	}

	USignetRoutineComponent* RoutineComp = Controller->FindComponentByClass<USignetRoutineComponent>();
	if (!IsValid(RoutineComp))
	{
		RoutineComp = NewObject<USignetRoutineComponent>(Controller);
		if (!IsValid(RoutineComp))
		{
			return false;
		}
		RoutineComp->RegisterComponent();
	}

	OutRoutineComponent = RoutineComp;
	return true;
}

bool ASignetRoutineSpawnerActor::ResolveInitialPathSpline(USplineComponent*& OutSpline, bool& bOutReverse) const
{
	OutSpline = nullptr;
	bOutReverse = RandomStream.RandRange(0, 1) == 1;

	if (!IsValid(RoutineDefinition))
	{
		return false;
	}

	UWorld* World = GetWorld();
	if (!IsValid(World))
	{
		return false;
	}

	const int32 StepCount = RoutineDefinition->Steps.Num();
	if (StepCount <= 0)
	{
		return false;
	}

	int32 InitialPathIndex = INDEX_NONE;
	for (int32 Index = 0; Index < StepCount; Index++)
	{
		const FSignetRoutineStep& Step = RoutineDefinition->Steps[Index];
		if (Step.StepType == ESignetRoutineStepType::FollowPath)
		{
			InitialPathIndex = Index;
			break;
		}
	}

	if (InitialPathIndex == INDEX_NONE)
	{
		return false;
	}

	const FSignetRoutineStep& PathStep = RoutineDefinition->Steps[InitialPathIndex];

	TArray<FName> ValidTags;
	ValidTags.Reserve(PathStep.Path.SplineActorTags.Num());
	for (const FName& Tag : PathStep.Path.SplineActorTags)
	{
		if (!Tag.IsNone())
		{
			ValidTags.Add(Tag);
		}
	}

	if (ValidTags.IsEmpty())
	{
		return false;
	}

	const int32 RandomTagIndex = RandomStream.RandRange(0, ValidTags.Num() - 1);
	const FName Tag = ValidTags[RandomTagIndex];

	TArray<AActor*> TaggedActors;
	UGameplayStatics::GetAllActorsWithTag(World, Tag, TaggedActors);
	if (TaggedActors.IsEmpty())
	{
		return false;
	}

	const int32 RandomActorIndex = RandomStream.RandRange(0, TaggedActors.Num() - 1);
	for (int32 ActorOffset = 0; ActorOffset < TaggedActors.Num(); ActorOffset++)
	{
		const int32 ActorIndex = (RandomActorIndex + ActorOffset) % TaggedActors.Num();
		AActor* CandidateActor = TaggedActors[ActorIndex];
		if (!IsValid(CandidateActor))
		{
			continue;
		}

		USplineComponent* Spline = CandidateActor->FindComponentByClass<USplineComponent>();
		if (!IsValid(Spline))
		{
			continue;
		}

		OutSpline = Spline;
		return true;
	}

	return false;
}

bool ASignetRoutineSpawnerActor::ChooseFreeSpawnTransform(FTransform& OutSpawnTransform, USplineComponent*& OutSpline, bool& bOutReverse) const
{
	OutSpawnTransform = FTransform::Identity;
	OutSpline = nullptr;
	bOutReverse = false;

	UWorld* World = GetWorld();
	if (!IsValid(World))
	{
		return false;
	}

	if (!ResolveInitialPathSpline(OutSpline, bOutReverse))
	{
		return false;
	}

	const float SplineLength = OutSpline->GetSplineLength();
	const float Distance = bOutReverse ? SplineLength : 0.0f;
	const FVector Location = OutSpline->GetLocationAtDistanceAlongSpline(Distance, ESplineCoordinateSpace::World);
	const FRotator Rotation = OutSpline->GetRotationAtDistanceAlongSpline(Distance, ESplineCoordinateSpace::World);
	OutSpawnTransform = FTransform(Rotation, Location, FVector::OneVector);

	if (SpawnCollisionRadius <= 0.0f)
	{
		return true;
	}

	FCollisionObjectQueryParams ObjectQuery;
	ObjectQuery.AddObjectTypesToQuery(ECC_Pawn);

	FCollisionQueryParams QueryParams(SCENE_QUERY_STAT(SignetRoutineSpawnerOverlap), false);
	QueryParams.AddIgnoredActor(this);

	TArray<FOverlapResult> Overlaps;
	const bool bHasOverlap = World->OverlapMultiByObjectType(
		Overlaps,
		Location,
		FQuat::Identity,
		ObjectQuery,
		FCollisionShape::MakeSphere(SpawnCollisionRadius),
		QueryParams);

	if (!bHasOverlap)
	{
		return true;
	}

	for (const FOverlapResult& Result : Overlaps)
	{
		const AActor* OverlapActor = Result.GetActor();
		if (IsValid(OverlapActor) && OverlapActor->IsA<APawn>())
		{
			return false;
		}
	}

	return true;
}

bool ASignetRoutineSpawnerActor::TryStartRoutineOnController(AAIController* Controller)
{
	if (!HasAuthority())
	{
		return false;
	}

	if (!IsValid(RoutineDefinition))
	{
		return false;
	}

	USignetRoutineComponent* RoutineComp = nullptr;
	if (!EnsureControllerSetup(Controller, RoutineComp))
	{
		return false;
	}

	FTransform SpawnTransform = FTransform::Identity;
	USplineComponent* InitialSpline = nullptr;
	bool bReverseInitialPath = false;
	if (!ChooseFreeSpawnTransform(SpawnTransform, InitialSpline, bReverseInitialPath))
	{
		VerboseScreenMessage(TEXT("No free spawn transform available (initial spline start occupied or no valid spline found)"));
		return false;
	}

	if (!IsValid(Controller->GetPawn()))
	{
		if (!RoutineDefinition->PawnClass)
		{
			VerboseScreenMessage(TEXT("RoutineDefinition missing PawnClass"));
			return false;
		}

		UWorld* World = GetWorld();
		if (!IsValid(World))
		{
			VerboseScreenMessage(TEXT("Missing World while spawning pawn"));
			return false;
		}

		VerboseScreenMessage(FString::Printf(
			TEXT("Spawning pawn class=%s at Loc=(%.1f,%.1f,%.1f) Rot=(P=%.1f,Y=%.1f,R=%.1f) Spline=%s Reverse=%s Radius=%.1f"),
			*GetNameSafe(RoutineDefinition->PawnClass.Get()),
			SpawnTransform.GetLocation().X, SpawnTransform.GetLocation().Y, SpawnTransform.GetLocation().Z,
			SpawnTransform.GetRotation().Rotator().Pitch, SpawnTransform.GetRotation().Rotator().Yaw, SpawnTransform.GetRotation().Rotator().Roll,
			*GetNameSafe(InitialSpline),
			bReverseInitialPath ? TEXT("true") : TEXT("false"),
			SpawnCollisionRadius));

		FActorSpawnParameters PawnSpawnParams;
		PawnSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;

		APawn* SpawnedPawn = World->SpawnActor<APawn>(RoutineDefinition->PawnClass, SpawnTransform, PawnSpawnParams);
		if (!IsValid(SpawnedPawn))
		{
			VerboseScreenMessage(FString::Printf(TEXT("Failed to spawn pawn (%s) at spline start (%s)"), *GetNameSafe(RoutineDefinition->PawnClass.Get()), *GetNameSafe(InitialSpline)));
			return false;
		}

		Controller->Possess(SpawnedPawn);
		VerboseScreenMessage(FString::Printf(TEXT("Spawned pawn %s (%s) possessed by %s"), *GetNameSafe(SpawnedPawn), *GetNameSafe(RoutineDefinition->PawnClass.Get()), *GetNameSafe(Controller)));
	}

	RoutineComp->RoutineDefinition = RoutineDefinition;
	RoutineComp->bAutoStart = false;
	RoutineComp->SetReversePathOverride(true, bReverseInitialPath);
	RoutineComp->StartRoutine(RoutineDefinition);
	VerboseScreenMessage(FString::Printf(TEXT("Started routine %s on controller %s (initial spline %s reverse=%s)"), *GetNameSafe(RoutineDefinition), *GetNameSafe(Controller), *GetNameSafe(InitialSpline), bReverseInitialPath ? TEXT("true") : TEXT("false")));
	return true;
}

void ASignetRoutineSpawnerActor::CleanupControllerList()
{
	for (int32 Index = SpawnedControllers.Num() - 1; Index >= 0; Index--)
	{
		AAIController* Controller = SpawnedControllers[Index];
		if (!IsValid(Controller))
		{
			SpawnedControllers.RemoveAtSwap(Index, 1, EAllowShrinking::No);
			continue;
		}

		if (Controller->IsPendingKillPending())
		{
			SpawnedControllers.RemoveAtSwap(Index, 1, EAllowShrinking::No);
		}
	}
}

int32 ASignetRoutineSpawnerActor::CountPawnsInternal() const
{
	int32 Count = 0;
	for (AAIController* Controller : SpawnedControllers)
	{
		if (!IsValid(Controller))
		{
			continue;
		}

		if (IsValid(Controller->GetPawn()))
		{
			Count++;
		}
	}
	return Count;
}
