#pragma once
#include "GameplayTagContainer.h"

struct FCastingTags
{
	FGameplayTag BlackMagic;
	FGameplayTag WhiteMagic;
	FGameplayTag BlueMagic;
	FGameplayTag Faith;
	FGameplayTag Summoning;
	FGameplayTag Geomancy;
	FGameplayTag Song;
	FGameplayTag SongWind;
	FGameplayTag SongString;
	FGameplayTag Item;
	FGameplayTag ShootGun;
	FGameplayTag ShootBow;
	FGameplayTag ShootThrown;
	FGameplayTag Ability;
	FGameplayTag Ninjutsu;
	TArray<FGameplayTag> AllTags{};
	
	void Initialize()
	{
		BlackMagic = FGameplayTag::RequestGameplayTag(TEXT("Data.Casting.BlackMagic"));
		WhiteMagic = FGameplayTag::RequestGameplayTag(TEXT("Data.Casting.WhiteMagic"));
		BlueMagic = FGameplayTag::RequestGameplayTag(TEXT("Data.Casting.BlueMagic"));
		Faith = FGameplayTag::RequestGameplayTag(TEXT("Data.Casting.Faith"));
		Summoning = FGameplayTag::RequestGameplayTag(TEXT("Data.Casting.Summoning"));
		Geomancy = FGameplayTag::RequestGameplayTag(TEXT("Data.Casting.Geomancy"));
		Song = FGameplayTag::RequestGameplayTag(TEXT("Data.Casting.Song.None"));
		SongWind = FGameplayTag::RequestGameplayTag(TEXT("Data.Casting.Song.Wind"));
		SongString = FGameplayTag::RequestGameplayTag(TEXT("Data.Casting.Song.String"));
		Item = FGameplayTag::RequestGameplayTag(TEXT("Data.Casting.Item"));
		ShootGun = FGameplayTag::RequestGameplayTag(TEXT("Data.Casting.Shoot.Gun"));
		ShootBow = FGameplayTag::RequestGameplayTag(TEXT("Data.Casting.Shoot.Bow"));
		ShootThrown = FGameplayTag::RequestGameplayTag(TEXT("Data.Casting.Shoot.Thrown"));
		Ability = FGameplayTag::RequestGameplayTag(TEXT("Data.Casting.Ability"));
		Ninjutsu = FGameplayTag::RequestGameplayTag(TEXT("Data.Casting.Ninjutsu"));

		AllTags = {
			BlackMagic, WhiteMagic, BlueMagic, Faith, Summoning, Geomancy,
			Song, SongWind, SongString, Item, ShootGun, ShootBow, ShootThrown,
			Ability, Ninjutsu
		};
	}
};