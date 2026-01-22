// Copyright Red Lotus Games, All Rights Reserved.

#include "Stats.h"
#include "SignetGame/Save/SignetSaveTypes.h"
#include "Logging.h"

void UStatsFunctions::PrintStats(const FStatCalculation& Params)
{
	auto BaseStats = CalcBaseStats(Params);
	if (GEngine)
	{
		const EJob Job = static_cast<EJob>(Params.Job);
		const EJob Sub = static_cast<EJob>(Params.SubJob);
		const FString JobName = UEnum::GetValueAsString(Job);
		const FString SubName = UEnum::GetValueAsString(Sub);
		
		// Draw Job Names
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, FString::Printf(TEXT("%i%s/%i%s"), Params.JobLevel, *JobName, Params.SubJobLevel, *SubName));
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("STR %f"), BaseStats.STR));
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("DEX %f"), BaseStats.DEX));
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("VIT %f"), BaseStats.VIT));
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("AGI %f"), BaseStats.AGI));
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("INT %f"), BaseStats.INT));
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("MND %f"), BaseStats.MND));
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("CHR %f"), BaseStats.CHR));
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("HP %i MP %i"), BaseStats.HP, BaseStats.MP));

		// Also log to console
		UE_LOG(LogSignet, Log, TEXT("%i%s/%i%s"), Params.JobLevel, *JobName, Params.SubJobLevel, *SubName);
		UE_LOG(LogSignet, Log, TEXT("STR %f"), BaseStats.STR);
		UE_LOG(LogSignet, Log, TEXT("DEX %f"), BaseStats.DEX);
		UE_LOG(LogSignet, Log, TEXT("VIT %f"), BaseStats.VIT);
		UE_LOG(LogSignet, Log, TEXT("AGI %f"), BaseStats.AGI);
		UE_LOG(LogSignet, Log, TEXT("INT %f"), BaseStats.INT);
		UE_LOG(LogSignet, Log, TEXT("MND %f"), BaseStats.MND);
		UE_LOG(LogSignet, Log, TEXT("CHR %f"), BaseStats.CHR);
		UE_LOG(LogSignet, Log, TEXT("HP %i MP %i"), BaseStats.HP, BaseStats.MP);
	}
}

