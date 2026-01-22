#pragma once

#include "CoreMinimal.h"
#include "Grades.h"
#include "Logging.h"
#include "Stats.generated.h"

enum class EJob : uint8;
enum class ERace : uint8;



USTRUCT(BlueprintType)
struct SIGNETGAME_API FStatCalculation
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsPlayer = true;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int Race = 0;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int Job = 0;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int SubJob = 0;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int JobLevel = 1;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int SubJobLevel = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float PlayerHPMultiplier = 1.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float PlayerMPMultiplier = 1.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float PlayerStatMultiplier = 1.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float SubJobDivisor = 2.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bHeal = false;
};

USTRUCT(BlueprintType)
struct SIGNETGAME_API FBaseStats
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly) int HP = 0;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly) int MP = 0;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly) float STR = 0.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly) float DEX = 0.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly) float VIT = 0.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly) float AGI = 0.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly) float INT = 0.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly) float MND = 0.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly) float CHR = 0.f;
};

static FORCEINLINE int32 ClampRange(int32 Value, int32 Min, int32 Max) { return FMath::Clamp(Value, Min, Max); }

UCLASS()
class SIGNETGAME_API UStatsFunctions : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, BlueprintPure)
	static void PrintStats(const FStatCalculation& Params);

	UFUNCTION(BlueprintCallable, BlueprintPure)
	static FBaseStats CalcBaseStats(const FStatCalculation& Params);
};