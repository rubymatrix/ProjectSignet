// Copyright Red Lotus Games, All Rights Reserved.


#include "SGPlayerListItemWidget.h"
#include "Components/TextBlock.h"



void USGPlayerListItemWidget::SetPlayer(const FPlayerListContent* InPlayer)
{
	if (InPlayer)
	{
		PlayerId = InPlayer->PlayerId;
		PlayerNameText->SetText(InPlayer->PlayerName);
		JobAbbrText->SetText(InPlayer->Job);
		LevelText->SetText(FText::FromString(FString::Printf(TEXT("%i"), InPlayer->Level)));
	}
}

void USGPlayerListItemWidget::SetPing(int32 InPing, bool bIsHost)
{
	auto StringPing = FString::Printf(TEXT("%ims"), InPing);
	PingText->SetText(FText::FromString(StringPing));
}

int32 USGPlayerListItemWidget::GetPlayerId() const
{
	return PlayerId;
}
