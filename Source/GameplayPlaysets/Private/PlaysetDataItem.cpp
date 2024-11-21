// Copyright © 2024 MajorT. All rights reserved.


#include "PlaysetDataItem.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(PlaysetDataItem)

UPlaysetDataItem::UPlaysetDataItem(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
	, bIsSingleton(true)
{
	DataObjectName = GetClass()->GetName();
}

FString UPlaysetDataItem::GetDesc()
{
	return DataObjectName;
}
