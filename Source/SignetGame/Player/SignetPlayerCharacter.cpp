// Copyright Red Lotus Games, All Rights Reserved.

#include "SignetPlayerCharacter.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "SignetPlayerState.h"
#include "Components/InventoryComponent.h"
#include "Components/SignetCameraComponent.h"
#include "Components/StatsComponent.h"
#include "Components/TargetingComponent.h"
#include "Utility/AlsVector.h"
#include "Net/UnrealNetwork.h"
#include "SignetGame/Abilities/SignetAbilitySystemComponent.h"
#include "SignetGame/Abilities/TagCache.h"
#include "SignetGame/Abilities/Attributes/SignetPrimaryAttributeSet.h"
#include "SignetGame/Inventory/SignetInventoryComponent.h"
#include "SignetGame/Util/Logging.h"
#include "SignetGame/Util/Stats.h"

bool ASignetPlayerCharacter::IsAscValid() const
{
	return IsValid(GetSignetAsc());
}

USignetAbilitySystemComponent* ASignetPlayerCharacter::GetSignetAsc() const
{
	return Cast<USignetAbilitySystemComponent>(GetAbilitySystemComponent());
}

ASignetPlayerCharacter::ASignetPlayerCharacter(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer.SetDefaultSubobjectClass<USignetAbilitySystemComponent>(TEXT("AbilitySystemComponent")))
{
	// Initialize the camera system
	Camera = CreateDefaultSubobject<USignetCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(GetMesh());
	Camera->SetRelativeLocation_Direct({0.f, 90.f, 0.f});
	
	auto CreateMeshObject = [&](FName InName, FName Tag)
	{
		auto MeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(InName); 

		MeshComp->SetupAttachment(GetMesh());
		MeshComp->SetVisibility(false);
		MeshComp->SetLeaderPoseComponent(GetMesh(), true);
		MeshComp->SetBoundsScale(5.f);
		MeshComp->ComponentTags.Add(Tag);

		return MeshComp;
	};

	// Initialize all module mesh parts
	MainMesh = CreateMeshObject(TEXT("MainMesh"), TEXT("Main"));
	SubMesh = CreateMeshObject(TEXT("SubMesh"), TEXT("Sub"));
	RangedMesh = CreateMeshObject(TEXT("RangedMesh"), TEXT("Ranged"));
	FaceMesh = CreateMeshObject(TEXT("FaceMesh"), TEXT("Face"));
	HeadMesh = CreateMeshObject(TEXT("HeadMesh"), TEXT("Head"));
	BodyMesh = CreateMeshObject(TEXT("BodyMesh"), TEXT("Body"));
	HandsMesh = CreateMeshObject(TEXT("HandsMesh"), TEXT("Hands"));
	LegsMesh = CreateMeshObject(TEXT("LegsMesh"), TEXT("Legs"));
	FeetMesh = CreateMeshObject(TEXT("FeetMesh"), TEXT("Feet"));

	// Initialize Gameplay Components
	Stats = CreateDefaultSubobject<UStatsComponent>(TEXT("StatsComp"));
	Targeting = CreateDefaultSubobject<UTargetingComponent>(TEXT("TargetingComp"));
	Inventory = CreateDefaultSubobject<USignetInventoryComponent>(TEXT("Inventory"));
}

void ASignetPlayerCharacter::NotifyControllerChanged()
{
	// Remove the mapping context
	if (const auto PreviousPlayer = Cast<APlayerController>(PreviousController); IsValid(PreviousPlayer))
	{
		if (const auto InputSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PreviousPlayer->GetLocalPlayer()); IsValid(InputSubsystem))
		{
			InputSubsystem->RemoveMappingContext(DefaultMappingContext);
		}
	}

	// Bind new mapping context
	if (const auto NewPlayer = Cast<APlayerController>(GetController()); IsValid(NewPlayer))
	{
		if (const auto InputSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(NewPlayer->GetLocalPlayer()); IsValid(InputSubsystem))
		{
			FModifyContextOptions Options;
			Options.bNotifyUserSettings = true;
			InputSubsystem->AddMappingContext(DefaultMappingContext, 0, Options);
		}
	}
	
	Super::NotifyControllerChanged();
}

