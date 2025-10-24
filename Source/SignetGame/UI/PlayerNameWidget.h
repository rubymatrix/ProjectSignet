// Copyright Red Lotus Games, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerNameWidget.generated.h"

class UTextBlock;
class UEditableTextBox;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPlayerNameChanged, const FString&, NewName);

/**
 * A simple widget with a label and text box for setting the player name.
 */
UCLASS()
class SIGNETGAME_API UPlayerNameWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

protected:
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> Label;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UEditableTextBox> NameTextBox;

public:
	
	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnPlayerNameChanged OnPlayerNameChanged;
	
	UFUNCTION(BlueprintCallable, Category = "PlayerName")
	void SetPlayerName(const FString& NewName);
	
	UFUNCTION(BlueprintCallable, Category = "PlayerName")
	FString GetPlayerName() const;

private:
	UFUNCTION()
	void HandleNameCommitted(const FText& Text, ETextCommit::Type CommitMethod);
};
