// Copyright Red Lotus Games, All Rights Reserved.


#include "AnimationTools.h"
#include "SignetGame/Player/Animation/ModularAnimData.h"
#include "SignetGame/Player/Animation/SignetPlayerAnimInstance.h"


void UAnimationTools::AssignAnimationsByRace(UObject* Object, const FGameplayTag& RaceTag)
{
	auto AnimBlueprint = Cast<UAnimBlueprint>(Object);
	if (!AnimBlueprint)
	{
		UE_LOG(LogTemp, Warning, TEXT("AssignCreatureAnimations: AnimBlueprint is null."));
		return;
	}
	
	if (UClass* GeneratedClass = AnimBlueprint->GeneratedClass)
	{
		if (USignetPlayerAnimInstance* DragonAnimCDO = Cast<USignetPlayerAnimInstance>(GeneratedClass->GetDefaultObject()))
		{
			AssignBasicAnimations(DragonAnimCDO, RaceTag);
			AssignCastingScheduleAnimations(DragonAnimCDO, RaceTag);
			
			AssignAnimationsByWeaponType(DragonAnimCDO, RaceTag, FTagCache::Get().Skill.None);
			AssignAnimationsByWeaponType(DragonAnimCDO, RaceTag, FTagCache::Get().Skill.H2H);
			AssignAnimationsByWeaponType(DragonAnimCDO, RaceTag, FTagCache::Get().Skill.Sword);
			AssignAnimationsByWeaponType(DragonAnimCDO, RaceTag, FTagCache::Get().Skill.Club);
			AssignAnimationsByWeaponType(DragonAnimCDO, RaceTag, FTagCache::Get().Skill.Axe);
			AssignAnimationsByWeaponType(DragonAnimCDO, RaceTag, FTagCache::Get().Skill.Dagger);
			AssignAnimationsByWeaponType(DragonAnimCDO, RaceTag, FTagCache::Get().Skill.Katana);
			AssignAnimationsByWeaponType(DragonAnimCDO, RaceTag, FTagCache::Get().Skill.Staff);
			AssignAnimationsByWeaponType(DragonAnimCDO, RaceTag, FTagCache::Get().Skill.GreatAxe);
			AssignAnimationsByWeaponType(DragonAnimCDO, RaceTag, FTagCache::Get().Skill.GreatSword);
			AssignAnimationsByWeaponType(DragonAnimCDO, RaceTag, FTagCache::Get().Skill.GreatKatana);
			AssignAnimationsByWeaponType(DragonAnimCDO, RaceTag, FTagCache::Get().Skill.Polearm);
			AssignAnimationsByWeaponType(DragonAnimCDO, RaceTag, FTagCache::Get().Skill.Scythe);
			AssignAnimationsByWeaponType(DragonAnimCDO, RaceTag, FTagCache::Get().Skill.DWCategory1);
			AssignAnimationsByWeaponType(DragonAnimCDO, RaceTag, FTagCache::Get().Skill.DWCategory2);
			AssignAnimationsByWeaponType(DragonAnimCDO, RaceTag, FTagCache::Get().Skill.DWCategory3);
			AssignAnimationsByWeaponType(DragonAnimCDO, RaceTag, FTagCache::Get().Skill.DWCategory4);
			AssignAnimationsByWeaponType(DragonAnimCDO, RaceTag, FTagCache::Get().Skill.DWSubCategory1);
			AssignAnimationsByWeaponType(DragonAnimCDO, RaceTag, FTagCache::Get().Skill.DWSubCategory2);
			AssignAnimationsByWeaponType(DragonAnimCDO, RaceTag, FTagCache::Get().Skill.DWSubCategory3);
			AssignAnimationsByWeaponType(DragonAnimCDO, RaceTag, FTagCache::Get().Skill.DWSubCategory4);

			// Mark dirty
			DragonAnimCDO->Modify();
			DragonAnimCDO->PostEditChange();

			// Mark blueprint package dirty
			UPackage* Package = AnimBlueprint->GetOutermost();
			if (Package)
			{
				Package->MarkPackageDirty();
			}

			UE_LOG(LogTemp, Log, TEXT("Assigned animations to AnimBlueprint: %s"), *AnimBlueprint->GetName());
		}
	}
	
}

