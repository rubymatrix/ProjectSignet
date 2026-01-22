// Copyright Red Lotus Games, All Rights Reserved.

#include "SkillRankStatics.h"
#include "SignetGame/Player/SignetPlayerCharacter.h"
#include "SignetGame/Save/SignetSaveSubsystem.h"
#include "SignetGame/Abilities/SignetAbilitySystemComponent.h"
#include "SignetGame/Abilities/Attributes/SignetCombatAttributeSet.h"
#include "SignetGame/Abilities/TagCache.h"
#include "SignetGame/Combat/Components/CombatTextComponent.h"

namespace
{
	static constexpr int32 MaxSkillLevel = 99;
	static constexpr int32 MaxOverLevelBonus = 156;
	static constexpr int32 MaxSkillRankIndex = static_cast<int32>(ESkillRank::G);
	static constexpr int32 JobCount = static_cast<int32>(EJob::Monstrosity) + 1;

	TArray<uint8> CreateEmptyJobRanks()
	{
		TArray<uint8> JobRanks;
		JobRanks.Init(0, JobCount);
		return JobRanks;
	}

	const TArray<EJob>& GetJobOrder()
	{
		static const TArray<EJob> JobOrder = {
			EJob::Warrior,
			EJob::Monk,
			EJob::WhiteMage,
			EJob::BlackMage,
			EJob::RedMage,
			EJob::Thief,
			EJob::Paladin,
			EJob::DarkKnight,
			EJob::Beastmaster,
			EJob::Bard,
			EJob::Ranger,
			EJob::Samurai,
			EJob::Ninja,
			EJob::Dragoon,
			EJob::Summoner,
			EJob::BlueMage,
			EJob::Corsair,
			EJob::Puppetmaster,
			EJob::Dancer,
			EJob::Scholar,
			EJob::Geomancer,
			EJob::Runefencer,
		};
		return JobOrder;
	}

	void AddSkillRankRow(TMap<FGameplayTag, TArray<uint8>>& Table, const TCHAR* TagName, const TArray<uint8>& SqlRanks)
	{
		TArray<uint8> JobRanks = CreateEmptyJobRanks();
		const TArray<EJob>& JobOrder = GetJobOrder();
		const int32 Count = FMath::Min(JobOrder.Num(), SqlRanks.Num());
		for (int32 Index = 0; Index < Count; ++Index)
		{
			const int32 JobIndex = static_cast<int32>(JobOrder[Index]);
			JobRanks[JobIndex] = SqlRanks[Index];
		}

		const FGameplayTag Tag = FGameplayTag::RequestGameplayTag(TagName);
		Table.Add(Tag, JobRanks);
	}

