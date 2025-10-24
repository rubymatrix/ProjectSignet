// Copyright Red Lotus Games, All Rights Reserved.

#pragma once
#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "SignetNetSubsystem.generated.h"

UCLASS(ClassGroup=(Signet))
class SIGNETGAME_API USignetNetSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
public:
	
	UFUNCTION(BlueprintCallable)
	void HostLocalLobby(int32 Port = 7777);
	
	UFUNCTION(BlueprintCallable)
	void ConnectToHost(const FString& Address); // "IP[:Port]" e.g., "127.0.0.1:7777"

private:
	
	bool IsAlreadyHosting() const;
	
};