// Copyright Red Lotus Games, All Rights Reserved.

#pragma once
#include "CoreMinimal.h"
#include "SignetGame/Save/SignetSaveTypes.h"

namespace Data
{
	static FName GetRowNameFromEnum(ERace Value)
	{
		return StaticEnum<ERace>()->GetNameByValue(static_cast<int64>(Value));
	}
	
	static ERace GetRaceFromRowName(const FName& RowName)
	{
		const int64 Raw = StaticEnum<ERace>()->GetValueByName(RowName);
		return Raw != INDEX_NONE ? static_cast<ERace>(Raw) : ERace::HumeMale;
	}

	static int GetVocalIndexFromFaceType(const EFace& Face)
	{
		switch (Face)
		{
		case EFace::F1A:
		case EFace::F1B: return 0;
		case EFace::F2A:
		case EFace::F2B: return 1;
		case EFace::F3A:
		case EFace::F3B: return 2;
		case EFace::F4A:
		case EFace::F4B: return 3;
		case EFace::F5A:
		case EFace::F5B: return 4;
		case EFace::F6A:
		case EFace::F6B: return 5;
		case EFace::F7A:
		case EFace::F7B: return 6;
		case EFace::F8A:
		case EFace::F8B: return 7;
		default: return 0;
		}
	}
	
}