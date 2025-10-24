// Copyright 2025, Red Lotus Games
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "SignetDebugHUD.generated.h"

class USignetSaveSubsystem;

UCLASS()
class SIGNETGAME_API ASignetDebugHUD : public AHUD
{
	GENERATED_BODY()

public:
	
	ASignetDebugHUD();

protected:
	
	virtual void ShowDebugInfo(float& YL, float& YPos) override;

private:
	
	static const FName CategoryName;
	
	TWeakObjectPtr<USignetSaveSubsystem> SaveSubsystem;

	USignetSaveSubsystem* GetSaveSubsystem() const;

	// Drawing helpers
	float DrawHeader(UCanvas* InCanvas, float X, float& Y, const FString& Title) const;
	float DrawKeyValue(UCanvas* InCanvas, float X, float& Y, const FString& Key, const FString& Value) const;
	float DrawSeparator(UCanvas* InCanvas, float X, float& Y) const;

	// The actual renderer for our category
	void DrawSignetProfile(UCanvas* InCanvas, APlayerController* PC) const;

	// Layout
	mutable float LeftX = 8.f;     // left margin
	mutable float TopY  = 300.f;     // top margin
	mutable float LineH = 0.f;      // computed per-font line height
};