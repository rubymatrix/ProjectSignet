// Copyright Red Lotus Games, All Rights Reserved.

#include "SignetGameInstance.h"
#include "SignetGame/Net/SignetNetSubsystem.h"

void USignetGameInstance::Init()
{
	Super::Init();

	if (USignetNetSubsystem* Net = GetSubsystem<USignetNetSubsystem>())
	{
		Net->HostLocalLobby(7777);
	}
}