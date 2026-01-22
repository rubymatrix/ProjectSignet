#pragma once

#include "CoreMinimal.h"
#include "SignetAttributeRowEntry.generated.h"

/**
 * Simple view model for a single attribute entry in editor list views.
 */
UCLASS(BlueprintType)
class SIGNETGAMEEDITOR_API USignetAttributeRowEntry : public UObject
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadOnly, Category="Signet|AttributeTable")
	FName RowName;

	UPROPERTY(BlueprintReadOnly, Category="Signet|AttributeTable")
	FString DisplayName;

	UPROPERTY(BlueprintReadOnly, Category="Signet|AttributeTable")
	FString AttributeName;

	UPROPERTY(BlueprintReadOnly, Category="Signet|AttributeTable")
	FString SetName;

	UPROPERTY(BlueprintReadOnly, Category="Signet|AttributeTable")
	float BaseValue = 0.0f;

	UPROPERTY(BlueprintReadOnly, Category="Signet|AttributeTable")
	float MinValue = 0.0f;

	UPROPERTY(BlueprintReadOnly, Category="Signet|AttributeTable")
	float MaxValue = 0.0f;

	UFUNCTION(BlueprintCallable, Category="Signet|AttributeTable", meta=(WorldContext="WorldContextObject"))
	static USignetAttributeRowEntry* MakeAttributeRowEntry(UObject* WorldContextObject, FName InRowName, const FString& InDisplayName, const FString& InAttributeName, const FString& InSetName, float InBaseValue, float InMinValue, float InMaxValue);
};