void UAnimationTools::AssignBasicAnimations(USignetPlayerAnimInstance* AnimInstance,
	const FGameplayTag& RaceTag)
{
	const auto RaceKey = GetRacePathKey(RaceTag);
	const FString BasicAnimPath = FString::Printf(TEXT("/Game/Characters/Anims/%s/basic"), *RaceKey);
	const TArray Keys = {TEXT("idl"), TEXT("wlk"), TEXT("run"), TEXT("cor"), TEXT("rx1"), TEXT("si1"), TEXT("ri0")};

	auto LoadAnim = [&](const FString& Path)
	{
		return Cast<UAnimSequence>(StaticLoadObject(UAnimSequence::StaticClass(), nullptr, *Path)); 	
	};

	AnimInstance->Animations.Idle.Lower = LoadAnim(FString::Printf(TEXT("%s/idl0.idl0"), *BasicAnimPath));
	AnimInstance->Animations.Idle.Upper = LoadAnim(FString::Printf(TEXT("%s/idl1.idl1"), *BasicAnimPath));
	AnimInstance->Animations.Idle.Waist = LoadAnim(FString::Printf(TEXT("%s/idl2.idl2"), *BasicAnimPath));
	AnimInstance->Animations.Walking.Lower = LoadAnim(FString::Printf(TEXT("%s/wlk0.wlk0"), *BasicAnimPath));
	AnimInstance->Animations.Walking.Upper = LoadAnim(FString::Printf(TEXT("%s/wlk1.wlk1"), *BasicAnimPath));
	AnimInstance->Animations.Walking.Waist = LoadAnim(FString::Printf(TEXT("%s/wlk2.wlk2"), *BasicAnimPath));
	AnimInstance->Animations.Running.Lower = LoadAnim(FString::Printf(TEXT("%s/run0.run0"), *BasicAnimPath));
	AnimInstance->Animations.Running.Upper = LoadAnim(FString::Printf(TEXT("%s/run1.run1"), *BasicAnimPath));
	AnimInstance->Animations.Running.Waist = LoadAnim(FString::Printf(TEXT("%s/run2.run2"), *BasicAnimPath));
	AnimInstance->Animations.Dead.Lower = LoadAnim(FString::Printf(TEXT("%s/cor0.cor0"), *BasicAnimPath));
	AnimInstance->Animations.Dead.Upper = LoadAnim(FString::Printf(TEXT("%s/cor1.cor1"), *BasicAnimPath));
	AnimInstance->Animations.Dead.Waist = LoadAnim(FString::Printf(TEXT("%s/cor2.cor2"), *BasicAnimPath));
	AnimInstance->Animations.Crafting.Lower = LoadAnim(FString::Printf(TEXT("%s/ri00.ri00"), *BasicAnimPath));
	AnimInstance->Animations.Crafting.Upper = LoadAnim(FString::Printf(TEXT("%s/ri01.ri01"), *BasicAnimPath));
	AnimInstance->Animations.Crafting.Waist = LoadAnim(FString::Printf(TEXT("%s/ri02.ri02"), *BasicAnimPath));
	AnimInstance->Animations.Resting.Lower = LoadAnim(FString::Printf(TEXT("%s/rx10.rx10"), *BasicAnimPath));
	AnimInstance->Animations.Resting.Upper = LoadAnim(FString::Printf(TEXT("%s/rx11.rx11"), *BasicAnimPath));
	AnimInstance->Animations.Resting.Waist = LoadAnim(FString::Printf(TEXT("%s/rx12.rx12"), *BasicAnimPath));
	AnimInstance->Animations.Sitting.Lower = LoadAnim(FString::Printf(TEXT("%s/si10.si10"), *BasicAnimPath));
	AnimInstance->Animations.Sitting.Upper = LoadAnim(FString::Printf(TEXT("%s/si11.si11"), *BasicAnimPath));
	AnimInstance->Animations.Sitting.Waist = LoadAnim(FString::Printf(TEXT("%s/si12.si12"), *BasicAnimPath));
}

