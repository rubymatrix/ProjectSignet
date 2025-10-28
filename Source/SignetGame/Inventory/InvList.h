// Copyright Red Lotus Games, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Net/Serialization//FastArraySerializer.h"
#include "InventoryTypes.h"
#include "InvList.generated.h"

class USignetInventoryComponent;

USTRUCT()
struct SIGNETGAME_API FInvEntry : public FFastArraySerializerItem
{
	GENERATED_BODY()

	UPROPERTY()
	int32 ItemID = 0;

	UPROPERTY()
	uint16 Quantity = 0;

	UPROPERTY()
	FGuid InstanceId;
};

USTRUCT()
struct SIGNETGAME_API FInvList : public FFastArraySerializer
{
	GENERATED_BODY()

	UPROPERTY()
	TArray<FInvEntry> Entries;
	
	UPROPERTY(NotReplicated)
	USignetInventoryComponent* Owner = nullptr;
	
	UPROPERTY(NotReplicated)
	TArray<int32> RemovedItemIDsShadow;
	
	void PreReplicatedRemove(const TArrayView<int32>& RemovedIndices, int32 FinalSize);
	void PostReplicatedRemove(const TArrayView<int32>& RemovedIndices, int32 FinalSize);
	void PostReplicatedAdd(const TArrayView<int32>& AddedIndices, int32 FinalSize);
	void PostReplicatedChange(const TArrayView<int32>& ChangedIndices, int32 FinalSize);

	bool NetDeltaSerialize(FNetDeltaSerializeInfo& DeltaParms)
	{
		return FFastArraySerializer::FastArrayDeltaSerialize<FInvEntry, FInvList>(Entries, DeltaParms, *this);
	}
	
	FORCEINLINE void MarkItemDirty(FInvEntry& Item) { FFastArraySerializer::MarkItemDirty(Item); }
	FORCEINLINE void MarkArrayDirty() { FFastArraySerializer::MarkArrayDirty(); }
};

template<>
struct TStructOpsTypeTraits<FInvList> : public TStructOpsTypeTraitsBase2<FInvList>
{
	enum { WithNetDeltaSerializer = true };
};