// Copyright 2025, Red Lotus Games


#include "SignetSkillAttributeSet.h"
#include "Net/UnrealNetwork.h"

#define DRAGON_SKILL_ATTRS(X) \
	X(Axe) \
	X(Archery) \
	X(Club) \
	X(Dagger) \
	X(Evasion) \
	X(GreatAxe) \
	X(GreatSword) \
	X(GreatKatana) \
	X(Guard) \
	X(HandToHand) \
	X(Katana) \
	X(Marksmanship) \
	X(Polearm) \
	X(Parrying) \
	X(Scythe) \
	X(Shield) \
	X(Staff) \
	X(Sword) \
	X(Throwing) \
	X(Blue) \
	X(Dark) \
	X(Divine) \
	X(Elemental) \
	X(Enfeebling) \
	X(Enhancing) \
	X(Geomancy) \
	X(Handbell) \
	X(Healing) \
	X(Summoning) \
	X(Ninjutsu) \
	X(Singing) \
	X(StringInstrument) \
	X(WindInstrument)

USignetSkillAttributeSet::USignetSkillAttributeSet()
{
	DRAGON_SKILL_ATTRS(DRAGON_INIT);
}

#define DRAGON_DEFINE(Name) DRAGON_DEFINE_ONREP(USignetSkillAttributeSet, Name)
DRAGON_SKILL_ATTRS(DRAGON_DEFINE);
#undef DRAGON_DEFINE

void USignetSkillAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

#define DRAGON_REP(Name) DRAGON_DO_REP(USignetSkillAttributeSet, Name)
	DRAGON_SKILL_ATTRS(DRAGON_REP);
#undef DRAGON_REP
}

#undef DRAGON_SKILL_ATTRS
