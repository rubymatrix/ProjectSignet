// Copyright Red Lotus Games, All Rights Reserved.


#include "CharacterDataComponent.h"

#include "AssetTypeCategories.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "SignetGame/Abilities/TagCache.h"
#include "SignetGame/Data/CharacterParts.h"
#include "SignetGame/Data/GameDataSubsystem.h"
#include "SignetGame/Inventory/SignetInventoryComponent.h"
#include "SignetGame/Player/SignetPlayerCharacter.h"


UCharacterDataComponent::UCharacterDataComponent()
{
}

void UCharacterDataComponent::BeginPlay()
{
	Super::BeginPlay();

	// Capture reference to owner
	if (const auto P = Cast<ASignetPlayerCharacter>(GetOwner()))
	{
		Character = P;
	}
}

UAnimMontage* UCharacterDataComponent::GetActionMontage(const EMontageType MontageType)
{
	if (IsWeaponAction(MontageType))
	{
		if (Character.IsValid())
		{
			if (const auto Inventory = Character->GetInventoryComponent())
			{
				const FGameplayTag WeaponSkill = Inventory->GetEquippedWeaponSkill();
				if (const auto GameData = GetGameData())
				{
					if (const auto WeaponData = GameData->GetWeaponTypeRow(Character->GetCurrentRace(), WeaponSkill))
					{
						if (const auto Result = WeaponData->ActionMontages.Find(MontageType))
						{
							return *Result;
						}
					}
				}
			}
		}
	}

	if (const auto Parts = GetParts())
	{
		if (const auto Result = Parts->Actions.ActionMontages.Find(MontageType))
		{
			return *Result;
		}
	}
	
	return nullptr;
}

TArray<UAnimMontage*> UCharacterDataComponent::GetAttackRoundMontages(const EMontageType AttackType,
	const int AttackCount)
{
	TArray<UAnimMontage*> Result{};
	TArray<UAnimMontage*> AttackAnims;

	if (Character.IsValid())
	{
		if (const auto Inventory = Character->GetInventoryComponent())
		{
			const FGameplayTag WeaponSkill = Inventory->GetEquippedWeaponSkill();
			if (const auto GameData = GetGameData())
			{
				if (const auto WeaponData = GameData->GetWeaponTypeRow(Character->GetCurrentRace(), WeaponSkill))
				{
					switch (AttackType)
					{
					case EMontageType::KickAttack: AttackAnims = WeaponData->KickAttackMontages; break;
					case EMontageType::SubAttack:  AttackAnims = WeaponData->SubAttackMontages; break;
					case EMontageType::Attack:
					default:					   AttackAnims = WeaponData->MainAttackMontages; break;
					}
				}
			}
		}
	}

	if (AttackAnims.Num() <= 0)
	{
		if (const auto Parts = GetParts())
		{
			// Get the appropriate attack type
			switch (AttackType)
			{
			case EMontageType::KickAttack: AttackAnims = Parts->Actions.KickAttackMontages; break;
			case EMontageType::SubAttack:  AttackAnims = Parts->Actions.SubAttackMontages; break;
			case EMontageType::Attack:
			default:					   AttackAnims = Parts->Actions.AttackMontages; break;
			}
		}
	}

	if (AttackAnims.Num() > 0)
	{
		int AttackIndex = 0;
		while (Result.Num() < AttackCount)
		{
			// Add the current index to the result
			Result.Add(AttackAnims[AttackIndex]);

			AttackIndex += 1;
			if (AttackIndex >= AttackAnims.Num())
			{
				AttackIndex = 0; // Loop back to first
			}
		}
	}

	return Result;
}

