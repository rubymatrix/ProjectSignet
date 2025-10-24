// Copyright Red Lotus Games, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "PlayerListContent.h"
#include "SGPlayerListWidget.h"
#include "Blueprint/UserWidget.h"
#include "SGPlayerListItemWidget.generated.h"

class UTextBlock;

/**
 * 
 */
UCLASS()
class SIGNETGAME_API USGPlayerListItemWidget : public UUserWidget
{
	GENERATED_BODY()

protected:

	UPROPERTY(VisibleAnywhere)   int32 PlayerId = -1;
	UPROPERTY(meta=(BindWidget)) TObjectPtr<UTextBlock> PlayerNameText;
	UPROPERTY(meta=(BindWidget)) TObjectPtr<UTextBlock> JobAbbrText;
	UPROPERTY(meta=(BindWidget)) TObjectPtr<UTextBlock> LevelText;
	UPROPERTY(meta=(BindWidget)) TObjectPtr<UTextBlock> PingText;
	
public:

	void SetPlayer(const FPlayerListContent* InPlayer);
	void SetPing(int32 InPing, bool bIsHost = false);

	int32 GetPlayerId() const;
	
};
