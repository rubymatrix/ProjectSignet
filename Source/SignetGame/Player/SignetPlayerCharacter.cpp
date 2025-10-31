// Copyright Red Lotus Games, All Rights Reserved.

#include "SignetPlayerCharacter.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "SignetPlayerState.h"
#include "AnimNodes/AnimNode_RandomPlayer.h"
#include "Components/SignetCameraComponent.h"
#include "Components/StatsComponent.h"
#include "Components/TargetingComponent.h"
#include "Engine/AssetManager.h"
#include "Utility/AlsVector.h"
#include "Net/UnrealNetwork.h"
#include "SignetGame/Abilities/SignetAbilitySystemComponent.h"
#include "SignetGame/Abilities/TagCache.h"
#include "SignetGame/Abilities/Attributes/SignetPrimaryAttributeSet.h"
#include "SignetGame/Inventory/SignetInventoryComponent.h"
#include "SignetGame/Util/Logging.h"
#include "SignetGame/Util/Stats.h"
#include "SignetGame/Data/GameDataSubsystem.h"


static TArray VisualSlots = { EGearSlot::Main, EGearSlot::Sub, EGearSlot::Head, EGearSlot::Body, EGearSlot::Hands, EGearSlot::Legs, EGearSlot::Feet };
static TArray DefaultSlots = { EGearSlot::Body, EGearSlot::Hands, EGearSlot::Legs, EGearSlot::Feet };

bool ASignetPlayerCharacter::IsAscValid() const
{
	return IsValid(GetSignetAsc());
}

USignetAbilitySystemComponent* ASignetPlayerCharacter::GetSignetAsc() const
{
	return Cast<USignetAbilitySystemComponent>(GetAbilitySystemComponent());
}

USkeletalMeshComponent* ASignetPlayerCharacter::GetFaceComponent()
{
	return FaceMesh;
}