	void EnsureAllSkillTagsPresent(TMap<FGameplayTag, TArray<uint8>>& Table)
	{
		const TArray<const TCHAR*> AllTags = {
			TEXT("Data.Skill.Alchemy"),
			TEXT("Data.Skill.Archery"),
			TEXT("Data.Skill.AutomatonMagic"),
			TEXT("Data.Skill.AutomatonMelee"),
			TEXT("Data.Skill.AutomatonRanged"),
			TEXT("Data.Skill.Axe"),
			TEXT("Data.Skill.BlueMagic"),
			TEXT("Data.Skill.Bonecraft"),
			TEXT("Data.Skill.Clothcraft"),
			TEXT("Data.Skill.Club"),
			TEXT("Data.Skill.Cooking"),
			TEXT("Data.Skill.Dagger"),
			TEXT("Data.Skill.DarkMagic"),
			TEXT("Data.Skill.Dig"),
			TEXT("Data.Skill.DivineMagic"),
			TEXT("Data.Skill.DualWield.Category1"),
			TEXT("Data.Skill.DualWield.Category2"),
			TEXT("Data.Skill.DualWield.Category3"),
			TEXT("Data.Skill.DualWield.Category4"),
			TEXT("Data.Skill.DualWield.SubCategory1"),
			TEXT("Data.Skill.DualWield.SubCategory2"),
			TEXT("Data.Skill.DualWield.SubCategory3"),
			TEXT("Data.Skill.DualWield.SubCategory4"),
			TEXT("Data.Skill.ElementalMagic"),
			TEXT("Data.Skill.EnfeeblingMagic"),
			TEXT("Data.Skill.EnhancingMagic"),
			TEXT("Data.Skill.Evasion"),
			TEXT("Data.Skill.Fishing"),
			TEXT("Data.Skill.Geomancy"),
			TEXT("Data.Skill.Goldsmithing"),
			TEXT("Data.Skill.GreatAxe"),
			TEXT("Data.Skill.GreatKatana"),
			TEXT("Data.Skill.GreatSword"),
			TEXT("Data.Skill.Guard"),
			TEXT("Data.Skill.Handbell"),
			TEXT("Data.Skill.HandToHand"),
			TEXT("Data.Skill.HealingMagic"),
			TEXT("Data.Skill.Katana"),
			TEXT("Data.Skill.Leathercraft"),
			TEXT("Data.Skill.Marksmanship"),
			TEXT("Data.Skill.Ninjutsu"),
			TEXT("Data.Skill.None"),
			TEXT("Data.Skill.Parry"),
			TEXT("Data.Skill.Polearm"),
			TEXT("Data.Skill.Rid"),
			TEXT("Data.Skill.Scythe"),
			TEXT("Data.Skill.Shield"),
			TEXT("Data.Skill.Singing"),
			TEXT("Data.Skill.Smithing"),
			TEXT("Data.Skill.Staff"),
			TEXT("Data.Skill.StringInstrument"),
			TEXT("Data.Skill.SummoningMagic"),
			TEXT("Data.Skill.Sword"),
			TEXT("Data.Skill.Synergy"),
			TEXT("Data.Skill.Throwing"),
			TEXT("Data.Skill.Unarmed"),
			TEXT("Data.Skill.WindInstrument"),
			TEXT("Data.Skill.Woodworking"),
		};

		for (const TCHAR* TagName : AllTags)
		{
			const FGameplayTag Tag = FGameplayTag::RequestGameplayTag(TagName);
			if (!Table.Contains(Tag))
			{
				Table.Add(Tag, CreateEmptyJobRanks());
			}
		}
	}

