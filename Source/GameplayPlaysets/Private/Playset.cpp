// Copyright © 2024 MajorT. All rights reserved.


#include "Playset.h"

#if WITH_EDITOR
#include "UObject/ObjectSaveContext.h"
#endif

#include "Data/PlaysetDisplayItem_Icon.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(Playset)

#define LOCTEXT_NAMESPACE "Playset"

UPlayset::UPlayset(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	// Fill in the default asset name if it is empty
	if (ItemName.IsEmpty())
	{
		ItemName = FText::FromString(GetName());
		PrimaryAssetName = FName(*ItemName.ToString());
	}
}

FPrimaryAssetId UPlayset::GetPrimaryAssetId() const
{
	return FPrimaryAssetId(PrimaryAssetType, PrimaryAssetName);
}

const FGameplayTagContainer& UPlayset::GetGameplayTags() const
{
	return PlaysetTags;
}

UTexture2D* UPlayset::GetIcon(const bool bUseLargeIfNotFound) const
{
	UTexture2D* Icon = nullptr;

	for (UPlaysetDataItem_Display* DisplayData : DisplayListObjects)
	{
		if (UPlaysetDisplayItem_Icon* IconData = Cast<UPlaysetDisplayItem_Icon>(DisplayData))
		{
			if (IconData->Icon.IsNull())
			{
				continue;
			}

			Icon = IconData->Icon.Get();

			if (Icon == nullptr)
			{
				Icon = IconData->Icon.LoadSynchronous();
			}
		}

		if (UPlaysetDisplayItem_LargeIcon* LargeIconData = Cast<UPlaysetDisplayItem_LargeIcon>(DisplayData))
		{
			if (LargeIconData->LargeIcon.IsNull())
			{
				continue;
			}

			Icon = LargeIconData->LargeIcon.Get();

			if (Icon == nullptr)
			{
				Icon = LargeIconData->LargeIcon.LoadSynchronous();
			}
		}

		if (Icon)
		{
			break;
		}
	}

	return Icon;
}


#if WITH_EDITOR
void UPlayset::PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
}

void UPlayset::PostSaveRoot(FObjectPostSaveRootContext ObjectSaveContext)
{
	PrimaryAssetName = GetFName();
	
	Super::PostSaveRoot(ObjectSaveContext);
}

EDataValidationResult UPlayset::IsDataValid(class FDataValidationContext& Context) const
{
	return Super::IsDataValid(Context);
}
#endif

#undef LOCTEXT_NAMESPACE