FBaseStats UStatsFunctions::CalcBaseStats(const FStatCalculation& Params)
{
	FBaseStats Stats;

	// ---- Column indices (mirroring your function) ----
	const int32 BaseCol        = 0;
	const int32 ScaleTo60Col   = 1;
	const int32 ScaleOver30Col = 2;
	const int32 ScaleOver60Col = 3;
	const int32 ScaleOver75Col = 4;

	// MP specific column after 60
	const int32 ScaleOver60_MP = 2;
	const int32 ScaleOver75    = 3; // used for stats

	// ---- Extract levels/jobs ----
	const uint8 mlvl = Params.JobLevel;
	const uint8 slvl = Params.SubJobLevel;
	const uint8 mjob = static_cast<uint8>(Params.Job);
	const uint8 sjob = static_cast<uint8>(Params.SubJob);
	const uint8 raceIdx = static_cast<uint8>(Params.Race);

	// ---- Guard: if job equals MON (if you have that enum), map to WAR. ----
	// If you have MON in your EJobType, handle it here. Example:
	// if (mjob == EJobType::MON || sjob == EJobType::MON) { mjob = EJobType::WAR; sjob = EJobType::WAR; }

	// ---- Precompute level deltas (as per your original logic) ----
	const int32 mainOver30      = ClampRange(mlvl - 30, 0, 30);
	const int32 mainUpTo60      = (mlvl < 60 ? mlvl - 1 : 59);
	const int32 mainOver60To75  = ClampRange(mlvl - 60, 0, 15);
	const int32 mainOver75      = (mlvl < 75 ? 0 : mlvl - 75);

	const int32 mainOver10            = (mlvl < 10 ? 0 : mlvl - 10);
	const int32 mainOver50andUnder60  = ClampRange(mlvl - 50, 0, 10);
	const int32 mainOver60            = (mlvl < 60 ? 0 : mlvl - 60);

	const int32 subOver10  = ClampRange(slvl - 10, 0, 20);
	const int32 subOver30  = (slvl < 30 ? 0 : slvl - 30);

	// --------------------
	// HP
	// --------------------
	float raceStat = 0.f, jobStat = 0.f, sJobStat = 0.f;
	int32 bonusStat = 0;
	uint8 grade = 0;

	// Race contribution
	grade = UGrades::GetRaceGrade(raceIdx, /*stat index*/ 0);
	raceStat =
		UGrades::GetHPScale(grade, BaseCol) +
		UGrades::GetHPScale(grade, ScaleTo60Col)   * mainUpTo60 +
		UGrades::GetHPScale(grade, ScaleOver30Col) * mainOver30 +
		UGrades::GetHPScale(grade, ScaleOver60Col) * mainOver60To75 +
		UGrades::GetHPScale(grade, ScaleOver75Col) * mainOver75;

	// Main Job
	grade = UGrades::GetJobGrade(mjob, /*stat index*/ 0);
	jobStat =
		UGrades::GetHPScale(grade, BaseCol) +
		UGrades::GetHPScale(grade, ScaleTo60Col)   * mainUpTo60 +
		UGrades::GetHPScale(grade, ScaleOver30Col) * mainOver30 +
		UGrades::GetHPScale(grade, ScaleOver60Col) * mainOver60To75 +
		UGrades::GetHPScale(grade, ScaleOver75Col) * mainOver75;

	// Bonus
	bonusStat = (mainOver10 + mainOver50andUnder60) * 2;

	// Sub Job
	if (slvl > 0)
	{
		grade = UGrades::GetJobGrade(sjob, /*stat index*/ 0);
		sJobStat =
			UGrades::GetHPScale(grade, BaseCol) +
			UGrades::GetHPScale(grade, ScaleTo60Col)   * (slvl - 1) +
			UGrades::GetHPScale(grade, ScaleOver30Col) * subOver30 +
			subOver30 + subOver10;
		sJobStat *= 0.5f;
	}

	const auto HPMultiplier = Params.bIsPlayer ? Params.PlayerHPMultiplier : 1.f;
	const auto MPMultiplier = Params.bIsPlayer ? Params.PlayerMPMultiplier : 1.f;
	const auto StatMultiplier = Params.bIsPlayer ? Params.PlayerStatMultiplier : 1.f;

	// TODO: Add Merit MaxHP back in
	// static_cast<float>(P.MeritMaxHP)
	const float MaxHP = HPMultiplier * (raceStat + jobStat + bonusStat + sJobStat);

	// Set the player's max HP
	Stats.HP = MaxHP;
	
	// --------------------
	// MP
	// --------------------
	raceStat = jobStat = sJobStat = 0.f;

	// Race MP
	grade = UGrades::GetRaceGrade(raceIdx, /*MP column*/ 1);

	// If main job has no MP grade, use racial bonus from subjob level (if subjob has MP grade)
	if (UGrades::GetJobGrade(mjob, /*MP stat*/ 1) == 0)
	{
		if (slvl > 0 && UGrades::GetJobGrade(sjob, /*MP stat*/ 1) != 0)
		{
			// ( base + <60 * (slvl-1) ) / divisor
			raceStat = (UGrades::GetMPScale(grade, BaseCol) +
						UGrades::GetMPScale(grade, ScaleTo60Col) * (slvl - 1)) / FMath::Max(Params.SubJobDivisor, 0.0001f);
		}
	}
	else
	{
		// Normal racial bonus
		raceStat =
			UGrades::GetMPScale(grade, BaseCol) +
			UGrades::GetMPScale(grade, ScaleTo60Col) * mainUpTo60 +
			UGrades::GetMPScale(grade, ScaleOver60_MP) * mainOver60;
	}

	// Main Job MP
	grade = UGrades::GetJobGrade(mjob, /*MP stat*/ 1);
	if (grade > 0)
	{
		jobStat =
			UGrades::GetMPScale(grade, BaseCol) +
			UGrades::GetMPScale(grade, ScaleTo60Col) * mainUpTo60 +
			UGrades::GetMPScale(grade, ScaleOver60_MP) * mainOver60;
	}

	// Sub Job MP
	if (slvl > 0)
	{
		grade = UGrades::GetJobGrade(sjob, /*MP stat*/ 1);
		sJobStat = (UGrades::GetMPScale(grade, BaseCol) +
					UGrades::GetMPScale(grade, ScaleTo60Col) * (slvl - 1)) / FMath::Max(Params.SubJobDivisor, 0.0001f);
	}

	const float MaxMP = MPMultiplier * (raceStat + jobStat + sJobStat);
	Stats.MP = MaxMP;
	

	// --------------------
	// Base Stats: STR,DEX,VIT,AGI,INT,MND,CHR correspond to statIndex 2..8
	// --------------------
	for (uint8 StatIndex = 2; StatIndex <= 8; ++StatIndex)
	{
		// Race portion
		grade = UGrades::GetRaceGrade(raceIdx, StatIndex);
		raceStat = UGrades::GetStatScale(grade, BaseCol) +
				   UGrades::GetStatScale(grade, ScaleTo60Col) * mainUpTo60;
	
		if (mainOver60 > 0)
		{
			raceStat += UGrades::GetStatScale(grade, ScaleOver60Col) * mainOver60;
			if (mainOver75 > 0)
			{
				raceStat += UGrades::GetStatScale(grade, ScaleOver75) * mainOver75 - (mlvl >= 75 ? 0.01f : 0.f);
			}
		}
	
		// Job portion
		grade   = UGrades::GetJobGrade(mjob, StatIndex);
		jobStat = UGrades::GetStatScale(grade, BaseCol) +
				  UGrades::GetStatScale(grade, ScaleTo60Col) * mainUpTo60;
	
		if (mainOver60 > 0)
		{
			jobStat += UGrades::GetStatScale(grade, ScaleOver60Col) * mainOver60;
			if (mainOver75 > 0)
			{
				jobStat += UGrades::GetStatScale(grade, ScaleOver75) * mainOver75 - (mlvl >= 75 ? 0.01f : 0.f);
			}
		}
	
		// Subjob portion
		if (slvl > 0)
		{
			grade = UGrades::GetJobGrade(sjob, StatIndex);
			sJobStat = (UGrades::GetStatScale(grade, BaseCol) +
						UGrades::GetStatScale(grade, ScaleTo60Col) * (slvl - 1)) * 0.5f;
		}
		else
		{
			sJobStat = 0.f;
		}

		const float FinalValue = StatMultiplier * (raceStat + jobStat + sJobStat);
		switch (StatIndex)
		{
		case 2: Stats.STR = FinalValue; break;
		case 3: Stats.DEX = FinalValue; break;
		case 4: Stats.VIT = FinalValue; break;
		case 5: Stats.AGI = FinalValue; break;
		case 6: Stats.INT = FinalValue; break;
		case 7: Stats.MND = FinalValue; break;
		case 8: Stats.CHR = FinalValue; break;
		default: break; 
		}
	}
	
	return Stats;
}
