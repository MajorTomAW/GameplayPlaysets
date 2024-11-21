// Copyright © 2024 MajorT. All rights reserved.


#include "Data/PlaysetDisplayItem_Icon.h"

UPlaysetDisplayItem_Icon::UPlaysetDisplayItem_Icon(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	DataObjectName = TEXT("Icon");
}

FString UPlaysetDisplayItem_Icon::GetDesc()
{
	FString Desc = TEXT("Texture: ");

	if (!Icon.IsNull())
	{
		Desc += FString::Printf(TEXT("%s"), *Icon->GetName());	
	}
	else
	{
		Desc += TEXT("None");
	}
	
	return Desc;
}

void UPlaysetDisplayItem_Icon::SetIcon(const UTexture2D* InIcon)
{
	if (InIcon != nullptr)
	{
		Icon = InIcon;
	}
}


UPlaysetDisplayItem_LargeIcon::UPlaysetDisplayItem_LargeIcon(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	DataObjectName = TEXT("Large Icon");
}

FString UPlaysetDisplayItem_LargeIcon::GetDesc()
{
	FString Desc = TEXT("Texture: ");

	if (!LargeIcon.IsNull())
	{
		Desc += FString::Printf(TEXT("%s"), *LargeIcon->GetName());	
	}
	else
	{
		Desc += TEXT("None");
	}
	
	return Desc;
}

void UPlaysetDisplayItem_LargeIcon::SetLargeIcon(const UTexture2D* InLargeIcon)
{
	if (InLargeIcon != nullptr)
	{
		LargeIcon = InLargeIcon;
	}
}