UAnimMontage* UCharacterDataComponent::GetMovingAttackMontage(const EActionDirection Direction)
{
	if (Character.IsValid())
	{
		if (const auto Inventory = Character->GetInventoryComponent())
		{
			const FGameplayTag WeaponSkill = Inventory->GetEquippedWeaponSkill();
			if (const auto GameData = GetGameData())
			{
				if (const auto WeaponData = GameData->GetWeaponTypeRow(Character->GetCurrentRace(), WeaponSkill))
				{
					if (const auto Montage = WeaponData->DirectionalAttackMontages.Find(Direction))
					{
						return *Montage;
					}
				}
			}
		}
	}

	return nullptr;
}

USoundBase* UCharacterDataComponent::GetVocalization(const EVocalizationType VocalType)
{
	const auto VoiceBank = GetVoiceBank();
	if (VoiceBank.HasVocal(VocalType))
	{
		if (const auto Voice = VoiceBank.Vocals.Find(VocalType))
		{
			if (const auto& Sounds = Voice->Sounds; Sounds.Num() > 0)
			{
				const auto Index = FMath::RandRange(0, Sounds.Num() - 1);
				return Sounds[Index];
			}
		}
	}
	return nullptr;
}

USoundBase* UCharacterDataComponent::GetFootstepSound()
{
	auto GameData = GetGameData();
	if (!GameData) return nullptr;

	EPhysicalSurface SurfaceType = SurfaceType_Default;
	EArmorSoundClass ArmorClass = EArmorSoundClass::Cloth;

	if (Character.IsValid())
	{
		// Surface Detection
		FVector Start = Character->GetActorLocation();
		FVector End = Start + (FVector::DownVector * 150.0f);
		FHitResult HitResult;
		FCollisionQueryParams QueryParams;
		QueryParams.AddIgnoredActor(Character.Get());
		QueryParams.bReturnPhysicalMaterial = true;

		if (GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility, QueryParams))
		{
			SurfaceType = UPhysicalMaterial::DetermineSurfaceType(HitResult.PhysMaterial.Get());
		}

		// Armor Detection
		if (const auto Inventory = Character->GetInventoryComponent())
		{
			const int32 FeetItemID = Inventory->GetEquippedItemID(EGearSlot::Feet);
			if (FeetItemID > 0)
			{
				if (const FInventoryItem* ItemDef = GameData->GetItem(FeetItemID))
				{
					ArmorClass = ItemDef->ArmorSoundClass;
				}
			}
		}
	}
	
	return GameData->GetFootstep(SurfaceType, ArmorClass);
}

USoundBase* UCharacterDataComponent::GetWeaponWhooshSound(EPhysicalAttackDirection Direction)
{
	const auto GameData = GetGameData();
	if (!GameData || !Character.IsValid()) return nullptr;

	if (const auto Inventory = Character->GetInventoryComponent())
	{
		const EGearSlot Slot = (Direction == EPhysicalAttackDirection::Right) ? EGearSlot::Main : EGearSlot::Sub;
		const int32 ItemID = Inventory->GetEquippedItemID(Slot);
		if (ItemID > 0)
		{
			if (const FInventoryItem* ItemDef = GameData->GetItem(ItemID))
			{
				if (const auto WeaponSoundBank = GameData->GetWeaponSound(ItemDef->WeaponSkill))
				{
					if (const auto Sound = WeaponSoundBank->WhooshSounds.WhooshSoundMap.Find(ItemDef->WeaponSoundType))
					{
						return *Sound;
					}
				}
			}
		}
	}

	return nullptr;
}

