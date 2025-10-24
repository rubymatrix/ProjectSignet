// Copyright 2025, Red Lotus Games
#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SignetGame/Save/SignetSaveTypes.h"
#include "SGProfileDisplayWidget.generated.h"

class USignetSaveGame;
class UTextBlock;

UCLASS(ClassGroup=Signet)
class SIGNETGAME_API USGProfileDisplayWidget : public UUserWidget
{
	GENERATED_BODY()

protected:

	UPROPERTY(meta=(BindWidget)) TObjectPtr<UTextBlock> PlayerName;
	UPROPERTY(meta=(BindWidget)) TObjectPtr<UTextBlock> Job;
	UPROPERTY(meta=(BindWidget)) TObjectPtr<UTextBlock> EXP;
	UPROPERTY(meta=(BindWidget)) TObjectPtr<UTextBlock> AP;
	UPROPERTY(meta=(BindWidget)) TObjectPtr<UTextBlock> SP;
	UPROPERTY(meta=(BindWidget)) TObjectPtr<UTextBlock> BeastmenSeals;
	UPROPERTY(meta=(BindWidget)) TObjectPtr<UTextBlock> KindredSeals;
	UPROPERTY(meta=(BindWidget)) TObjectPtr<UTextBlock> Gil;

	virtual void NativeConstruct() override;
	
public:

	void SetDataFromSave();
	
private:

	FString GetJobName(const EJob JobType);
	FText GetText(int Amount);
	
};