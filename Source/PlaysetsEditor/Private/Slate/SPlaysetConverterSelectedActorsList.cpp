// Copyright © 2024 MajorT. All rights reserved.


#include "SPlaysetConverterSelectedActorsList.h"

#include "SlateOptMacros.h"
#include "SPlaysetConverterSelectedActorsListRow.h"

#define LOCTEXT_NAMESPACE "PlaysetConverter"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION

void SPlaysetConverterSelectedActorsList::Construct(const FArguments& InArgs)
{
	AllActors = InArgs._SelectedActors;

	SelectionTileView = SNew(STileView<TSharedRef<FPlaysetSelectedActorEntry>>)
		.ListItemsSource(&SelectionEntries)
		.Orientation(Orient_Vertical)
		.AllowOverscroll(EAllowOverscroll::Yes)
		.ItemWidth(300.f)
		.ItemHeight(20.f)
		.SelectionMode(ESelectionMode::Type::Multi)
		.ItemAlignment(EListItemAlignment::Fill)
		.OnGenerateTile(this, &SPlaysetConverterSelectedActorsList::HandleGenerateTile);
	
	ChildSlot
	[
		SNew(SVerticalBox)

		+ SVerticalBox::Slot()
		.Padding(8.f, 16.f)
		.HAlign(HAlign_Center)
		.AutoHeight()
		[
			SNew(STextBlock)
			.TextStyle(&FAppStyle::GetWidgetStyle<FTextBlockStyle>("NormalText.Subdued"))
			.Text(LOCTEXT("ConvertInfo", "All selected actors will be converted to a Playset."))
		]

		+ SVerticalBox::Slot()
		.HAlign(HAlign_Fill)
		.VAlign(VAlign_Fill)
		[
			SNew(SBorder)
			.Padding(0.f)
			.BorderImage(FAppStyle::GetBrush("Brushes.Recessed"))
			[
				SelectionTileView.ToSharedRef()
			]
		]

		// Bottom Bar
		+ SVerticalBox::Slot()
		.AutoHeight()
		[
			SNew(SBorder)
			.BorderImage(FAppStyle::GetBrush("Brushes.Header"))
			.VAlign(VAlign_Center)
			.HAlign(HAlign_Left)
			.Padding(FMargin(14.f, 9.f))
			[
				SNew(STextBlock)
				.Text(this, &Self::GetFilterStatusText)
				.ColorAndOpacity(this, &Self::GetFilterStatusTextColor)
			]
		]
	];

	PopulateSelectionEntries();
	SelectionTileView->RequestListRefresh();
}

TArray<TSharedRef<FPlaysetSelectedActorEntry>> SPlaysetConverterSelectedActorsList::GetSelectedActorEntries() const
{
	TArray<TSharedRef<FPlaysetSelectedActorEntry>> Result;

	for (const TSharedRef Entry : SelectionEntries)
	{
		if (Entry->bIsEnabled)
		{
			Result.Add(Entry);
		}
	}

	return Result;
}

FText SPlaysetConverterSelectedActorsList::GetFilterStatusText() const
{
	int32 NumSelected = 0;

	for (const TSharedRef Entry : SelectionEntries)
	{
		if (Entry->bIsEnabled) NumSelected++;
	}

	auto BuildText = [NumSelected, this]()
	{
		if (NumSelected > 0)
		{
			return FText::Format(LOCTEXT("FilterStatusTextMultiple", "{0} actors ({1} selected)"), SelectionEntries.Num(), NumSelected);
		}

		return FText::Format(LOCTEXT("FilterStatusTextSingle", "{0} actors"), SelectionEntries.Num());
	};
	
	return BuildText();
}

FSlateColor SPlaysetConverterSelectedActorsList::GetFilterStatusTextColor() const
{
	return FAppStyle::Get().GetSlateColor("Colors.AccentWhite");
}

TSharedRef<ITableRow> SPlaysetConverterSelectedActorsList::HandleGenerateTile(
	TSharedRef<FPlaysetSelectedActorEntry> InEntry, const TSharedRef<STableViewBase>& InOwnerTable)
{
	return SNew(SPlaysetConverterSelectedActorsListRow, InOwnerTable, InEntry, SharedThis(this));
}

void SPlaysetConverterSelectedActorsList::PopulateSelectionEntries()
{
	SelectionEntries.Empty();

	for (const TWeakObjectPtr ActorPtr : AllActors)
	{
		if (AActor* Actor = ActorPtr.Get())
		{
			SelectionEntries.Add(MakeShareable(new FPlaysetSelectedActorEntry(ActorPtr)));
		}
	}
}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION

#undef LOCTEXT_NAMESPACE