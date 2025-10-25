#pragma once

#include "Tags/StateTags.h"
#include "Tags/SkillTags.h"
#include "Tags/CastingTags.h"
#include "Tags/EffectTags.h"
#include "Tags/AbilityTags.h"
#include "Tags/DataTags.h"
#include "Tags/MoveDirections.h"

struct FTagCache
{
	FStateTags State;
	FSkillTags Skill;
	FCastingTags Casting;
	FEffectTags Effect;
	FAbilityTags Ability;
	FDataTags Data;
	FMoveDirectionTags Movement;
	
	static const FTagCache& Get()
	{
		static FTagCache Instance;
		static bool bInitialized = false;

		if (!bInitialized)
		{
			Instance.State.Initialize();
			Instance.Skill.Initialize();
			Instance.Casting.Initialize();
			Instance.Effect.Initialize();
			Instance.Ability.Initialize();
			Instance.Data.Initialize();
			Instance.Movement.Initialize();
			bInitialized = true;
		}
		
		return Instance;
	}
};
