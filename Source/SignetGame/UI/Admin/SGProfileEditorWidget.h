// Copyright 2025, Red Lotus Games
#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SignetGame/Save/SignetSaveTypes.h"
#include "SGProfileEditorWidget.generated.h"

class UEditableTextBox;
class UComboBoxString;
class USpinBox;
class UButton;
class USignetSaveGame;

USTRUCT(BlueprintType)
struct SIGNETGAME_API FCurrencyAdjustment
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ECurrencyType Currency = static_cast<ECurrencyType>(0);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Amount = 0;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnProfileSaved, const FSignetPlayerProfile&, Data);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnJobSaved, const FSignetJobProgress&, Data);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCurrencyAdded, const FCurrencyAdjustment&, Data);

/**
 * Widget for editing player profile & adding currency.
 *
 * Designer hierarchy (names must match):
 * - NameTextBox                (UEditableTextBox)
 * - JobCombo                   (UComboBoxString)
 * - LevelSpinBox               (USpinBox - int)
 * - ExpSpinBox                 (USpinBox - int)
 * - CurrencyCombo              (UComboBoxString)
 * - CurrencyAmountSpinBox      (USpinBox - int)
 * - SaveButton                 (UButton)
 * - AddCurrencyButton          (UButton)
 */
UCLASS()
class SIGNETGAME_API USGProfileEditorWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	// Fill the widget with existing data (e.g., from your Save Subsystem)
	UFUNCTION(BlueprintCallable, Category="Profile")
	void InitializeFromData(const USignetSaveGame* Save);

	// Broadcasts when user presses Save
	UPROPERTY(BlueprintAssignable, Category="Profile|Events")
	FOnProfileSaved OnProfileSaved;

	// Broadcasts when user presses Add Currency
	UPROPERTY(BlueprintAssignable, Category="Profile|Events")
	FOnCurrencyAdded OnCurrencyAdded;

protected:
	virtual void NativeConstruct() override;

	// --- Designer Bindings ---
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UEditableTextBox> NameTextBox;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UComboBoxString> JobCombo;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<USpinBox> LevelSpinBox;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<USpinBox> ExpSpinBox;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UComboBoxString> CurrencyCombo;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<USpinBox> CurrencyAmountSpinBox;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UButton> SaveButton;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UButton> AddCurrencyButton;

private:
	// Cache of enum name → value for quick lookup
	TMap<FString, int64> JobLabelToValue;
	TMap<FString, int64> CurrencyLabelToValue;

	void PopulateEnumCombo(UComboBoxString* Combo, UEnum* Enum, TMap<FString, int64>& OutLabelToValue);
	static FString MakeEnumEntryLabel(UEnum* Enum, int64 Value);

	// Button handlers
	UFUNCTION()
	void HandleSavePressed();

	UFUNCTION()
	void HandleAddCurrencyPressed();

	// Helpers to read UI → data
	bool TryGetSelectedJob(EJob& OutJob) const;
	bool TryGetSelectedCurrency(ECurrencyType& OutCurrency) const;
};
