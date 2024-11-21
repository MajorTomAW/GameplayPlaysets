// Copyright © 2024 MajorT. All rights reserved.


#include "PlaysetDetailCustomization.h"
#include "Application/PlaysetEditorApp.h"
#include "DetailLayoutBuilder.h"


FPlaysetDetailCustomization::FPlaysetDetailCustomization()
{
}

TSharedRef<IDetailCustomization> FPlaysetDetailCustomization::MakeInstance(FPlaysetDetailsViewArgs InArgs, TSharedPtr<FPlaysetEditorApp> InPlaysetApp)
{
	TSharedRef<FPlaysetDetailCustomization> Instance = MakeShareable(new FPlaysetDetailCustomization());
	Instance->Args = InArgs;
	Instance->AppPtr = InPlaysetApp;
	return Instance;
}

void FPlaysetDetailCustomization::CustomizeDetails(IDetailLayoutBuilder& DetailBuilder)
{
	for (const FName& Category : Args.CategoryBlacklist)
	{
		DetailBuilder.HideCategory(Category);
	}

	for (const FName& Property : Args.PropertyBlacklist)
	{
		DetailBuilder.HideProperty(Property);
	}

	TArray<FName> AllCategories;
	DetailBuilder.GetCategoryNames(AllCategories);

	if (!Args.CategoryWhitelist.IsEmpty())
	{
		for (const FName& Category : AllCategories)
		{
			if (Args.CategoryWhitelist.Contains(Category))
			{
				continue;
			}

			DetailBuilder.HideCategory(Category);
		}
	}
}