USoundBase* UCharacterDataComponent::GetWeaponImpactSound(const FAttackResult& Result)
{
	UE_LOG(LogTemp, Error, TEXT("GetWeaponImpactSound - WASSUP"));
	
	const auto GameData = GetGameData();
	if (!GameData || !Character.IsValid()) return nullptr;

	if (Result.bIsCritical)
	{
		return GameData->GetCriticalImpactSound();
	}

	if (const auto Inventory = Character->GetInventoryComponent())
	{
		const EGearSlot Slot = (Result.AttackDirection == EPhysicalAttackDirection::Right) ? EGearSlot::Main : EGearSlot::Sub;
		const int32 ItemID = Inventory->GetEquippedItemID(Slot);
		UE_LOG(LogTemp, Log, TEXT("GetWeaponImpactSound: Item ID: %d"), ItemID);

		if (ItemID == 0)
		{
			if (const auto WeaponSoundBank = GameData->GetWeaponSound(FTagCache::Get().Skill.None))
			{
				if (const auto SoundPtr = WeaponSoundBank->ImpactSounds.ImpactSoundMap.Find(EWeaponSoundSlot::Slot1))
				{
					return *SoundPtr;
				}
			}
		}
		
		if (const FInventoryItem* ItemDef = GameData->GetItem(ItemID))
		{
			UE_LOG(LogTemp, Log, TEXT("GetWeaponImpactSound: Found Item Definition -> %s"), *ItemDef->En);
			if (const auto WeaponSoundBank = GameData->GetWeaponSound(ItemDef->WeaponSkill))
			{
				UE_LOG(LogTemp, Error, TEXT("GetWeaponImpactSound: Found Weapon Sound Bank!"));

				if (const auto SoundPtr = WeaponSoundBank->ImpactSounds.ImpactSoundMap.Find(ItemDef->WeaponSoundType))
				{
					UE_LOG(LogTemp, Log, TEXT("GetWeaponImpactSound: Found impact sound"));
					return *SoundPtr;
				}
				
				UE_LOG(LogTemp, Error, TEXT("GetWeaponImpactSound: Failed to find sound. Sound Map Count (%d)"), WeaponSoundBank->ImpactSounds.ImpactSoundMap.Num());
			}
			else
			{
				UE_LOG(LogTemp, Error, TEXT("GetWeaponImpactSound: Unable to find soundbank for: %s"), *ItemDef->WeaponSkill.ToString());
			}
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Unable to find item definition for ItemId: %d"), ItemID);
		}
	}

	return nullptr;
}

UGameDataSubsystem* UCharacterDataComponent::GetGameData()
{
	const auto World = GetWorld();
	if (!World) return nullptr;

	const auto GameInstance = World->GetGameInstance();
	if (!GameInstance) return nullptr;

	return GameInstance->GetSubsystem<UGameDataSubsystem>();
}

const FCharacterPartsRow* UCharacterDataComponent::GetParts()
{
	auto GameData = GetGameData();
	if (!GameData) return nullptr;
	if (!Character.IsValid()) return nullptr;

	return GameData->GetCharacterPartsRow(Character->GetCurrentRace());
}

int UCharacterDataComponent::GetFaceVoiceIndex()
{
	if (!Character.IsValid()) return 0;

	switch (const auto FaceType = Character->GetCurrentFace())
	{
	case EFace::F1A:
	case EFace::F1B: return 0;
	case EFace::F2A:
	case EFace::F2B: return 1;
	case EFace::F3A:
	case EFace::F3B: return 2;
	case EFace::F4A:
	case EFace::F4B: return 3;
	case EFace::F5A:
	case EFace::F5B: return 4;
	case EFace::F6A:
	case EFace::F6B: return 5;
	case EFace::F7A:
	case EFace::F7B: return 6;
	case EFace::F8A:
	case EFace::F8B: return 7;
	default: return 0;
	}
}

FVoiceBank UCharacterDataComponent::GetVoiceBank()
{
	const auto FaceIndex = GetFaceVoiceIndex();
	
	if (const auto Parts = GetParts())
	{
		if (Parts->Voices.Num() > FaceIndex)
		{
			return Parts->Voices[FaceIndex];
		}

		if (Parts->Voices.Num() > 0)
		{
			return Parts->Voices[0];
		}
	}

	return {};
}

bool UCharacterDataComponent::IsWeaponAction(const EMontageType ActionType)
{
	switch (ActionType)
	{
	case EMontageType::Attack:
	case EMontageType::Disengage:
	case EMontageType::Engage:
	case EMontageType::KickAttack:
	case EMontageType::Die: return true;
	default: return false;
	}
}