	const TMap<FGameplayTag, TArray<uint8>>& GetSkillRankTable()
	{
		static TMap<FGameplayTag, TArray<uint8>> Table;
		static bool bInitialized = false;

		if (!bInitialized)
		{
			Table.Reserve(66);
			AddSkillRankRow(Table, TEXT("Data.Skill.HandToHand"), { 9, 1, 0, 0, 0, 10, 0, 0, 0, 0, 0, 0, 10, 0, 0, 0, 0, 1, 9, 0, 0, 0 });
			AddSkillRankRow(Table, TEXT("Data.Skill.Dagger"), { 5, 0, 0, 9, 4, 1, 8, 7, 6, 5, 5, 10, 6, 10, 10, 0, 3, 8, 1, 9, 8, 0 });
			AddSkillRankRow(Table, TEXT("Data.Skill.Sword"), { 4, 0, 0, 0, 4, 9, 1, 5, 10, 8, 9, 6, 7, 8, 0, 1, 5, 0, 9, 0, 0, 2 });
			AddSkillRankRow(Table, TEXT("Data.Skill.GreatSword"), { 3, 0, 0, 0, 0, 0, 4, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 });
			AddSkillRankRow(Table, TEXT("Data.Skill.Axe"), { 2, 0, 0, 0, 0, 0, 0, 5, 1, 0, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5 });
			AddSkillRankRow(Table, TEXT("Data.Skill.GreatAxe"), { 1, 0, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4 });
			AddSkillRankRow(Table, TEXT("Data.Skill.Scythe"), { 3, 0, 0, 10, 0, 0, 0, 1, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 });
			AddSkillRankRow(Table, TEXT("Data.Skill.Polearm"), { 5, 0, 0, 0, 0, 0, 10, 0, 0, 0, 0, 5, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0 });
			AddSkillRankRow(Table, TEXT("Data.Skill.Katana"), { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0 });
			AddSkillRankRow(Table, TEXT("Data.Skill.GreatKatana"), { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 8, 0, 0, 0, 0, 0, 0, 0, 0, 0 });
			AddSkillRankRow(Table, TEXT("Data.Skill.Club"), { 5, 6, 3, 6, 9, 10, 2, 8, 9, 9, 10, 10, 10, 10, 6, 5, 0, 9, 0, 6, 3, 8 });
			AddSkillRankRow(Table, TEXT("Data.Skill.Staff"), { 4, 4, 6, 5, 0, 0, 2, 0, 0, 6, 0, 0, 0, 5, 4, 0, 0, 0, 0, 6, 6, 0 });
			AddSkillRankRow(Table, TEXT("Data.Skill.Archery"), { 9, 0, 0, 0, 9, 8, 0, 0, 0, 0, 1, 6, 10, 0, 0, 0, 0, 0, 0, 0, 0, 0 });
			AddSkillRankRow(Table, TEXT("Data.Skill.Marksmanship"), { 9, 0, 0, 0, 0, 6, 0, 10, 0, 0, 1, 0, 7, 0, 0, 0, 4, 0, 0, 0, 0, 0 });
			AddSkillRankRow(Table, TEXT("Data.Skill.Throwing"), { 9, 10, 10, 9, 11, 9, 0, 0, 0, 10, 8, 6, 1, 0, 0, 0, 6, 6, 6, 9, 0, 0 });
			AddSkillRankRow(Table, TEXT("Data.Skill.Guard"), { 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0 });
			AddSkillRankRow(Table, TEXT("Data.Skill.Evasion"), { 7, 3, 10, 10, 9, 1, 7, 7, 7, 9, 10, 3, 2, 4, 10, 8, 9, 4, 3, 10, 9, 3 });
			AddSkillRankRow(Table, TEXT("Data.Skill.Shield"), { 6, 0, 9, 0, 11, 11, 1, 0, 10, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 });
			AddSkillRankRow(Table, TEXT("Data.Skill.Parry"), { 8, 10, 0, 0, 10, 2, 7, 10, 7, 10, 0, 2, 2, 5, 0, 9, 2, 9, 4, 10, 10, 1 });
			AddSkillRankRow(Table, TEXT("Data.Skill.DivineMagic"), { 0, 0, 2, 0, 10, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 9, 0, 4 });
			AddSkillRankRow(Table, TEXT("Data.Skill.HealingMagic"), { 0, 0, 1, 0, 8, 0, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 9, 0, 0 });
			AddSkillRankRow(Table, TEXT("Data.Skill.EnhancingMagic"), { 0, 0, 6, 10, 3, 0, 9, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 9, 0, 5 });
			AddSkillRankRow(Table, TEXT("Data.Skill.EnfeeblingMagic"), { 0, 0, 7, 6, 1, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 9, 6, 0 });
			AddSkillRankRow(Table, TEXT("Data.Skill.ElementalMagic"), { 0, 0, 0, 1, 6, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 9, 3, 0 });
			AddSkillRankRow(Table, TEXT("Data.Skill.DarkMagic"), { 0, 0, 0, 2, 10, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 9, 7, 0 });
			AddSkillRankRow(Table, TEXT("Data.Skill.SummoningMagic"), { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0 });
			AddSkillRankRow(Table, TEXT("Data.Skill.Ninjutsu"), { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0 });
			AddSkillRankRow(Table, TEXT("Data.Skill.Singing"), { 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 });
			AddSkillRankRow(Table, TEXT("Data.Skill.StringInstrument"), { 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 });
			AddSkillRankRow(Table, TEXT("Data.Skill.WindInstrument"), { 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 });
			AddSkillRankRow(Table, TEXT("Data.Skill.BlueMagic"), { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0 });
			AddSkillRankRow(Table, TEXT("Data.Skill.Geomancy"), { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 0 });
			AddSkillRankRow(Table, TEXT("Data.Skill.Handbell"), { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 0 });
			EnsureAllSkillTagsPresent(Table);
			bInitialized = true;
		}

		return Table;
	}
}

