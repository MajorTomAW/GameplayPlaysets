// Copyright © 2024 MajorT. All rights reserved.


#include "Data/PlaysetDataItem_Traits.h"

UPlaysetDataItem_Traits::UPlaysetDataItem_Traits(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	DataObjectName = TEXT("Traits");
}

FString UPlaysetDataItem_Traits::GetDesc()
{
	return FString::Printf(TEXT("%s"), *Traits.ToString());
}
