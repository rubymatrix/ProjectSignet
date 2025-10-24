// Copyright Red Lotus Games, All Rights Reserved.


#include "SGPlayerListWidget.h"

#include "SGPlayerListItemWidget.h"
#include "Components/StackBox.h"
#include "SignetGame/Player/SignetPlayerState.h"
#include "SignetGame/Core/SignetLobbyGameState.h"

static int32 GetPingMs(const ASignetPlayerState* PS)
{
	if (!PS) return -1;
	const double Ping = PS->GetPingInMilliseconds(); // already de-compressed
	return Ping > 0.0 ? FMath::Clamp((int32)FMath::RoundToInt(Ping), 0, 999) : -1;
}


void USGPlayerListWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (auto* GS = GetWorld()->GetGameState<ASignetLobbyGameState>())
	{
		GS->OnLobbyPlayerLeft.AddDynamic(this, &USGPlayerListWidget::PlayerLeft);
		GS->OnLobbyPlayerUpdated.AddDynamic(this, &USGPlayerListWidget::ReceivedPlayerUpdate);
	}

	GetWorld()->GetTimerManager().SetTimer(
	PingRefreshTimer, this, &USGPlayerListWidget::RefreshPings, 1.0f, true);
}

void USGPlayerListWidget::RefreshPings()
{
	if (auto* GS = GetWorld()->GetGameState<ASignetLobbyGameState>())
	{
		for (ASignetPlayerState* PS : GS->GetLobbyPlayers(/*bSortByJoinOrder=*/false))
		{
			if (auto* Entry = PlayerList.FindByPredicate([&](const FPlayerListContent& I){ return I.PlayerId == PS->GetPlayerId(); }))
			{
				const int32 NewPing = GetPingMs(PS);
				if (auto* Row = GetListItemWidget(Entry))
				{
					Row->SetPing(NewPing, PS->IsLobbyHost());
				}
			}
		}
	}
}

void USGPlayerListWidget::NativeDestruct()
{
	Super::NativeDestruct();
	GetWorld()->GetTimerManager().ClearTimer(PingRefreshTimer);
}

void USGPlayerListWidget::UpdatePlayer(ASignetPlayerState* Player)
{
	if (auto ListItem = PlayerList.FindByPredicate([this, Player](const FPlayerListContent& Item)
	{
		return Item.PlayerId == Player->GetPlayerId();
	}))
	{
		ListItem->bIsLeader = Player->IsLobbyHost();
		ListItem->PlayerId = Player->GetPlayerId();
		ListItem->PlayerName = FText::FromString(Player->PlayerName);
		ListItem->Level = Player->Level;
		ListItem->Job = GetJobAbbr(Player->Job);

		if (auto Widget = GetListItemWidget(ListItem))
		{
			Widget->SetPlayer(ListItem);
		}
	}
}

void USGPlayerListWidget::RebuildList()
{
	if (auto* GS = GetWorld()->GetGameState<ASignetLobbyGameState>())
	{
		for (auto* Player : GS->GetLobbyPlayers())
		{
			PlayerJoined(Player);
		}
	}
}

bool USGPlayerListWidget::PlayerExists(ASignetPlayerState* Player)
{
	auto Item = PlayerList.FindByPredicate([this, Player](const FPlayerListContent& Content)
	{
		return Content.PlayerId == Player->GetPlayerId();
	});
	
	return (Item != nullptr);
}

void USGPlayerListWidget::ReceivedPlayerUpdate(ASignetPlayerState* Player)
{
	if (PlayerExists(Player))
	{
		UpdatePlayer(Player);
	}
	else
	{
		PlayerJoined(Player);	
	}
}

void USGPlayerListWidget::PlayerJoined(ASignetPlayerState* Player)
{
	if (auto ListItem = PlayerList.FindByPredicate([this, Player](const FPlayerListContent& Item)
	{
		return Item.PlayerId == Player->GetPlayerId();
	})) // Player already exists in the list, update and return
	{
		ListItem->bIsLeader = Player->IsLobbyHost();
		ListItem->PlayerId = Player->GetPlayerId();
		ListItem->PlayerName = FText::FromString(Player->PlayerName);
		ListItem->Level = Player->Level;
		ListItem->Job = GetJobAbbr(Player->Job);

		if (const auto ItemWidget = GetListItemWidget(ListItem))
		{
			ItemWidget->SetPlayer(ListItem); // Refresh player data
		}
		
		return;
	}
	
	PlayerList.Add({
		Player->GetPlayerId(),
		Player->IsLobbyHost(),
		FText::FromString(Player->PlayerName),
		GetJobAbbr(Player->Job),
		Player->Level
	});

	SortAndRebuildUI();
}