const TArray<TArray<int32>> USkillRankStatics::SkillCapsByLevel = {
	{ { 0, 3, 3, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1 } },
	{ { 0, 6, 6, 5, 5, 5, 5, 5, 5, 4, 4, 4, 3 } },
	{ { 0, 9, 9, 7, 7, 7, 7, 7, 7, 6, 6, 6, 5 } },
	{ { 0, 12, 12, 10, 10, 10, 10, 10, 10, 9, 9, 8, 7 } },
	{ { 0, 15, 15, 13, 13, 13, 13, 13, 13, 12, 11, 10, 9 } },
	{ { 0, 18, 18, 16, 16, 16, 16, 16, 16, 14, 14, 13, 11 } },
	{ { 0, 21, 21, 19, 19, 19, 19, 19, 19, 17, 16, 15, 13 } },
	{ { 0, 24, 24, 22, 22, 22, 21, 21, 21, 20, 19, 17, 15 } },
	{ { 0, 27, 27, 25, 25, 25, 24, 24, 24, 22, 21, 20, 17 } },
	{ { 0, 30, 30, 28, 28, 28, 27, 27, 27, 25, 24, 22, 19 } },
	{ { 0, 33, 33, 31, 31, 31, 30, 30, 30, 28, 26, 24, 21 } },
	{ { 0, 36, 36, 34, 34, 34, 33, 33, 33, 31, 29, 27, 23 } },
	{ { 0, 39, 39, 36, 36, 36, 35, 35, 35, 33, 31, 29, 25 } },
	{ { 0, 42, 42, 39, 39, 39, 38, 38, 38, 36, 34, 31, 27 } },
	{ { 0, 45, 45, 42, 42, 42, 41, 41, 41, 39, 36, 33, 29 } },
	{ { 0, 48, 48, 45, 45, 45, 44, 44, 44, 41, 39, 36, 31 } },
	{ { 0, 51, 51, 48, 48, 48, 47, 47, 47, 44, 41, 38, 33 } },
	{ { 0, 54, 54, 51, 51, 51, 49, 49, 49, 47, 44, 40, 35 } },
	{ { 0, 57, 57, 54, 54, 54, 52, 52, 52, 49, 46, 43, 37 } },
	{ { 0, 60, 60, 57, 57, 57, 55, 55, 55, 52, 49, 45, 39 } },
	{ { 0, 63, 63, 60, 60, 60, 58, 58, 58, 55, 51, 47, 41 } },
	{ { 0, 66, 66, 63, 63, 63, 61, 61, 61, 58, 54, 50, 43 } },
	{ { 0, 69, 69, 65, 65, 65, 63, 63, 63, 60, 56, 52, 45 } },
	{ { 0, 72, 72, 68, 68, 68, 66, 66, 66, 63, 59, 54, 47 } },
	{ { 0, 75, 75, 71, 71, 71, 69, 69, 69, 66, 61, 56, 49 } },
	{ { 0, 78, 78, 74, 74, 74, 72, 72, 72, 68, 64, 59, 51 } },
	{ { 0, 81, 81, 77, 77, 77, 75, 75, 75, 71, 66, 61, 53 } },
	{ { 0, 84, 84, 80, 80, 80, 77, 77, 77, 74, 69, 63, 55 } },
	{ { 0, 87, 87, 83, 83, 83, 80, 80, 80, 76, 71, 66, 57 } },
	{ { 0, 90, 90, 86, 86, 86, 83, 83, 83, 79, 74, 68, 59 } },
	{ { 0, 93, 93, 89, 89, 89, 86, 86, 86, 82, 76, 70, 61 } },
	{ { 0, 96, 96, 92, 92, 92, 89, 89, 89, 85, 79, 73, 63 } },
	{ { 0, 99, 99, 94, 94, 94, 91, 91, 91, 87, 81, 75, 65 } },
	{ { 0, 102, 102, 97, 97, 97, 94, 94, 94, 90, 84, 77, 67 } },
	{ { 0, 105, 105, 100, 100, 100, 97, 97, 97, 93, 86, 79, 69 } },
	{ { 0, 108, 108, 103, 103, 103, 100, 100, 100, 95, 89, 82, 71 } },
	{ { 0, 111, 111, 106, 106, 106, 103, 103, 103, 98, 91, 84, 73 } },
	{ { 0, 114, 114, 109, 109, 109, 105, 105, 105, 101, 94, 86, 75 } },
	{ { 0, 117, 117, 112, 112, 112, 108, 108, 108, 103, 96, 89, 77 } },
	{ { 0, 120, 120, 115, 115, 115, 111, 111, 111, 106, 99, 91, 79 } },
	{ { 0, 123, 123, 118, 118, 118, 114, 114, 114, 109, 101, 93, 81 } },
	{ { 0, 126, 126, 121, 121, 121, 117, 117, 117, 112, 104, 96, 83 } },
	{ { 0, 129, 129, 123, 123, 123, 119, 119, 119, 114, 106, 98, 85 } },
	{ { 0, 132, 132, 126, 126, 126, 122, 122, 122, 117, 109, 100, 87 } },
	{ { 0, 135, 135, 129, 129, 129, 125, 125, 125, 120, 111, 102, 89 } },
	{ { 0, 138, 138, 132, 132, 132, 128, 128, 128, 122, 114, 105, 91 } },
	{ { 0, 141, 141, 135, 135, 135, 130, 130, 130, 125, 116, 107, 93 } },
	{ { 0, 144, 144, 138, 138, 138, 133, 133, 133, 128, 119, 109, 95 } },
	{ { 0, 147, 147, 141, 141, 141, 136, 136, 136, 130, 121, 112, 97 } },
	{ { 0, 150, 150, 144, 144, 144, 139, 139, 139, 133, 124, 114, 99 } },
	{ { 0, 153, 153, 147, 147, 147, 142, 142, 142, 136, 126, 116, 101 } },
	{ { 0, 158, 158, 151, 151, 151, 146, 146, 146, 140, 130, 120, 105 } },
	{ { 0, 163, 163, 156, 156, 156, 151, 151, 151, 145, 135, 124, 109 } },
	{ { 0, 168, 168, 161, 161, 161, 156, 156, 156, 150, 139, 128, 113 } },
	{ { 0, 173, 173, 166, 166, 166, 161, 161, 161, 154, 144, 133, 117 } },
	{ { 0, 178, 178, 171, 171, 171, 166, 166, 166, 159, 148, 137, 121 } },
	{ { 0, 183, 183, 176, 176, 176, 170, 170, 170, 164, 153, 141, 125 } },
	{ { 0, 188, 188, 181, 181, 181, 175, 175, 175, 168, 157, 146, 129 } },
	{ { 0, 193, 193, 186, 186, 186, 180, 180, 180, 173, 162, 150, 133 } },
	{ { 0, 198, 198, 191, 191, 191, 185, 185, 185, 178, 166, 154, 137 } },
	{ { 0, 203, 203, 196, 196, 196, 190, 190, 190, 183, 171, 159, 141 } },
	{ { 0, 207, 207, 199, 199, 198, 192, 192, 192, 184, 172, 161, 143 } },
	{ { 0, 212, 211, 203, 202, 201, 195, 194, 194, 186, 174, 163, 145 } },
	{ { 0, 217, 215, 207, 205, 204, 197, 196, 196, 188, 176, 165, 147 } },
	{ { 0, 222, 219, 210, 208, 206, 200, 199, 198, 190, 178, 167, 149 } },
	{ { 0, 227, 223, 214, 212, 209, 202, 201, 200, 192, 180, 169, 151 } },
	{ { 0, 232, 227, 218, 215, 212, 205, 203, 202, 194, 182, 171, 153 } },
	{ { 0, 236, 231, 221, 218, 214, 207, 205, 204, 195, 184, 173, 155 } },
	{ { 0, 241, 235, 225, 221, 217, 210, 208, 206, 197, 186, 175, 157 } },
	{ { 0, 246, 239, 229, 225, 220, 212, 210, 208, 199, 188, 177, 159 } },
	{ { 0, 251, 244, 233, 228, 223, 215, 212, 210, 201, 190, 179, 161 } },
	{ { 0, 256, 249, 237, 232, 226, 218, 214, 212, 202, 192, 181, 163 } },
	{ { 0, 261, 254, 242, 236, 229, 221, 217, 214, 204, 194, 183, 165 } },
	{ { 0, 266, 259, 246, 241, 233, 224, 219, 216, 206, 196, 185, 167 } },
	{ { 0, 271, 264, 251, 245, 236, 227, 222, 218, 208, 198, 187, 169 } },
	{ { 0, 276, 269, 256, 250, 240, 230, 225, 220, 210, 200, 189, 171 } },
	{ { 0, 281, 274, 261, 255, 245, 235, 230, 225, 214, 203, 191, 173 } },
	{ { 0, 286, 279, 266, 260, 250, 240, 235, 230, 218, 206, 193, 175 } },
	{ { 0, 291, 284, 271, 265, 255, 245, 240, 235, 222, 209, 195, 177 } },
	{ { 0, 296, 289, 276, 270, 260, 250, 245, 240, 226, 212, 197, 179 } },
	{ { 0, 301, 294, 281, 275, 265, 255, 250, 245, 230, 215, 199, 181 } },
	{ { 0, 307, 300, 287, 281, 271, 261, 256, 251, 235, 219, 202, 183 } },
	{ { 0, 313, 306, 293, 287, 277, 267, 262, 257, 240, 223, 205, 185 } },
	{ { 0, 319, 312, 299, 293, 283, 273, 268, 263, 245, 227, 208, 187 } },
	{ { 0, 325, 318, 305, 299, 289, 279, 274, 269, 250, 231, 211, 189 } },
	{ { 0, 331, 324, 311, 305, 295, 285, 280, 275, 255, 235, 214, 191 } },
	{ { 0, 337, 330, 317, 311, 301, 291, 286, 281, 260, 239, 217, 193 } },
	{ { 0, 343, 336, 323, 317, 307, 297, 292, 287, 265, 243, 220, 195 } },
	{ { 0, 349, 342, 329, 323, 313, 303, 298, 293, 270, 247, 223, 197 } },
	{ { 0, 355, 348, 335, 329, 319, 309, 304, 299, 275, 251, 226, 199 } },
	{ { 0, 361, 354, 341, 335, 325, 315, 310, 305, 280, 255, 229, 201 } },
	{ { 0, 368, 361, 348, 342, 332, 322, 317, 312, 286, 260, 233, 204 } },
	{ { 0, 375, 368, 355, 349, 339, 329, 324, 319, 292, 265, 237, 207 } },
	{ { 0, 382, 375, 362, 356, 346, 336, 331, 326, 298, 270, 241, 210 } },
	{ { 0, 389, 382, 369, 363, 353, 343, 338, 333, 304, 275, 245, 213 } },
	{ { 0, 396, 389, 376, 370, 360, 350, 345, 340, 310, 280, 249, 216 } },
	{ { 0, 403, 396, 383, 377, 367, 357, 352, 347, 316, 285, 253, 219 } },
	{ { 0, 410, 403, 390, 384, 374, 364, 359, 354, 322, 290, 257, 222 } },
	{ { 0, 417, 410, 397, 391, 381, 371, 366, 361, 328, 295, 261, 225 } },
	{ { 0, 424, 417, 404, 398, 388, 378, 373, 368, 334, 300, 265, 228 } },
};

