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
	
}