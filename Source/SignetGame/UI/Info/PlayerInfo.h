// Copyright Red Lotus Games, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "Blueprint/UserWidget.h"
#include "PlayerInfo.generated.h"

class ASignetPlayerState;
struct FGameplayAttribute;
struct FOnAttributeChangeData;
class USignetAbilitySystemComponent;
class UTextBlock;

UCLASS()
class SIGNETGAME_API UPlayerInfo : public UUserWidget
{
	GENERATED_BODY()


protected:

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> PlayerNameTextBlock;
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> JobTextBlock;
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> SubJobTextBlock;
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> HPTextBlock;
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> MPTextBlock;
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> STRTextBlock;
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> DEXTextBlock;
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> VITTextBlock;
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> AGITextBlock;
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> INTTextBlock;
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> MNDTextBlock;
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> CHRTextBlock;
	
public:

	virtual void NativeConstruct() override;

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	virtual void NativeDestruct() override;

private:

	bool bInitialized = false;

	TMap<FGameplayAttribute, FDelegateHandle> BoundAttributeHandles;
	TWeakObjectPtr<USignetAbilitySystemComponent> PlayerAsc;

	void InitWithActorInfo(AActor* InActor);

	void BindAttributeChange(const FGameplayAttribute& Attribute);

	void OnAttributeChanged(const FOnAttributeChangeData& Data);

	void OnPlayerUpdated(ASignetPlayerState* Player);

	void SetPlayerName();
	void Update();
	void SetJobText();
	void SetSubJobText();
	void SetHPText();
	void SetMPText();
	void SetSTRText();
	void SetDEXText();
	void SetVITText();
	void SetAGIText();
	void SetINTText();
	void SetMNDText();
	void SetCHRText();

	void SetAttrText(UTextBlock* InText, const FGameplayAttribute& Attribute);
	
};
