// Copyright © 2024 MajorT. All rights reserved.


#include "AssetDefinition_Playset.h"

#include "Playset.h"
#include "PlaysetsEditor.h"
#include "Style/PlaysetStyle.h"
#include "ThumbnailRendering/SceneThumbnailInfo.h"

#define LOCTEXT_NAMESPACE "AssetDefinition_Playset"

FText UAssetDefinition_Playset::GetAssetDisplayName() const
{
	return LOCTEXT("AssetDisplayName", "Playset Definition");
}

FText UAssetDefinition_Playset::GetAssetDescription(const FAssetData& AssetData) const
{
	if (const UPlayset* Playset = Cast<UPlayset>(AssetData.GetAsset()))
	{
		return Playset->ItemDescription;
	}

	return FText();
}

FLinearColor UAssetDefinition_Playset::GetAssetColor() const
{
	return FLinearColor(FPlaysetStyle::Get().GetColor("PlaysetsEditor.Color.Playset"));;
}

TSoftClassPtr<> UAssetDefinition_Playset::GetAssetClass() const
{
	return UPlayset::StaticClass();
}

TConstArrayView<FAssetCategoryPath> UAssetDefinition_Playset::GetAssetCategories() const
{
	static const auto Categories =
		{
			EAssetCategoryPaths::Basic,

			FAssetCategoryPath(FText::FromName("Playsets")),
		};

	return Categories;
}

UThumbnailInfo* UAssetDefinition_Playset::LoadThumbnailInfo(const FAssetData& InAssetData) const
{
	UPlayset* Playset = Cast<UPlayset>(InAssetData.GetAsset());
	if (Playset == nullptr)
	{
		return nullptr;
	}

	return UE::Editor::FindOrCreateThumbnailInfo<USceneThumbnailInfo>(Playset);
}

EAssetCommandResult UAssetDefinition_Playset::OpenAssets(const FAssetOpenArgs& OpenArgs) const
{
	const EToolkitMode::Type Mode = OpenArgs.GetToolkitMode();
	IPlaysetsEditorModule& PlaysetsEditorModule = IPlaysetsEditorModule::Get();

	for (UPlayset* Playset : OpenArgs.LoadObjects<UPlayset>())
	{
		bool bFoundExistingToolkit = false;
		//if (Playset-)

		if (!bFoundExistingToolkit)
		{
			PlaysetsEditorModule.CreatePlaysetsEditor(Mode, OpenArgs.ToolkitHost, Playset);
		}
	}

	return EAssetCommandResult::Handled;
}

#undef LOCTEXT_NAMESPACE
