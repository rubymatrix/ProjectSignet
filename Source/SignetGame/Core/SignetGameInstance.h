// Copyright Red Lotus Games, All Rights Reserved.

#pragma once
#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "SignetGameInstance.generated.h"

UCLASS(ClassGroup=(Signet))
class SIGNETGAME_API USignetGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	
	virtual void Init() override;
	
};