#pragma once
#include "CoreMinimal.h"
#include "Grades.generated.h"

UCLASS()
class SIGNETGAME_API UGrades : public UObject
{
	GENERATED_BODY()

public:
	/** Getters (static utility functions) */
	UFUNCTION(BlueprintCallable, Category="Grades")
	static uint8 GetJobGrade(uint8 Job, uint8 Stat);

	UFUNCTION(BlueprintCallable, Category="Grades")
	static uint8 GetRaceGrade(uint8 Race, uint8 Stat);

	UFUNCTION(BlueprintCallable, Category="Grades")
	static float GetHPScale(uint8 Rank, uint8 Scale);

	UFUNCTION(BlueprintCallable, Category="Grades")
	static float GetMPScale(uint8 Rank, uint8 Scale);

	UFUNCTION(BlueprintCallable, Category="Grades")
	static float GetStatScale(uint8 Rank, uint8 Scale);

	UFUNCTION(BlueprintCallable, Category="Grades")
	static float GetMobHPScale(uint8 Rank, uint8 Scale);

	UFUNCTION(BlueprintCallable, Category="Grades")
	static float GetMobRBI(uint8 Rank, uint8 Scale);

private:
	/** Data tables converted from std::array */
	static const TArray<TArray<uint8>> JobGrades;
	static const TArray<TArray<uint8>> RaceGrades;
	static const TArray<TArray<float>> HPScale;
	static const TArray<TArray<float>> MPScale;
	static const TArray<TArray<float>> StatScale;
	static const TArray<TArray<float>> MobHPScale;
	static const TArray<TArray<float>> MobRBI;
};