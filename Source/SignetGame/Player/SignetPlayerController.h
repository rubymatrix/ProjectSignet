// Copyright Red Lotus Games, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "ModularGameplayActors/GSCModularPlayerController.h"
#include "SignetGame/Lobby/SignetProfileSnapshot.h"
#include "SignetPlayerController.generated.h"

struct FCurrencyAdjustment;
class USGProfileDisplayWidget;
class USGProfileEditorWidget;
/**
 * 
 */
UCLASS()
class SIGNETGAME_API ASignetPlayerController : public AGSCModularPlayerController
{
	GENERATED_BODY()

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI")
	TSubclassOf<UUserWidget> AdminPanelWidgetClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI")
	TObjectPtr<UUserWidget> AdminPanelWidget;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI")
	TObjectPtr<USGProfileEditorWidget> ProfileEditorWidget;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI")
	TObjectPtr<USGProfileDisplayWidget> ProfileDisplayWidget;

public:

	ASignetPlayerController();

	virtual void BeginPlay() override;
	UFUNCTION(Client, Reliable) void ClientRequestProfileSnapshot();
	UFUNCTION(Server, Reliable) void ServerSubmitProfileSnapshot(const FSignetProfileSnapshot Snapshot);
	void ClientRequestProfileSnapshot_Implementation();
	void ServerSubmitProfileSnapshot_Implementation(const FSignetProfileSnapshot Snapshot);
	
	UFUNCTION(BlueprintCallable, Server, Reliable) void ServerDebugLogConnections();
	void ServerDebugLogConnections_Implementation();

private:

	void InitializeAdminUI();

	UFUNCTION()
	void OnProfileSaved(const FSignetPlayerProfile& Profile);
	UFUNCTION()
	void OnCurrencyAdded(const FCurrencyAdjustment& Data);
};