void ASignetPlayerCharacter::ResetBaseStats_Implementation()
{
	const auto Asc = Cast<USignetAbilitySystemComponent>(GetAbilitySystemComponent());
	if (!Asc)
	{
		UE_LOG(LogSignet, Error, TEXT("Unable to update Base Stats - ASC was NULL."));
		return;
	}

	const auto PS = GetPlayerState<ASignetPlayerState>();
	if (!PS)
	{
		UE_LOG(LogSignet, Error, TEXT("Unable to update Base Stats - Player State was NULL."));
		return;
	}
	
	// Remove the existing base stats effect
	if (BaseStatsEffectHandle.IsValid())
	{
		Asc->RemoveActiveGameplayEffect(BaseStatsEffectHandle);
	}

	FStatCalculation CalcParams;
	CalcParams.Job = static_cast<int>(PS->Job);
	CalcParams.JobLevel = PS->Level;
	CalcParams.SubJob = 0;
	CalcParams.SubJobLevel = 0;
	CalcParams.bIsPlayer = true;
	CalcParams.Race = static_cast<int>(PS->Race);

	const auto [HP, MP, STR, DEX, VIT, AGI, INT, MND, CHR] = UStatsFunctions::CalcBaseStats(CalcParams);

	auto Ctx = Asc->MakeEffectContext();
	Ctx.AddSourceObject(this);

	FGameplayEffectSpecHandle SpecHandle = Asc->MakeOutgoingSpec(BaseStatsEffectClass, /*Level=*/1.f, Ctx);
	if (!SpecHandle.IsValid()) return;

	FGameplayEffectSpec* Spec = SpecHandle.Data.Get();
	Spec->SetSetByCallerMagnitude(FTagCache::Get().Data.Job, CalcParams.Job);
	Spec->SetSetByCallerMagnitude(FTagCache::Get().Data.JobLevel, CalcParams.JobLevel);
	Spec->SetSetByCallerMagnitude(FTagCache::Get().Data.SubJob, CalcParams.SubJob);
	Spec->SetSetByCallerMagnitude(FTagCache::Get().Data.SubJobLevel, CalcParams.SubJobLevel);
	Spec->SetSetByCallerMagnitude(FTagCache::Get().Data.STR, STR);
	Spec->SetSetByCallerMagnitude(FTagCache::Get().Data.DEX, DEX);
	Spec->SetSetByCallerMagnitude(FTagCache::Get().Data.VIT, VIT);
	Spec->SetSetByCallerMagnitude(FTagCache::Get().Data.AGI, AGI);
	Spec->SetSetByCallerMagnitude(FTagCache::Get().Data.INT, INT);
	Spec->SetSetByCallerMagnitude(FTagCache::Get().Data.MND, MND);
	Spec->SetSetByCallerMagnitude(FTagCache::Get().Data.CHR, CHR);
	Spec->SetSetByCallerMagnitude(FTagCache::Get().Data.MaxHP,  static_cast<float>(HP));
	Spec->SetSetByCallerMagnitude(FTagCache::Get().Data.MaxMP,  static_cast<float>(MP));

	// Apply & Store Active Effect
	BaseStatsEffectHandle = Asc->ApplyGameplayEffectSpecToSelf(*Spec);

	// Heal the Player
	Heal();
}

void ASignetPlayerCharacter::Heal_Implementation()
{
	const auto Asc = Cast<USignetAbilitySystemComponent>(GetAbilitySystemComponent());
	if (!Asc)
	{
		UE_LOG(LogSignet, Error, TEXT("Unable to update Base Stats - ASC was NULL."));
		return;
	}

	bool bFound = false;
	const auto MaxHP = Asc->GetGameplayAttributeValue(USignetPrimaryAttributeSet::GetMaxHPAttribute(), bFound);
	const auto MaxMP = Asc->GetGameplayAttributeValue(USignetPrimaryAttributeSet::GetMaxMPAttribute(), bFound);

	auto Ctx = Asc->MakeEffectContext();
	Ctx.AddSourceObject(this);

	const auto SpecHandle = Asc->MakeOutgoingSpec(HealEffectClass, 1.f, Ctx);
	if (!SpecHandle.IsValid()) return;

	FGameplayEffectSpec* Spec = SpecHandle.Data.Get();
	Spec->SetSetByCallerMagnitude(FTagCache::Get().Data.HP, MaxHP);
	Spec->SetSetByCallerMagnitude(FTagCache::Get().Data.MP, MaxMP);
	Asc->ApplyGameplayEffectSpecToSelf(*Spec);
}

void ASignetPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void ASignetPlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (const auto EnhancedInput = Cast<UEnhancedInputComponent>(PlayerInputComponent); IsValid(EnhancedInput))
	{
		EnhancedInput->BindAction(LookMouseAction, ETriggerEvent::Triggered, this, &ThisClass::Input_OnLookMouse);
		EnhancedInput->BindAction(LookMouseAction, ETriggerEvent::Canceled, this, &ThisClass::Input_OnLookMouse);
		EnhancedInput->BindAction(LookAction, ETriggerEvent::Triggered, this, &ThisClass::Input_OnLook);
		EnhancedInput->BindAction(LookAction, ETriggerEvent::Canceled, this, &ThisClass::Input_OnLook);
		EnhancedInput->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ThisClass::Input_OnMove);
		EnhancedInput->BindAction(MoveAction, ETriggerEvent::Canceled, this, &ThisClass::Input_OnMove);
		EnhancedInput->BindAction(ToggleRunWalkAction, ETriggerEvent::Triggered, this, &ThisClass::Input_OnToggleRunWalk);
		EnhancedInput->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ThisClass::Input_OnJump);
		EnhancedInput->BindAction(JumpAction, ETriggerEvent::Canceled, this, &ThisClass::Input_OnJump);
	}
}

void ASignetPlayerCharacter::CalcCamera(float DeltaTime, struct FMinimalViewInfo& OutResult)
{
	if (Camera->IsActive())
	{
		Camera->GetViewInfo(OutResult);
		return;
	}
	
	Super::CalcCamera(DeltaTime, OutResult);
}

void ASignetPlayerCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}

void ASignetPlayerCharacter::Input_OnLookMouse(const FInputActionValue& ActionValue)
{
	const auto Value = ActionValue.Get<FVector2D>();
	AddControllerPitchInput(Value.Y * LookUpMouseSensitivity);
	AddControllerYawInput(Value.X * LookRightMouseSensitivity);
}

void ASignetPlayerCharacter::Input_OnLook(const FInputActionValue& ActionValue)
{
	const auto Value = ActionValue.Get<FVector2D>();
	AddControllerPitchInput(Value.Y * LookUpRate);
	AddControllerYawInput(Value.X * LookRightRate);
}

void ASignetPlayerCharacter::Input_OnMove(const FInputActionValue& ActionValue)
{
	const auto Value = UAlsVector::ClampMagnitude012D(ActionValue.Get<FVector2D>());
	auto ViewRotation = GetViewState().Rotation;

	if (IsValid(GetController()))
	{
		FVector ViewLocation;
		GetController()->GetPlayerViewPoint(ViewLocation, ViewRotation);
	}

	const auto ForwardDirection = UAlsVector::AngleToDirectionXY(UE_REAL_TO_FLOAT(ViewRotation.Yaw));
	const auto RightDirection = UAlsVector::PerpendicularCounterClockwiseXY(ForwardDirection);

	AddMovementInput(ForwardDirection * Value.Y + RightDirection * Value.X);
}

void ASignetPlayerCharacter::Input_OnToggleRunWalk(const FInputActionValue& ActionValue)
{
	SetDesiredGait(
		GetDesiredGait() == AlsGaitTags::Walking ?
		AlsGaitTags::Running :
		AlsGaitTags::Walking
	);
}

void ASignetPlayerCharacter::Input_OnJump(const FInputActionValue& ActionValue)
{
	if (ActionValue.Get<bool>())
	{
		Jump();
	}
	else
	{
		StopJumping();
	}
}

void ASignetPlayerCharacter::Input_OnSit(const FInputActionValue& ActionValue)
{
}

void ASignetPlayerCharacter::Input_OnHeal(const FInputActionValue& ActionValue)
{
}

void ASignetPlayerCharacter::Input_OnEmotePressed(const FInputActionValue& ActionValue)
{
}

void ASignetPlayerCharacter::Input_OnEmoteReleased(const FInputActionValue& ActionValue)
{
}

void ASignetPlayerCharacter::Input_OnAccept(const FInputActionValue& ActionValue)
{
}

void ASignetPlayerCharacter::Input_OnCancel(const FInputActionValue& ActionValue)
{
}

void ASignetPlayerCharacter::Input_OnMenu(const FInputActionValue& ActionValue)
{
}

void ASignetPlayerCharacter::OnRep_VisualState()
{
	VisualStateUpdated.Broadcast(VisualState);
}

void ASignetPlayerCharacter::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ASignetPlayerCharacter, VisualState);
}

void ASignetPlayerCharacter::DisplayDebug(UCanvas* Canvas, const FDebugDisplayInfo& DisplayInfo, float& Unused,
                                          float& VerticalLocation)
{
	if (Camera->IsActive())
	{
		Camera->DisplayDebug(Canvas, DisplayInfo, VerticalLocation);
	}
	
	Super::DisplayDebug(Canvas, DisplayInfo, Unused, VerticalLocation);
}

