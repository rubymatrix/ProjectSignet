// Copyright 2025, Red Lotus Games


#include "SignetPrimaryAttributeSet.h"
#include "Net/UnrealNetwork.h"
#include "SignetGame/Save/SignetSaveTypes.h"

#define DRAGON_PRIMARY_ATTRS(X) \
	X(Job) \
	X(SubJob) \
	X(JobLevel) \
	X(SubJobLevel) \
	X(STR) \
	X(DEX) \
	X(VIT) \
	X(AGI) \
	X(INT) \
	X(MND) \
	X(CHR) \
	X(HP) \
	X(MaxHP) \
	X(MP) \
	X(MaxMP) \
	X(TP) \
	X(MaxTP) \
	X(MoveSpeed)

USignetPrimaryAttributeSet::USignetPrimaryAttributeSet()
{
}

void USignetPrimaryAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	if (Attribute == GetJobAttribute() || Attribute == GetSubJobAttribute())
	{
		// Clamp job and subjob to valid range
		NewValue = FMath::Clamp(NewValue, 0.f, static_cast<float>(EJob::WhiteMage));
	}

	if (Attribute == GetJobLevelAttribute() || Attribute == GetSubJobLevelAttribute())
	{
		// Clamp job levels to valid range
		NewValue = FMath::Clamp(NewValue, 1.f, 99.f);
	}

	if (const TArray StatAttributes = { GetSTRAttribute(), GetDEXAttribute(), GetVITAttribute(), GetAGIAttribute(), GetINTAttribute(), GetMNDAttribute(), GetCHRAttribute() };
		StatAttributes.Contains(Attribute))
	{
		// Clamp stats to valid range
		NewValue = FMath::Clamp(NewValue, 1.f, 999.f);
	}

	if (const TArray ParamAttributes = { GetHPAttribute(), GetMaxHPAttribute(), GetMPAttribute(), GetMaxMPAttribute() };
		ParamAttributes.Contains(Attribute))
	{
		NewValue = FMath::Clamp(NewValue, 0.f, 9999.f);
	}

	if (Attribute == GetTPAttribute() || Attribute == GetMaxTPAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, 3000.f);
	}
	
	Super::PreAttributeChange(Attribute, NewValue);
}

#define DRAGON_DEFINE(Name) DRAGON_DEFINE_ONREP(USignetPrimaryAttributeSet, Name)
DRAGON_PRIMARY_ATTRS(DRAGON_DEFINE);
#undef DRAGON_DEFINE

void USignetPrimaryAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
#define DRAGON_REP(Name) DRAGON_DO_REP(USignetPrimaryAttributeSet, Name)
	DRAGON_PRIMARY_ATTRS(DRAGON_REP);
#undef DRAGON_REP
}

#undef DRAGON_PRIMARY_ATTRS