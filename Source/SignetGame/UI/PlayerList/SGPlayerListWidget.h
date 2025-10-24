// Copyright Red Lotus Games, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "PlayerListContent.h"
#include "Blueprint/UserWidget.h"
#include "SGPlayerListWidget.generated.h"

enum class EJob : uint8;
class ASignetPlayerState;
class USGPlayerListItemWidget;


class UStackBox;

/**
 * 
 */
UCLASS()
class SIGNETGAME_API USGPlayerListWidget : public UUserWidget
{
	GENERATED_BODY()

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TArray<FPlayerListContent> PlayerList;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<USGPlayerListItemWidget> ListItemClass;
	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UStackBox> Players;

	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

public:

	UFUNCTION(BlueprintCallable)
	void UpdatePlayer(ASignetPlayerState* Player);


private:

	void RebuildList();
	bool PlayerExists(ASignetPlayerState* Player);
	UFUNCTION() void ReceivedPlayerUpdate(ASignetPlayerState* Player);
	UFUNCTION() void PlayerJoined(ASignetPlayerState* Player);
	UFUNCTION() void PlayerLeft(ASignetPlayerState* Player);
	FText GetJobAbbr(const EJob& JobType);
	FPlayerListContent* GetContent(int32 PlayerId);
	USGPlayerListItemWidget* GetListItemWidget(FPlayerListContent* TargetItem);
	void CreateListItemWidget(FPlayerListContent* TargetItem);

	void SortAndRebuildUI();
	static bool IsALessThanB(const FPlayerListContent& A, const FPlayerListContent& B);

	FTimerHandle PingRefreshTimer;
	void RefreshPings();


	
	
};
