#pragma once
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Blueprint/IUserObjectListEntry.h"
#include "SignetInventoryRowWidget.generated.h"

class UButton;
class UImage;
class UTextBlock;
class UBorder;
class USignetInventoryListItem;

UCLASS()
class SIGNETGAME_API USignetInventoryRowWidget : public UUserWidget, public IUserObjectListEntry
{
	GENERATED_BODY()
public:
	virtual void NativeOnListItemObjectSet(UObject* ListItemObject) override;

protected:

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UButton> RootButton = nullptr;
	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UImage> IconImage = nullptr;
	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> NameText = nullptr;
	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> QtyText  = nullptr;
	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UBorder> IconOutline = nullptr;
};