ESkillRank USkillRankStatics::GetSkillRankForJob(EJob Job, FGameplayTag SkillTag)
{
	if (!SkillTag.IsValid())
	{
		return ESkillRank::None;
	}

	const TMap<FGameplayTag, TArray<uint8>>& Table = GetSkillRankTable();
	const TArray<uint8>* JobRanks = Table.Find(SkillTag);
	if (!JobRanks)
	{
		return ESkillRank::None;
	}

	const int32 JobIndex = static_cast<int32>(Job);
	if (!JobRanks->IsValidIndex(JobIndex))
	{
		return ESkillRank::None;
	}

	const int32 RankValue = FMath::Clamp(static_cast<int32>((*JobRanks)[JobIndex]), 0, MaxSkillRankIndex);
	return static_cast<ESkillRank>(RankValue);
}

int32 USkillRankStatics::GetSkillCapForLevelAndRank(int32 Level, ESkillRank Rank)
{
	const int32 ClampedLevel = FMath::Clamp(Level, 0, MaxSkillLevel);
	const int32 RankIndex = FMath::Clamp(static_cast<int32>(Rank), 0, MaxSkillRankIndex);
	const int32 OverLevelBonus = FMath::Clamp(Level - MaxSkillLevel, 0, MaxOverLevelBonus);

	return SkillCapsByLevel[ClampedLevel][RankIndex] + OverLevelBonus;
}

