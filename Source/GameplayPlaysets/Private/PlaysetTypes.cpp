// Copyright © 2024 MajorT. All rights reserved.


#include "PlaysetTypes.h"
#include "NativeGameplayTags.h"

UE_DEFINE_GAMEPLAY_TAG_STATIC(TAG_Trait_Transient, "Item.Trait.Transient")
UE_DEFINE_GAMEPLAY_TAG_STATIC(TAG_Trait_SingleStack, "Item.Trait.SingleStack")


FPlaysetDataList::FPlaysetDataList()
	: MinLevel(INDEX_NONE)
	, MaxLevel(INDEX_NONE)
{
	Traits.AddTag(TAG_Trait_Transient);
	Traits.AddTag(TAG_Trait_SingleStack);
}

UTexture2D* FPlaysetDataList::GetThumbnailImage() const
{
	UTexture2D* ThumbnailImage = nullptr;

	if (!Icon.IsNull())
	{
		ThumbnailImage = Icon.Get();

		if (ThumbnailImage)
		{
			return ThumbnailImage;
		}

		ThumbnailImage = Icon.LoadSynchronous();
	}

	if (!LargeIcon.IsNull() && ThumbnailImage == nullptr)
	{
		ThumbnailImage = LargeIcon.Get();

		if (ThumbnailImage == nullptr)
		{
			ThumbnailImage = LargeIcon.LoadSynchronous();
		}
	}

	return ThumbnailImage;
}

FPlaysetActorData::FPlaysetActorData()
{
}
