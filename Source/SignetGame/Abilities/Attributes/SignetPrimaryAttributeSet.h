// Copyright 2025, Red Lotus Games

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AttributeMacros.h"
#include "AbilitySystemComponent.h"
#include "Abilities/Attributes/GSCAttributeSetBase.h"
#include "SignetPrimaryAttributeSet.generated.h"

UCLASS()
class SIGNETGAME_API USignetPrimaryAttributeSet : public UGSCAttributeSetBase
{
	GENERATED_BODY()

public:

	USignetPrimaryAttributeSet();


	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
	
	UPROPERTY(BlueprintReadOnly, Category="Stats|Primary", ReplicatedUsing=OnRep_Job)
	FGameplayAttributeData Job;
	ATTRIBUTE_ACCESSORS(USignetPrimaryAttributeSet, Job);
	UFUNCTION()
	void OnRep_Job(const FGameplayAttributeData& OldJob);

	UPROPERTY(BlueprintReadOnly, Category="Stats|Primary", ReplicatedUsing=OnRep_SubJob)
	FGameplayAttributeData SubJob;
	ATTRIBUTE_ACCESSORS(USignetPrimaryAttributeSet, SubJob);
	UFUNCTION()
	void OnRep_SubJob(const FGameplayAttributeData& OldSubJob);
	
	// Job Levels
	UPROPERTY(BlueprintReadOnly, Category="Stats|Primary", ReplicatedUsing=OnRep_JobLevel)
	FGameplayAttributeData JobLevel;
	ATTRIBUTE_ACCESSORS(USignetPrimaryAttributeSet, JobLevel);
	UFUNCTION()
	void OnRep_JobLevel(const FGameplayAttributeData& OldJobLevel);

	UPROPERTY(BlueprintReadOnly, Category="Stats|Primary", ReplicatedUsing=OnRep_SubJobLevel)
	FGameplayAttributeData SubJobLevel;
	ATTRIBUTE_ACCESSORS(USignetPrimaryAttributeSet, SubJobLevel);
	UFUNCTION()
	void OnRep_SubJobLevel(const FGameplayAttributeData& OldSubJobLevel);

	// Core Attributes
	UPROPERTY(BlueprintReadOnly, Category="Stats|Primary", ReplicatedUsing=OnRep_STR)
	FGameplayAttributeData STR;
	ATTRIBUTE_ACCESSORS(USignetPrimaryAttributeSet, STR);
	UFUNCTION()
	void OnRep_STR(const FGameplayAttributeData& OldSTR);

	UPROPERTY(BlueprintReadOnly, Category="Stats|Primary", ReplicatedUsing=OnRep_DEX)
	FGameplayAttributeData DEX;
	ATTRIBUTE_ACCESSORS(USignetPrimaryAttributeSet, DEX);
	UFUNCTION()
	void OnRep_DEX(const FGameplayAttributeData& OldDEX);

	UPROPERTY(BlueprintReadOnly, Category="Stats|Primary", ReplicatedUsing=OnRep_VIT)
	FGameplayAttributeData VIT;
	ATTRIBUTE_ACCESSORS(USignetPrimaryAttributeSet, VIT);
	UFUNCTION()
	void OnRep_VIT(const FGameplayAttributeData& OldVIT);

	UPROPERTY(BlueprintReadOnly, Category="Stats|Primary", ReplicatedUsing=OnRep_AGI)
	FGameplayAttributeData AGI;
	ATTRIBUTE_ACCESSORS(USignetPrimaryAttributeSet, AGI);
	UFUNCTION()
	void OnRep_AGI(const FGameplayAttributeData& OldAGI);

	UPROPERTY(BlueprintReadOnly, Category="Stats|Primary", ReplicatedUsing=OnRep_INT)
	FGameplayAttributeData INT;
	ATTRIBUTE_ACCESSORS(USignetPrimaryAttributeSet, INT);
	UFUNCTION()
	void OnRep_INT(const FGameplayAttributeData& OldINT);

	UPROPERTY(BlueprintReadOnly, Category="Stats|Primary", ReplicatedUsing=OnRep_MND)
	FGameplayAttributeData MND;
	ATTRIBUTE_ACCESSORS(USignetPrimaryAttributeSet, MND);
	UFUNCTION()
	void OnRep_MND(const FGameplayAttributeData& OldMND);

	UPROPERTY(BlueprintReadOnly, Category="Stats|Primary", ReplicatedUsing=OnRep_CHR)
	FGameplayAttributeData CHR;
	ATTRIBUTE_ACCESSORS(USignetPrimaryAttributeSet, CHR);
	UFUNCTION()
	void OnRep_CHR(const FGameplayAttributeData& OldCHR);

	// Core Parameters
	UPROPERTY(BlueprintReadOnly, Category="Stats|Primary", ReplicatedUsing=OnRep_HP)
	FGameplayAttributeData HP;
	ATTRIBUTE_ACCESSORS(USignetPrimaryAttributeSet, HP);
	UFUNCTION()
	void OnRep_HP(const FGameplayAttributeData& OldHP);

	UPROPERTY(BlueprintReadOnly, Category="Stats|Primary", ReplicatedUsing=OnRep_MaxHP)
	FGameplayAttributeData MaxHP;
	ATTRIBUTE_ACCESSORS(USignetPrimaryAttributeSet, MaxHP);
	UFUNCTION()
	void OnRep_MaxHP(const FGameplayAttributeData& OldMaxHP);

	UPROPERTY(BlueprintReadOnly, Category="Stats|Primary", ReplicatedUsing=OnRep_MP)
	FGameplayAttributeData MP;
	ATTRIBUTE_ACCESSORS(USignetPrimaryAttributeSet, MP);
	UFUNCTION()
	void OnRep_MP(const FGameplayAttributeData& OldMP);

	UPROPERTY(BlueprintReadOnly, Category="Stats|Primary", ReplicatedUsing=OnRep_MaxMP)
	FGameplayAttributeData MaxMP;
	ATTRIBUTE_ACCESSORS(USignetPrimaryAttributeSet, MaxMP);
	UFUNCTION()
	void OnRep_MaxMP(const FGameplayAttributeData& OldMaxMP);

	UPROPERTY(BlueprintReadOnly, Category="Stats|Primary", ReplicatedUsing=OnRep_TP)
	FGameplayAttributeData TP;
	ATTRIBUTE_ACCESSORS(USignetPrimaryAttributeSet, TP);
	UFUNCTION()
	void OnRep_TP(const FGameplayAttributeData& OldTP);

	UPROPERTY(BlueprintReadOnly, Category="Stats|Primary", ReplicatedUsing=OnRep_MaxTP)
	FGameplayAttributeData MaxTP;
	ATTRIBUTE_ACCESSORS(USignetPrimaryAttributeSet, MaxTP);
	UFUNCTION()
	void OnRep_MaxTP(const FGameplayAttributeData& OldMaxTP);

	UPROPERTY(BlueprintReadOnly, Category="Stats|Primary", ReplicatedUsing=OnRep_MoveSpeed)
	FGameplayAttributeData MoveSpeed;
	ATTRIBUTE_ACCESSORS(USignetPrimaryAttributeSet, MoveSpeed);
	UFUNCTION()
	void OnRep_MoveSpeed(const FGameplayAttributeData& OldMoveSpeed);

	
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
};
