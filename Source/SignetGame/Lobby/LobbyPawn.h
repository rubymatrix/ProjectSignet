// Copyright Red Lotus Games, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "ModularGameplayActors/GSCModularCharacter.h"
#include "LobbyPawn.generated.h"

UCLASS(ClassGroup=(Signet))
class SIGNETGAME_API ALobbyPawn : public AGSCModularCharacter
{
	GENERATED_BODY()

public:

	ALobbyPawn();

protected:
	
	virtual void BeginPlay() override;
	
};
