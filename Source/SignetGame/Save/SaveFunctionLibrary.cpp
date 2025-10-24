// Copyright Red Lotus Games, All Rights Reserved.


#include "SaveFunctionLibrary.h"
#include "SignetSaveSubsystem.h"
#include "SignetSaveGame.h"
#include "SignetSaveTypes.h"
#include "Kismet/GameplayStatics.h"
#include "SignetGame/Util/Logging.h"

USignetSaveGame* USaveFunctionLibrary::GetPlayerData(UWorld* WorldContext)
{
	if (WorldContext == nullptr)
	{
		UE_LOG(LogSignet, Warning, TEXT("GetPlayerData: WorldContext is null"));
	}

	if (const auto Save = WorldContext->GetGameInstance()->GetSubsystem<USignetSaveSubsystem>())
	{
		return Save->GetSave();
	}

	UE_LOG(LogSignet, Warning, TEXT("GetPlayerData: USignetSaveSubsystem not found"));
	
	return nullptr;
}

void USaveFunctionLibrary::SetPlayerName(UWorld* WorldContext, const FString& NewName)
{
	if (WorldContext == nullptr)
	{
		UE_LOG(LogSignet, Warning, TEXT("SetPlayerName: WorldContext is null"));
		return;
	}

	if (const auto Save = WorldContext->GetGameInstance()->GetSubsystem<USignetSaveSubsystem>())
	{
		Save->SetPlayerName(NewName);
		return;
	}

	UE_LOG(LogSignet, Warning, TEXT("SetPlayerName: USignetSaveSubsystem not found"));
}

int USaveFunctionLibrary::AddCurrency(UWorld* WorldContext, const ECurrencyType& CurrencyType, int Delta)
{
	if (WorldContext == nullptr)
	{
		UE_LOG(LogSignet, Warning, TEXT("AddCurrency: WorldContext is null"));
		return 0;
	}

	if (const auto Save = WorldContext->GetGameInstance()->GetSubsystem<USignetSaveSubsystem>())
	{
		Save->AddCurrency(CurrencyType, Delta, true);
		return Save->GetWallet().GetRef(CurrencyType);
	}
	
	UE_LOG(LogSignet, Warning, TEXT("AddCurrency: USignetSaveSubsystem not found"));
	return 0;
}

void USaveFunctionLibrary::SetJobLevel(UWorld* WorldContext, const EJob& JobType, int NewLevel)
{
	if (WorldContext == nullptr)
	{
		UE_LOG(LogSignet, Warning, TEXT("SetJobLevel: WorldContext is null"));
		return;
	}

	if (const auto Save = WorldContext->GetGameInstance()->GetSubsystem<USignetSaveSubsystem>())
	{
		FSignetJobProgress* Job = Save->GetOrAddJob(JobType);
		Job->Level = FMath::Max(1, NewLevel);
		Save->SaveNow();
		return;
	}

	UE_LOG(LogSignet, Warning, TEXT("SetJobLevel: USignetSaveSubsystem not found"));
}
