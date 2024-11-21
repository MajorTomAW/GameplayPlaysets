// Copyright © 2024 MajorT. All rights reserved.


#include "Data/PlaysetDataItem_Traits.h"

#include "PlaysetGameplayTags.h"

UPlaysetDataItem_Traits::UPlaysetDataItem_Traits(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	bIsSingleton = false;
	
	DataObjectName = TEXT("Traits");

	Traits.AddTag(PlaysetGameplayTags::TAG_ItemTrait_Transient);
}

FString UPlaysetDataItem_Traits::GetDesc()
{
	return FString::Printf(TEXT("%s"), *Traits.ToString());
}