void USGPlayerListWidget::PlayerLeft(ASignetPlayerState* Player)
{
	// Check if the player was in the list
	if (auto* ListItem = PlayerList.FindByPredicate([this, Player](const FPlayerListContent& Item)
	{
		return Item.PlayerId == Player->GetPlayerId();
	}))
	{
		if (auto* Widget = GetListItemWidget(ListItem))
		{
			// Remove/Destroy the widget
			Players->RemoveChild(Widget);
			Widget->MarkAsGarbage();
		}

		// Remove the item from the player list
		const auto Index = PlayerList.IndexOfByPredicate([this, Player](const FPlayerListContent& Item)
		{
			return Item.PlayerId == Player->GetPlayerId();
		});

		if (Index != INDEX_NONE)
		{
			PlayerList.RemoveAt(Index);
		}

		SortAndRebuildUI();
	}
}

FText USGPlayerListWidget::GetJobAbbr(const EJob& JobType)
{
	TMap<EJob, FString> JobMap = {
		{ EJob::Warrior, TEXT("WAR") },
		{ EJob::Monk, TEXT("MNK") },
		{ EJob::Thief, TEXT("THF") },
		{ EJob::RedMage, TEXT("RDM") },
		{ EJob::BlackMage, TEXT("BLM") },
		{ EJob::WhiteMage, TEXT("WHM") }
	};

	if (auto* Item = JobMap.Find(JobType))
	{
		return FText::FromString(*Item);
	}

	return FText::FromString("");
}

FPlayerListContent* USGPlayerListWidget::GetContent(int32 PlayerId)
{
	if (auto* ListItem = PlayerList.FindByPredicate([this, PlayerId](const FPlayerListContent& Item)
	{
		return Item.PlayerId == PlayerId;
	}))
	{
		return ListItem;
	}
	return nullptr;
}

USGPlayerListItemWidget* USGPlayerListWidget::GetListItemWidget(FPlayerListContent* TargetItem)
{
	auto Children = Players->GetAllChildren();
	if (Children.Num() < 1) return nullptr;
	
	auto* TargetChild = Children.FindByPredicate([this, TargetItem](UWidget* Child)
	{
		if (auto Item = Cast<USGPlayerListItemWidget>(Child); Item->GetPlayerId() == TargetItem->PlayerId)
		{
			return true;
		}

		return false;
	});

	if (TargetChild)
	{
		return Cast<USGPlayerListItemWidget>(*TargetChild);
	};
	return nullptr;
}

void USGPlayerListWidget::CreateListItemWidget(FPlayerListContent* TargetItem)
{
	if (auto ItemWidget = Cast<USGPlayerListItemWidget>(CreateWidget(GetOwningPlayer(), ListItemClass)))
	{
		ItemWidget->SetPlayer(TargetItem);
		Players->AddChildToStackBox(ItemWidget);
	}
}

bool USGPlayerListWidget::IsALessThanB(const FPlayerListContent& A, const FPlayerListContent& B)
{
	// Host/leader always first
	if (A.bIsLeader != B.bIsLeader)
	{
		return A.bIsLeader; // true comes before false
	}

	// Case-insensitive alphabetical by display name
	const FString AName = A.PlayerName.ToString();
	const FString BName = B.PlayerName.ToString();
	const int32 NameCmp = AName.Compare(BName, ESearchCase::IgnoreCase);

	if (NameCmp != 0)
	{
		return NameCmp < 0;
	}

	// Stable tie-breaker so every client sees the same order
	return A.PlayerId < B.PlayerId;
}

void USGPlayerListWidget::SortAndRebuildUI()
{
	// 1) Sort the backing data
	PlayerList.Sort([](const FPlayerListContent& A, const FPlayerListContent& B)
	{
		return IsALessThanB(A, B);
	});

	// 2) Rebuild the visible list in sorted order
	if (!Players) return;

	Players->ClearChildren();

	for (const FPlayerListContent& Entry : PlayerList)
	{
		if (auto* Item = GetContent(Entry.PlayerId))
		{
			// Reuse widget if you want, but simplest is to recreate here
			CreateListItemWidget(Item);
		}
	}
}