USkeletalMeshComponent* ASignetPlayerCharacter::GetMeshComponent(const EGearSlot& GearSlot)
{
	switch (GearSlot)
	{
	case EGearSlot::Main: return MainMesh;
	case EGearSlot::Sub: return SubMesh;
	case EGearSlot::Ranged: return RangedMesh;	
	case EGearSlot::Head: return HeadMesh;
	case EGearSlot::Body: return BodyMesh;
	case EGearSlot::Hands: return HandsMesh;
	case EGearSlot::Legs: return LegsMesh;
	case EGearSlot::Feet: return FeetMesh;
	case EGearSlot::None: 
	default:
		return nullptr;
	}
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
		MeshComp->SetLeaderPoseComponent(GetMesh());
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
	InventoryComponent = CreateDefaultSubobject<USignetInventoryComponent>(TEXT("InventoryComp"));
	
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

	// Listen for equipment changes
	if (InventoryComponent)
	{
		InventoryComponent->OnEquipmentChanged.AddDynamic(this, &ASignetPlayerCharacter::OnEquipmentChanged);
	}
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
	RefreshAllSlots();
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

static TMap<EGearSlot, EBodyPart> GearSlotHidePartsCache = {
	{ EGearSlot::Head, EBodyPart::Head },
	{ EGearSlot::Body, EBodyPart::Body },
	{ EGearSlot::Hands, EBodyPart::Hands },
	{ EGearSlot::Legs, EBodyPart::Legs },
	{ EGearSlot::Feet, EBodyPart::Feet }
};

TArray<USkeletalMeshComponent*> ASignetPlayerCharacter::GetComponentsToHide()
{
	TArray<USkeletalMeshComponent*> Components;

	for (auto Pair : GearSlotHidePartsCache)
	{
		if ((CurrentHideParts & static_cast<int32>(Pair.Value)) != 0)
		{
			Components.Add(GetMeshComponent(Pair.Key));
		}
	}
	
	return Components;
}

UGameDataSubsystem* ASignetPlayerCharacter::GetGameData()
{
	const auto World = GetWorld();
	if (!World) return nullptr;
	const auto GI = World->GetGameInstance();
	if (!GI) return nullptr;
	return GI->GetSubsystem<UGameDataSubsystem>();
}

const FInventoryItem* ASignetPlayerCharacter::FindItemDef(const int ItemID)
{
	if (ItemID == 0) return nullptr;
	if (!GetGameData()) return nullptr;
	return GetGameData()->GetItem(ItemID);
}

const FCharacterPartsRow* ASignetPlayerCharacter::GetCharacterParts()
{
	if (!GetGameData()) return nullptr;
	return GetGameData()->GetCharacterPartsRow(CurrentRace);
}

static USkeletalMesh* GetDefaultMeshPart(const FCharacterPartsRow* Parts, const EGearSlot GearSlot)
{
	if (!Parts) return nullptr;

	if (const auto NewMesh = Parts->SlotDefaults.Find(GearSlot))
	{
		return *NewMesh;
	}
	
	return nullptr;
}

struct FCharacterFace
{
	USkeletalMesh* Mesh;
	UMaterialInstance* Material;
};

static FCharacterFace GetFaceMesh(const FCharacterPartsRow* Parts, const EFace Face, const EFaceClipStage DesiredClipStage)
{
	if (!Parts) return {};

	if (const auto FoundFace = Parts->Faces.Find(Face))
	{
		FCharacterFace NewFace{};
		NewFace.Material = FoundFace->Material;
		
		switch (DesiredClipStage)
		{
		case EFaceClipStage::RemoveTop: NewFace.Mesh = FoundFace->ClipStage1; break;
		case EFaceClipStage::RemoveAll: NewFace.Mesh = FoundFace->ClipStage2; break;
		case EFaceClipStage::None:
		default: NewFace.Mesh = FoundFace->Mesh;
		}

		return NewFace;
	}

	return {};
}

void ASignetPlayerCharacter::ApplyBaseCharacter()
{
	const auto GameData = GetGameData();
	const auto MeshComp = GetMesh();
	const auto PS = GetPlayerState<ASignetPlayerState>();

	if (!GameData || !MeshComp || !PS) return;

	CurrentRace = PS->Race;
	CurrentFace = PS->Face;

	if (const auto Parts = GetCharacterParts())
	{
		if (const auto NewMesh = Parts->RootMesh)
		{
			GetMesh()->SetSkeletalMesh(NewMesh);
			GetMesh()->SetAnimInstanceClass(Parts->AnimInstanceClass);
		}
		else
		{
			UE_LOG(LogSignetGameData, Error, TEXT("ApplyBaseCharacter: RootMesh for [%s] is NULL"), *UEnum::GetValueAsString(CurrentRace));
		}
	}
	else
	{
		UE_LOG(LogSignetGameData, Error, TEXT("ApplyBaseCharacter: Parts for [%s] is NULL"), *UEnum::GetValueAsString(CurrentRace));
	}
}

void ASignetPlayerCharacter::RefreshAllSlots()
{
	const auto World = GetWorld();

	if (!World->GetTimerManager().IsTimerActive(VisualUpdateDebounceTimer))
	{
		World->GetTimerManager().SetTimer(VisualUpdateDebounceTimer, [this]()
		{
			ApplyBaseCharacter();
	
			for (const auto& S : VisualSlots)
			{
				ApplyEquipmentSlot(S);
			}

			ApplyMeshHiding();		
		}, 0.15f, false);
	}
}

static TMap<ERace, FString> RaceTokens{
	{ ERace::HumeMale, TEXT("Hume/Male/")},
	{ ERace::HumeFemale, TEXT("Hume/Female/") },
	{ ERace::ElvaanMale, TEXT("Elvaan/Male/") },
	{ ERace::ElvaanFemale, TEXT("Elvaan/Female/") },
	{ ERace::Galka, TEXT("Galka/") },
	{ ERace::Mithra, TEXT("Mithra/") },
	{ ERace::TarutaruMale, TEXT("Tarutaru/") },
	{ ERace::TarutaruFemale, TEXT("Tarutaru/") }
};

static FString GetRacePathToken(const ERace InRace)
{
	if (const auto RaceToken = RaceTokens.Find(InRace))
	{
		return FString::Printf(TEXT("/Game/Characters/Modular/%s"), **RaceToken);
	}

	return TEXT("");
}
static USkeletalMesh* LoadMeshFromPathToken(const ERace InRace, const FString& Path)
{
	USkeletalMesh* MeshAsset = nullptr;
	
	const auto RacePath = GetRacePathToken(InRace);
	if (RacePath.IsEmpty()) return MeshAsset;

	const auto FullPathString = FString::Printf(TEXT("%s%s"), *RacePath, *Path);
	if (FullPathString.IsEmpty()) return MeshAsset;

	
	FSoftObjectPath AssetPath(FullPathString);
	FStreamableManager& Streamable = UAssetManager::GetStreamableManager();

	if (const TSoftObjectPtr<USkeletalMesh> SoftObject = TSoftObjectPtr<USkeletalMesh>(AssetPath); SoftObject.IsValid())
	{
		MeshAsset = SoftObject.Get();
	}
	else
	{
		MeshAsset = Cast<USkeletalMesh>(Streamable.LoadSynchronous(AssetPath));
	}
	
	return MeshAsset;
}

void ASignetPlayerCharacter::ApplyEquipmentSlot(const EGearSlot InGearSlot)
{
	
	const auto MeshComp = GetMeshComponent(InGearSlot);
	if (!MeshComp)
	{
		UE_LOG(LogSignet, Error, TEXT("Unable to find mesh component"));
		return;
	}

	// Get the base character parts info
	const auto Parts = GetCharacterParts();
	if (!Parts) return;
	
	// Get Equipped Item Info
	auto bMeshApplied = false;
	const auto bIsHeadSlot = InGearSlot == EGearSlot::Head;
	const auto EquippedItemID = InventoryComponent->GetEquippedItemID(InGearSlot);
	EFaceClipStage DesiredClipStage = EFaceClipStage::None;
	
	
	if (const FInventoryItem* ItemDef = EquippedItemID != 0 ? FindItemDef(EquippedItemID) : nullptr)
	{
		// Capture desired face clip
		DesiredClipStage = ItemDef->FaceClippingStage;

		// Get the sub path that we need to load for the current race
		if (const auto MeshPath = ItemDef->MeshPath; !MeshPath.IsEmpty())
		{
			if (const auto NewMesh = LoadMeshFromPathToken(CurrentRace, MeshPath))
			{
				MeshComp->SetSkeletalMesh(NewMesh);
				MeshComp->SetVisibility(true);
				bMeshApplied = true;
			}
		}
	}

	if (!bMeshApplied) // Try to apply a fallback mesh
	{
		ApplyDefaultMesh(InGearSlot, Parts);
	}

	if (bIsHeadSlot) // If it's a head slot, make sure we set the appropriate face
	{
		ApplyFace(Parts, DesiredClipStage);
	}
	
}

void ASignetPlayerCharacter::UpdateRace(const ERace NewRace)
{
	CurrentRace = NewRace;
	RefreshAllSlots();
}

void ASignetPlayerCharacter::UpdateFace(const EFace NewFace)
{
	CurrentFace = NewFace;
	ApplyEquipmentSlot(EGearSlot::Head);
}

void ASignetPlayerCharacter::CaptureVisualState(const ERace NewRace, const EFace NewFace)
{
	const auto Inv = InventoryComponent;
	if (!Inv) return;
	
	VisualState = {
		.Race = NewRace,
		.Face = NewFace,
		.MainID = Inv->GetEquippedItemID(EGearSlot::Main),
		.SubID = Inv->GetEquippedItemID(EGearSlot::Sub),
		.HeadID = Inv->GetEquippedItemID(EGearSlot::Head),
		.BodyID = Inv->GetEquippedItemID(EGearSlot::Body),
		.HandsID = Inv->GetEquippedItemID(EGearSlot::Hands),
		.LegsID = Inv->GetEquippedItemID(EGearSlot::Legs),
		.FeetID = Inv->GetEquippedItemID(EGearSlot::Feet),
	};

	if (HasAuthority())
	{
		RefreshAllSlots();
	}
}

void ASignetPlayerCharacter::ApplyDefaultMesh(const EGearSlot InGearSlot, const FCharacterPartsRow* Parts)
{
	if (!Parts) return;
	const auto MeshComp = GetMeshComponent(InGearSlot);
	if (!MeshComp) return;
	
	if (const auto NewMesh = Parts->SlotDefaults.Find(InGearSlot))
	{
		// Default piece found, need to apply it
		MeshComp->SetSkeletalMesh(*NewMesh);
		MeshComp->SetVisibility(true);	
	}
	else
	{
		// Remove & Hide the mesh part
		MeshComp->SetSkeletalMesh(nullptr);
		MeshComp->SetVisibility(false);
	}
}

void ASignetPlayerCharacter::ApplyFace(const FCharacterPartsRow* Parts, const EFaceClipStage DesiredClipStage)
{
	if (!Parts) return;
	const auto NewFace = GetFaceMesh(Parts, CurrentFace, DesiredClipStage);

	if (NewFace.Mesh)
	{
		FaceMesh->SetSkeletalMesh(NewFace.Mesh);
	}

	if (NewFace.Material)
	{
		FaceMesh->SetMaterial(0, NewFace.Material);
	}
}

void ASignetPlayerCharacter::ApplyMeshHiding()
{
	const auto GameData = GetGameData();
	if (!GameData) return;
	if (!InventoryComponent) return;

	int32 NewHideParts = 0;
	for (const auto GearSlot : {EGearSlot::Head, EGearSlot::Body, EGearSlot::Hands, EGearSlot::Legs, EGearSlot::Feet})
	{
		if (const auto MeshComp = GetMeshComponent(GearSlot)) // Need to reset visibility here, otherwise things will stay hidden unexpectedly
		{
			if (MeshComp->GetSkeletalMeshAsset() != nullptr)
			{
				MeshComp->SetVisibility(true);
			}
		}
		
		const auto EquippedItemID = InventoryComponent->GetEquippedItemID(GearSlot);
		if (EquippedItemID == 0) continue;

		if (const auto ItemDef = GameData->GetItem(EquippedItemID))
		{
			NewHideParts |= ItemDef->HiddenPartsMask;
		}
	}

	CurrentHideParts = NewHideParts;
	
	for (const auto MeshComp : GetComponentsToHide())
	{
		MeshComp->SetVisibility(false);
	}
}

void ASignetPlayerCharacter::OnEquipmentChanged(EGearSlot InGearSlot, int32 OldItemID, int32 NewItemID)
{
	ApplyEquipmentSlot(InGearSlot);
	ApplyMeshHiding();
}