void UAnimationTools::AssignCastingScheduleAnimations(USignetPlayerAnimInstance* AnimInstance,
	const FGameplayTag& RaceTag)
{
	const auto RaceKey = GetRacePathKey(RaceTag);
	const FString BasicAnimPath = FString::Printf(TEXT("/Game/Characters/Anims/%s/basic"), *RaceKey);
	
	const TMap<FGameplayTag, FString> CastingSchedules = {
		{ FTagCache::Get().Casting.Ability, TEXT("cm1") },
		{ FTagCache::Get().Casting.Item, TEXT("mi1") },
		{ FTagCache::Get().Casting.BlackMagic, TEXT("mb0") },
		{ FTagCache::Get().Casting.WhiteMagic, TEXT("mw0") },
		{ FTagCache::Get().Casting.Faith, TEXT("ms0") },
		{ FTagCache::Get().Casting.Geomancy, TEXT("gc1") },
		{ FTagCache::Get().Casting.Summoning, TEXT("ms0") },
		{ FTagCache::Get().Casting.BlueMagic, TEXT("ma0") },
		{ FTagCache::Get().Casting.Ninjutsu, TEXT("mn0") },
		{ FTagCache::Get().Casting.Song, TEXT("sk1") },
		{ FTagCache::Get().Casting.SongString, TEXT("sh1") },
		{ FTagCache::Get().Casting.SongWind, TEXT("sf1") },
		{ FTagCache::Get().Casting.ShootBow, TEXT("yu1") },
		{ FTagCache::Get().Casting.ShootGun, TEXT("gu1") },
		{ FTagCache::Get().Casting.ShootThrown, TEXT("na1") }
	};

	auto LoadAnim = [&](const FString& Path)
	{
		return Cast<UAnimSequence>(StaticLoadObject(UAnimSequence::StaticClass(), nullptr, *Path)); 	
	};

	for (auto& [Tag, AnimCode] : CastingSchedules)
	{
		const auto  Lower = FString::Printf(TEXT("%s/%s0.%s0"), *BasicAnimPath, *AnimCode, *AnimCode);
		const auto  Upper = FString::Printf(TEXT("%s/%s1.%s1"), *BasicAnimPath, *AnimCode, *AnimCode);
		const auto  Waist = FString::Printf(TEXT("%s/%s2.%s2"), *BasicAnimPath, *AnimCode, *AnimCode);

		FLayeredSequence Schedule = {
			LoadAnim(Lower),
			LoadAnim(Upper),
			LoadAnim(Waist)
		};
		
		if (!AnimInstance->CastingAnims.Find(Tag))
		{
			AnimInstance->CastingAnims.Add(Tag, Schedule);
		}
		else
		{
			AnimInstance->CastingAnims[Tag] = Schedule;
		}
	}
}

