// Copyright Red Lotus Games, All Rights Reserved.


#include "CharacterDataComponent.h"
#include "SignetGame/Data/CharacterParts.h"
#include "SignetGame/Data/GameDataSubsystem.h"
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

	if (const auto Parts = GetParts())
	{
		int AttackIndex = 0;

		// Get the appropriate attack type
		TArray<UAnimMontage*> AttackAnims;
		switch (AttackType)
		{
		case EMontageType::KickAttack: AttackAnims = Parts->Actions.KickAttackMontages; break;
		case EMontageType::SubAttack:  AttackAnims = Parts->Actions.SubAttackMontages; break;
		case EMontageType::Attack:
		default:					   AttackAnims = Parts->Actions.AttackMontages; break;
		}
		
		if (AttackAnims.Num() <= 0) return Result; // Don't have any attack anims
		
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

USoundBase* UCharacterDataComponent::GetVocalization(const EVocalizationType VocalType)
{
	return nullptr;
}

USoundBase* UCharacterDataComponent::GetFootstepSound()
{
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

