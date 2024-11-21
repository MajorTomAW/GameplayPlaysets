// Copyright © 2024 MajorT. All rights reserved.


#include "SPlaysetConverter.h"

#include "SPlaysetConverterSelectedActorsList.h"
#include "SPlaysetConverterOptions.h"

#include "SlateOptMacros.h"
#include "SPlaysetConverterBottomToolbar.h"

#define LOCTEXT_NAMESPACE "PlaysetConverter"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION

void SPlaysetConverter::Construct(const FArguments& InArgs)
{
	TSharedRef<SPlaysetConverter> ThisPtr = SharedThis(this);
	
	ChildSlot
	[
		SNew(SBorder)
		.Padding(0.f)
		.BorderImage(FAppStyle::GetBrush("Brushes.Panel"))
		[
			SNew(SVerticalBox)

			// Content
			+ SVerticalBox::Slot()
			.Padding(0.f)
			[
				SNew(SSplitter)
				.Orientation(Orient_Horizontal)
				.PhysicalSplitterHandleSize(2.f)

				// Selected Actors List
				+ SSplitter::Slot()
				.Value(0.5f)
				[
					SAssignNew(SelectedActorsList, SPlaysetConverterSelectedActorsList)
					.SelectedActors(InArgs._SelectedActors)
				]

				// Playset Converter Options
				+ SSplitter::Slot()
				.Value(0.5f)
				[
					SAssignNew(ConverterOptions, SPlaysetConverterOptions, ThisPtr)
				]
			]

			// Bottom Toolbar
			+ SVerticalBox::Slot()
			.AutoHeight()
			[
				SNew(SSeparator)
				.Orientation(Orient_Horizontal)
				.Thickness(2.f)
			]

			+ SVerticalBox::Slot()
			.AutoHeight()
			[
				SAssignNew(BottomToolbar, SPlaysetConverterBottomToolbar, ThisPtr)
				.OnAssetsActivated(InArgs._OnAssetsActivated)
			]
		]
	];
}

TArray<TSharedRef<FPlaysetSelectedActorEntry>> SPlaysetConverter::GetSelectedActorEntries() const
{
	TArray<TSharedRef<FPlaysetSelectedActorEntry>> Entries;
	
	if (SelectedActorsList.IsValid())
	{
		Entries = SelectedActorsList->GetSelectedActorEntries();
	}

	return Entries;
}

UPlaysetConverterOptionsObject* SPlaysetConverter::GetOptionsObject() const
{
	if (ConverterOptions.IsValid())
	{
		return ConverterOptions->GetOptionsObject();
	}

	return nullptr;
}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION

#undef LOCTEXT_NAMESPACE