void UAnimationTools::AssignAnimationsByWeaponType(USignetPlayerAnimInstance* AnimInstance, const FGameplayTag& RaceTag, const FGameplayTag& WeaponType)
{

	FCharacterAnimData AnimData = GetAnimDataForRace(RaceTag);
	const auto WeaponKey = AnimData.GetKey(WeaponType);
	if (WeaponKey.IsEmpty()) return;

	const auto RaceKey = GetRacePathKey(RaceTag);

	FString BattleKey = "battle_";
	if (WeaponType.ToString().Contains("DWCategory"))
	{
		BattleKey = "dual_wield_main_";
	}
	else if (WeaponType.ToString().Contains("DWSubCategory"))
	{
		BattleKey = "dual_wield_sub_";
	}

	const FString BasicAnimPath = FString::Printf(TEXT("/Game/Characters/Anims/%s/basic"), *RaceKey);
	const FString BaseObjectPath = FString::Printf(TEXT("/Game/Characters/Anims/%s/battle/%s%s"), *RaceKey, *BattleKey, *WeaponKey);

	TMap<FString, TArray<FString>> AnimMap = {
		{ TEXT("cor"), {
			FString::Printf(TEXT("%s/cor0.cor0"), *BasicAnimPath),
			FString::Printf(TEXT("%s/cor1.cor1"), *BaseObjectPath),
			FString::Printf(TEXT("%s/cor2.cor2"), *BasicAnimPath),
		}},
		{ TEXT("btl"), {
			FString::Printf(TEXT("%s/btl0.btl0"), *BaseObjectPath),
			FString::Printf(TEXT("%s/btl1.btl1"), *BaseObjectPath),
			FString::Printf(TEXT("%s/btl2.btl2"), *BaseObjectPath),
		}},
		{ TEXT("wlk"), {
			FString::Printf(TEXT("%s/wlk0.wlk0"), *BasicAnimPath),
			FString::Printf(TEXT("%s/wlk1.wlk1"), *BaseObjectPath),
			FString::Printf(TEXT("%s/wlk2.wlk2"), *BasicAnimPath),
		}},
		{ TEXT("run"), {
			FString::Printf(TEXT("%s/run0.run0"), *BasicAnimPath),
			FString::Printf(TEXT("%s/run1.run1"), *BaseObjectPath),
			FString::Printf(TEXT("%s/run2.run2"), *BasicAnimPath),
		}},
		{ TEXT("mvr"), {
			FString::Printf(TEXT("%s/mvr0.mvr0"), *BasicAnimPath),
			FString::Printf(TEXT("%s/mvr1.mvr1"), *BaseObjectPath),
			FString::Printf(TEXT("%s/mvr2.mvr2"), *BasicAnimPath),
		}},
		{ TEXT("mvl"), {
			FString::Printf(TEXT("%s/mvl0.mvl0"), *BasicAnimPath),
			FString::Printf(TEXT("%s/mvl1.mvl1"), *BaseObjectPath),
			FString::Printf(TEXT("%s/mvl2.mvl2"), *BasicAnimPath),
		}},
		{ TEXT("mvb"), {
			FString::Printf(TEXT("%s/mvb0.mvb0"), *BasicAnimPath),
			FString::Printf(TEXT("%s/mvb1.mvb1"), *BaseObjectPath),
			FString::Printf(TEXT("%s/mvb2.mvb2"), *BasicAnimPath),
		}}
	};

	auto LoadAnim = [&](const FString& Path)
	{
		return Cast<UAnimSequence>(StaticLoadObject(UAnimSequence::StaticClass(), nullptr, *Path)); 	
	};

	// Make sure it exists
	if (!AnimInstance->WeaponOverlayAnims.Find(WeaponType))
	{
		AnimInstance->WeaponOverlayAnims.Add(WeaponType, {});
	}

	// Assign the base engaged anims for no weapon type
	if (WeaponType == FTagCache::Get().Skill.None)
	{
		AnimInstance->Animations.Engaged.Lower = LoadAnim(AnimMap["btl"][0]);
		AnimInstance->Animations.Engaged.Upper = LoadAnim(AnimMap["btl"][1]);
		AnimInstance->Animations.Engaged.Waist = LoadAnim(AnimMap["btl"][2]);
	}
	
	if (AnimInstance->WeaponOverlayAnims.Find(WeaponType))
	{
		AnimInstance->WeaponOverlayAnims[WeaponType].Dead.Lower = LoadAnim(AnimMap["cor"][0]);
		AnimInstance->WeaponOverlayAnims[WeaponType].Dead.Upper = LoadAnim(AnimMap["cor"][1]);
		AnimInstance->WeaponOverlayAnims[WeaponType].Dead.Waist = LoadAnim(AnimMap["cor"][2]);
		AnimInstance->WeaponOverlayAnims[WeaponType].Idle.Lower = LoadAnim(AnimMap["btl"][0]);
		AnimInstance->WeaponOverlayAnims[WeaponType].Idle.Upper = LoadAnim(AnimMap["btl"][1]);
		AnimInstance->WeaponOverlayAnims[WeaponType].Idle.Waist = LoadAnim(AnimMap["btl"][2]);
		AnimInstance->WeaponOverlayAnims[WeaponType].Walking.Lower = LoadAnim(AnimMap["wlk"][0]);
		AnimInstance->WeaponOverlayAnims[WeaponType].Walking.Upper = LoadAnim(AnimMap["wlk"][1]);
		AnimInstance->WeaponOverlayAnims[WeaponType].Walking.Waist = LoadAnim(AnimMap["wlk"][2]);
		AnimInstance->WeaponOverlayAnims[WeaponType].Running.Lower = LoadAnim(AnimMap["run"][0]);
		AnimInstance->WeaponOverlayAnims[WeaponType].Running.Upper = LoadAnim(AnimMap["run"][1]);
		AnimInstance->WeaponOverlayAnims[WeaponType].Running.Waist = LoadAnim(AnimMap["run"][2]);
		// AnimInstance->WeaponOverlayAnims[WeaponType].MoveRight.Lower = LoadAnim(AnimMap["mvr"][0]);
		// AnimInstance->WeaponOverlayAnims[WeaponType].MoveRight.Upper = LoadAnim(AnimMap["mvr"][1]);
		// AnimInstance->WeaponOverlayAnims[WeaponType].MoveRight.Waist = LoadAnim(AnimMap["mvr"][2]);
		// AnimInstance->WeaponOverlayAnims[WeaponType].MoveLeft.Lower = LoadAnim(AnimMap["mvl"][0]);
		// AnimInstance->WeaponOverlayAnims[WeaponType].MoveLeft.Upper = LoadAnim(AnimMap["mvl"][1]);
		// AnimInstance->WeaponOverlayAnims[WeaponType].MoveLeft.Waist = LoadAnim(AnimMap["mvl"][2]);
		// AnimInstance->WeaponOverlayAnims[WeaponType].MoveBack.Lower = LoadAnim(AnimMap["mvb"][0]);
		// AnimInstance->WeaponOverlayAnims[WeaponType].MoveBack.Upper = LoadAnim(AnimMap["mvb"][1]);
		// AnimInstance->WeaponOverlayAnims[WeaponType].MoveBack.Waist = LoadAnim(AnimMap["mvb"][2]);
	}
}

