// Copyright Red Lotus Games, All Rights Reserved.

#include "SignetPlayerCharacter.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Components/SignetCameraComponent.h"
#include "Utility/AlsVector.h"


ASignetPlayerCharacter::ASignetPlayerCharacter()
{
	// Initialize the camera system
	Camera = CreateDefaultSubobject<USignetCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(GetMesh());
	Camera->SetRelativeLocation_Direct({0.f, 90.f, 0.f});
	
	auto CreateMeshObject = [&](TObjectPtr<USkeletalMeshComponent> MeshComp, FName InName, FName Tag)
	{
		MeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(InName);
		MeshComp->SetupAttachment(GetMesh());
		MeshComp->SetVisibility(false);
		MeshComp->SetComponentTickEnabled(false);
		MeshComp->ComponentTags.Add(Tag);
	};

	// Initialize all module mesh parts
	CreateMeshObject(MainMesh, TEXT("MainMesh"), TEXT("Main"));
	CreateMeshObject(SubMesh, TEXT("SubMesh"), TEXT("Sub"));
	CreateMeshObject(RangedMesh, TEXT("RangedMesh"), TEXT("Ranged"));
	CreateMeshObject(FaceMesh, TEXT("FaceMesh"), TEXT("Face"));
	CreateMeshObject(HeadMesh, TEXT("HeadMesh"), TEXT("Head"));
	CreateMeshObject(BodyMesh, TEXT("BodyMesh"), TEXT("Body"));
	CreateMeshObject(HandsMesh, TEXT("HandsMesh"), TEXT("Hands"));
	CreateMeshObject(LegsMesh, TEXT("LegsMesh"), TEXT("Legs"));
	CreateMeshObject(FeetMesh, TEXT("FeetMesh"), TEXT("Feet"));
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

void ASignetPlayerCharacter::DisplayDebug(UCanvas* Canvas, const FDebugDisplayInfo& DisplayInfo, float& Unused,
	float& VerticalLocation)
{
	if (Camera->IsActive())
	{
		Camera->DisplayDebug(Canvas, DisplayInfo, VerticalLocation);
	}
	
	Super::DisplayDebug(Canvas, DisplayInfo, Unused, VerticalLocation);
}

