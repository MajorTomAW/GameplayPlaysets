// Copyright © 2024 MajorT. All rights reserved.


#include "SPlaysetConverterBottomToolbar.h"

#include "ContentBrowserModule.h"
#include "IContentBrowserSingleton.h"
#include "SlateOptMacros.h"
#include "SPlaysetConverter.h"
#include "SPlaysetConverterSelectedActorsList.h"

#define LOCTEXT_NAMESPACE "PlaysetConverterBottomToolbar"

class FContentBrowserModule;
BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION

void SPlaysetConverterBottomToolbar::Construct(const FArguments& InArgs, const TSharedRef<SPlaysetConverter>& InOwner)
{
	Owner = InOwner;
	OnAssetsActivatedDelegate = InArgs._OnAssetsActivated;
	
	ChildSlot
	[
		SNew(SVerticalBox)
		+ SVerticalBox::Slot()
		.Padding(4.f)
		[
			SNew(SHorizontalBox)

			// Save Button
			+ SHorizontalBox::Slot()
			.AutoWidth()
			.Padding(5.f, 2.f)
			.HAlign(HAlign_Right)
			[
				SNew(SButton)
				.ContentPadding(FMargin(50.f, 4.f))
				.ButtonStyle(&FAppStyle::Get().GetWidgetStyle<FButtonStyle>("PrimaryButton"))
				.TextStyle(&FAppStyle::Get().GetWidgetStyle<FTextBlockStyle>("PrimaryButtonText"))
				.HAlign(HAlign_Center)
				.Text(LOCTEXT("SaveButton", "Save"))
				.IsEnabled(this, &SPlaysetConverterBottomToolbar::IsSaveButtonEnabled)
				.OnClicked(this, &SPlaysetConverterBottomToolbar::HandleSaveButtonClicked)
			]

			// Warning Message
			+ SHorizontalBox::Slot()
			.AutoWidth()
			.Padding(5.f, 1.f)
			.HAlign(HAlign_Center)
			[
				SNew(SHorizontalBox)

				// Warning Icon
				+ SHorizontalBox::Slot()
				.AutoWidth()
				.VAlign(VAlign_Center)
				.Padding(16.f, 0.f, 4.f, 0.f)
				[
					SNew(SImage)
					.Image(FAppStyle::Get().GetBrush("Icons.Warning"))
					.ColorAndOpacity(FStyleColors::Warning)
				]

				// Warning Text
				+ SHorizontalBox::Slot()
				.AutoWidth()
				.VAlign(VAlign_Center)
				[
					SNew(STextBlock)
					.TextStyle(&FAppStyle::Get().GetWidgetStyle<FTextBlockStyle>("NormalText.Subdued"))
					.Text(LOCTEXT("WarningMessage", "Note that only the transform and the class defaults of the actors will be saved."))
					.ColorAndOpacity(FStyleColors::Warning)
				]
			]

			/*// Cancel Button REDUNDANT AS WE CAN JUST CLOSE THE WINDOW LMAO
			+ SHorizontalBox::Slot()
			.AutoWidth()
			.Padding(5.f, 1.f)
			[
				SNew(SButton)
				.ContentPadding(FMargin(50.f, 4.f))
				.TextStyle(&FAppStyle::Get().GetWidgetStyle<FTextBlockStyle>("PrimaryButtonText"))
				.HAlign(HAlign_Center)
				.Text(LOCTEXT("CancelButton", "Cancel"))
			]*/
		]
	];
}

bool SPlaysetConverterBottomToolbar::IsSaveButtonEnabled() const
{
	if (!Owner.IsValid())
	{
		return false;
	}

	return Owner->GetSelectedActorEntries().Num() > 0;
}

FReply SPlaysetConverterBottomToolbar::HandleSaveButtonClicked()
{
	if (!Owner.IsValid())
	{
		return FReply::Handled();
	}

	TArray<TSharedRef<FPlaysetSelectedActorEntry>> Entries = Owner->GetSelectedActorEntries();
	TArray<FAssetData> Assets;
	for (const TSharedRef<FPlaysetSelectedActorEntry>& Entry : Entries)
	{
		Assets.Add(Entry->GetAssetData());
	}

	OnAssetsActivatedDelegate.Execute(Assets, EAssetTypeActivationMethod::Opened);
	return FReply::Handled();
}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION

#undef LOCTEXT_NAMESPACE