FString UAnimationTools::GetRacePathKey(const FGameplayTag& RaceTag)
{
	if (RaceTag == FTagCache::Get().Race.HumeMale) return TEXT("hume_male");
	if (RaceTag == FTagCache::Get().Race.HumeFemale) return TEXT("hume_female");
	if (RaceTag == FTagCache::Get().Race.ElvaanMale) return TEXT("elvaan_male");
	if (RaceTag == FTagCache::Get().Race.ElvaanFemale) return TEXT("elvaan_female");
	if (RaceTag == FTagCache::Get().Race.Mithra) return TEXT("mithra");
	if (RaceTag == FTagCache::Get().Race.TarutaruMale || RaceTag == FTagCache::Get().Race.TarutaruFemale) return TEXT("tarutaru");
	if (RaceTag == FTagCache::Get().Race.Galka) return TEXT("galka");
	
	return "";
}

FCharacterAnimData UAnimationTools::GetAnimDataForRace(const FGameplayTag& RaceTag)
{
	FCharacterAnimData HumeMale {
		TEXT("32-21"),
		TEXT("32-15"),
		TEXT("32-13"),
		TEXT("32-13"),
		TEXT("32-14"),
		TEXT("32-16"),
		TEXT("98-55"),
		TEXT("32-20"),
		TEXT("32-17"),
		TEXT("32-18"),
		TEXT("32-18"),
		TEXT("32-20"),
		TEXT("32-19"),
		TEXT("108-112"),
		TEXT("108-113"),
		TEXT("108-114"),
		TEXT("108-115"),
		TEXT("108-100"),
		TEXT("108-101"),
		TEXT("108-102"),
		TEXT("108-103")
	};

	FCharacterAnimData HumeFemale {
		TEXT("36-124"),
		TEXT("36-120"),
		TEXT("36-118"),
		TEXT("36-118"),
		TEXT("36-117"),
		TEXT("36-119"),
		TEXT("98-86"),
		TEXT("36-123"),
		TEXT("36-121"),
		TEXT("36-122"),
		TEXT("36-122"),
		TEXT("36-123"),
		TEXT("36-121"),
		TEXT("108-112"),
		TEXT("108-113"),
		TEXT("108-114"),
		TEXT("108-115"),
		TEXT("108-100"),
		TEXT("108-101"),
		TEXT("108-102"),
		TEXT("108-103")
	};

	FCharacterAnimData ElvaanMale {
		TEXT("41-93"),
		TEXT("41-84"),
		TEXT("41-85"),
		TEXT("41-85"),
		TEXT("41-86"),
		TEXT("41-92"),
		TEXT("98-117"),
		TEXT("41-91"),
		TEXT("41-87"),
		TEXT("41-88"),
		TEXT("41-90"),
		TEXT("41-91"),
		TEXT("41-89"),
		TEXT("108-112"),
		TEXT("108-113"),
		TEXT("108-114"),
		TEXT("108-115"),
		TEXT("108-100"),
		TEXT("108-101"),
		TEXT("108-102"),
		TEXT("108-103")
	};

	FCharacterAnimData ElvaanFemale {
		TEXT("46-62"),
		TEXT("46-58"),
		TEXT("46-57"),
		TEXT("46-57"),
		TEXT("46-57"),
		TEXT("46-57"),
		TEXT("99-20"),
		TEXT("46-60"),
		TEXT("46-59"),
		TEXT("46-61"),
		TEXT("46-61"),
		TEXT("46-60"),
		TEXT("99-21"),
		TEXT("108-112"),
		TEXT("108-113"),
		TEXT("108-114"),
		TEXT("108-115"),
		TEXT("108-100"),
		TEXT("108-101"),
		TEXT("108-102"),
		TEXT("108-103")
	};

	FCharacterAnimData Tarutaru {
		TEXT("51-24"),
		TEXT("51-24"),
		TEXT("51-22"),
		TEXT("51-22"),
		TEXT("51-22"),
		TEXT("51-20"),
		TEXT("99-55"),
		TEXT("51-23"),
		TEXT("51-21"),
		TEXT("51-21"),
		TEXT("51-21"),
		TEXT("51-23"),
		TEXT("51-21"),
		TEXT("108-112"),
		TEXT("108-113"),
		TEXT("108-114"),
		TEXT("108-115"),
		TEXT("108-100"),
		TEXT("108-101"),
		TEXT("108-102"),
		TEXT("108-103")
	};

	FCharacterAnimData Galka {
		TEXT("60-112"),
		TEXT("60-119"),
		TEXT("60-114"),
		TEXT("60-114"),
		TEXT("60-114"),
		TEXT("60-113"),
		TEXT("99-117"),
		TEXT("60-116"),
		TEXT("60-115"),
		TEXT("60-117"),
		TEXT("60-118"),
		TEXT("60-116"),
		TEXT("99-118"),
		TEXT("108-112"),
		TEXT("108-113"),
		TEXT("108-114"),
		TEXT("108-115"),
		TEXT("108-100"),
		TEXT("108-101"),
		TEXT("108-102"),
		TEXT("108-103")
	};

	FCharacterAnimData Mithra {
		TEXT("56-22"),
		TEXT("56-15"),
		TEXT("56-16"),
		TEXT("56-16"),
		TEXT("56-21"),
		TEXT("56-14"),
		TEXT("99-86"),
		TEXT("56-19"),
		TEXT("56-17"),
		TEXT("56-20"),
		TEXT("56-18"),
		TEXT("56-19"),
		TEXT("56-17"),
		TEXT("108-112"),
		TEXT("108-113"),
		TEXT("108-114"),
		TEXT("108-115"),
		TEXT("108-100"),
		TEXT("108-101"),
		TEXT("108-102"),
		TEXT("108-103")
	};
	
	if (RaceTag == FTagCache::Get().Race.HumeMale) return HumeMale;
	if (RaceTag == FTagCache::Get().Race.HumeFemale) return HumeFemale;
	if (RaceTag == FTagCache::Get().Race.ElvaanMale) return ElvaanMale;
	if (RaceTag == FTagCache::Get().Race.ElvaanFemale) return ElvaanFemale;
	if (RaceTag == FTagCache::Get().Race.TarutaruFemale || RaceTag == FTagCache::Get().Race.TarutaruMale) return Tarutaru;
	if (RaceTag == FTagCache::Get().Race.Galka) return Galka;
	if (RaceTag == FTagCache::Get().Race.Mithra) return Mithra;

	return {};
}