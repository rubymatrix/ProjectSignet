#pragma once

#include "AttributeSet.h"
#include "AbilitySystemComponent.h"

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
		GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
		GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
		GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
		GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)


#define DRAGON_DEFINE_ATTRIBUTE(ClassName, PropertyName, CategoryName) \
UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=CategoryName, ReplicatedUsing=OnRep_##PropertyName) \
FGameplayAttributeData PropertyName; \
GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName) \
UFUNCTION() \
void OnRep_##PropertyName(const FGameplayAttributeData& Old##PropertyName);

// Declares a replicated attribute with:
// - UPROPERTY (ReplicatedUsing)
// - ATTRIBUTE_ACCESSORS(Class, Prop)
// - OnRep_Prop(const FGameplayAttributeData& OldProp);
#define DRAGON_ATTRIBUTE(ClassName, PropertyName, CategoryPath) \
		UPROPERTY(BlueprintReadOnly, Category=CategoryPath, ReplicatedUsing=OnRep_##PropertyName) \
		FGameplayAttributeData PropertyName; \
		ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
		UFUNCTION() \
		void OnRep_##PropertyName(const FGameplayAttributeData& Old##PropertyName);

// Defines the OnRep body that forwards to GAMEPLAYATTRIBUTE_REPNOTIFY
#define DRAGON_DEFINE_ONREP(ClassName, PropertyName) \
void ClassName::OnRep_##PropertyName(const FGameplayAttributeData& Old##PropertyName) \
{ \
GAMEPLAYATTRIBUTE_REPNOTIFY(ClassName, PropertyName, Old##PropertyName); \
}

// Shorthand for the DOREPLIFETIME line inside GetLifetimeReplicatedProps
#define DRAGON_DO_REP(ClassName, PropertyName) \
DOREPLIFETIME_CONDITION_NOTIFY(ClassName, PropertyName, COND_None, REPNOTIFY_Always);

#define DRAGON_INIT(Name) Init##Name(0.f);