int32 USkillRankStatics::GetSkillCapForJobAndSkill(int32 Level, EJob Job, FGameplayTag SkillTag)
{
	const ESkillRank Rank = GetSkillRankForJob(Job, SkillTag);
	return GetSkillCapForLevelAndRank(Level, Rank);
}

void USkillRankStatics::TrySkillUp(ASignetPlayerCharacter* Player, FGameplayTag SkillTag, int32 VictimLevel, bool bForceSkillUp)
{
	if (!Player || SkillTag == FTagCache::Get().Skill.None)
	{
		return;
	}

	USignetSaveSubsystem* SaveSubsystem = Player->GetGameInstance()->GetSubsystem<USignetSaveSubsystem>();
	if (!SaveSubsystem)
	{
		return;
	}

	const EJob CurrentJob = SaveSubsystem->GetCurrentJob();
	const int32 CurrentJobLevel = SaveSubsystem->GetCurrentJobLevel();

	const float CurrentSkillRank = SaveSubsystem->GetSkillRank(SkillTag);
	const int32 CapSkill = GetSkillCapForJobAndSkill(CurrentJobLevel, CurrentJob, SkillTag);
	const int32 MaxSkill = GetSkillCapForJobAndSkill(FMath::Min(CurrentJobLevel, VictimLevel), CurrentJob, SkillTag);

	if (CurrentSkillRank >= CapSkill)
	{
		return;
	}

	const float Diff = MaxSkill - CurrentSkillRank;
	if (Diff <= 0 && !bForceSkillUp)
	{
		return;
	}

	// SkillUpChance = Diff / 5.0 + multiplier * (2.0 - log10(1.0 + CurSkill / 100))
	// Using default multiplier of 1.0
	float SkillUpChance = (Diff / 5.0f) + (2.0f - FMath::LogX(10.0f, 1.0f + CurrentSkillRank / 10.0f));
	SkillUpChance = FMath::Clamp(SkillUpChance, 0.0f, 0.5f);

	// Modifiers
	if (const USignetAbilitySystemComponent* Asc = Player->GetSignetAsc())
	{
		const FSkillTags& SkillTags = FTagCache::Get().Skill;
		// Determine if combat or magic skill (simplified mapping)
		bool bIsCombat = (SkillTag.MatchesTag(SkillTags.H2H) || SkillTag.MatchesTag(SkillTags.Dagger) || 
						  SkillTag.MatchesTag(SkillTags.Sword) || SkillTag.MatchesTag(SkillTags.GreatSword) ||
						  SkillTag.MatchesTag(SkillTags.Axe) || SkillTag.MatchesTag(SkillTags.GreatAxe) ||
						  SkillTag.MatchesTag(SkillTags.Scythe) || SkillTag.MatchesTag(SkillTags.Polearm) ||
						  SkillTag.MatchesTag(SkillTags.Katana) || SkillTag.MatchesTag(SkillTags.GreatKatana) ||
						  SkillTag.MatchesTag(SkillTags.Club) || SkillTag.MatchesTag(SkillTags.Staff) ||
						  SkillTag.MatchesTag(SkillTags.Archery) || SkillTag.MatchesTag(SkillTags.Marksmanship) ||
						  SkillTag.MatchesTag(SkillTags.Throwing) || SkillTag.MatchesTag(SkillTags.Shield));

		if (bIsCombat)
		{
			float RateMod = Asc->GetNumericAttribute(USignetCombatAttributeSet::GetCombatSkillUpRateAttribute());
			SkillUpChance *= ((100.0f + RateMod) / 100.0f);
		}
		else
		{
			// Assume magic for others for now
			float RateMod = Asc->GetNumericAttribute(USignetCombatAttributeSet::GetMagicSkillUpRateAttribute());
			SkillUpChance *= ((100.0f + RateMod) / 100.0f);
		}
	}

	if (bForceSkillUp || FMath::FRand() < SkillUpChance)
	{
		// Skill amount calculation (Legacy had tiers, we'll simplify to 0.1 - 0.5 range based on diff)
		float SkillAmount = 0.1f;
		int32 Tier = FMath::Clamp(FMath::FloorToInt(1 + (Diff / 5.0f)), 1, 5);
		
		float Roll = FMath::FRand();
		if (Tier >= 5 && Roll < 0.9f) SkillAmount = 0.5f;
		else if (Tier >= 4 && Roll < 0.7f) SkillAmount = 0.4f;
		else if (Tier >= 3 && Roll < 0.5f) SkillAmount = 0.3f;
		else if (Tier >= 2 && Roll < 0.3f) SkillAmount = 0.2f;

		// Ensure we don't overcap
		if (CurrentSkillRank + SkillAmount > CapSkill)
		{
			SkillAmount = CapSkill - CurrentSkillRank;
		}

		if (SkillAmount > 0.05f) // Using small epsilon to avoid floating point issues
		{
			float NewSkillRank = CurrentSkillRank + SkillAmount;
			SaveSubsystem->SetSkillRank(SkillTag, NewSkillRank);

			// Feedback
			if (UCombatTextComponent* CombatText = Player->FindComponentByClass<UCombatTextComponent>())
			{
				CombatText->AddSkillUpText(SkillTag, SkillAmount);
			}

			// Combat text feedback is handled by UCombatTextComponent.
		}
	}
}
