// Copyright © 2024 MajorT. All rights reserved.


#include "SPlaysetConverterSelectedActorsListRow.h"

#include "SlateOptMacros.h"
#include "Styling/SlateIconFinder.h"

#define LOCTEXT_NAMESPACE "PlaysetConverter"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION

void SPlaysetConverterSelectedActorsListRow::Construct(
	const FArguments& InArgs,
	const TSharedRef<STableViewBase>& InOwnerTable, TSharedRef<FPlaysetSelectedActorEntry>& InEntry,
	const TSharedRef<SPlaysetConverterSelectedActorsList>& InOwnerList)
{
	Entry = InEntry;
	OwnerList = InOwnerList;
	
	ConstructInternal(STableRow::FArguments()
		.Padding(0.f), InOwnerTable);
	
	this->ChildSlot
	[
		SNew(SHorizontalBox)
		.ToolTip(MakeToolTip())

		// Checkbox
		+ SHorizontalBox::Slot()
		.AutoWidth()
		.HAlign(HAlign_Left)
		.VAlign(VAlign_Center)
		.Padding(8.f, 0.f)
		[
			SNew(SCheckBox)
			.HAlign(HAlign_Center)
			.IsChecked(this, &Self::IsItemEnabled)
			.OnCheckStateChanged(this, &Self::HandleCheckStateChanged)
		]

		// Icon
		+ SHorizontalBox::Slot()
		.AutoWidth()
		.HAlign(HAlign_Left)
		.VAlign(VAlign_Center)
		.Padding(18.f, 0.f, 4.f, 0.f)
		[
			SNew(SImage)
			.DesiredSizeOverride(FVector2d(16.f))
			.Image(FSlateIconFinder::FindIconForClass(InEntry->GetActorClass()).GetIcon())
			.ColorAndOpacity(this, &Self::GetIconColor)
		]

		// Name
		+ SHorizontalBox::Slot()
		.AutoWidth()
		.HAlign(HAlign_Left)
		.VAlign(VAlign_Center)
		.Padding(4.f, 0.f)
		[
			SAssignNew(LabelText, STextBlock)
			.Text_Lambda([InEntry]()
			{
				return FText::FromString(InEntry->GetActorLabel());
			})
			.ColorAndOpacity(this, &Self::GetTextColor)
			.TextStyle(GetTextStyle())
		]

		// Native
		+ SHorizontalBox::Slot()
		.AutoWidth()
		.HAlign(HAlign_Left)
		.VAlign(VAlign_Center)
		.Padding(24.f, 0.f, 0.f, 0.f)
		[
			SNew(STextBlock)
			.TextStyle(&FAppStyle::GetWidgetStyle<FTextBlockStyle>("NormalText.Subdued"))
			.Text_Lambda([InEntry]()
			{
				if (InEntry->bIsNative)
				{
					return LOCTEXT("NativeWarning", "(Native)");
				}
					
				return FText();
			})
		]
	];
}

TSharedPtr<IToolTip> SPlaysetConverterSelectedActorsListRow::MakeToolTip() const
{
	return FSlateApplication::Get().MakeToolTip(Entry->GetActorClass()->GetToolTipText());
}

ECheckBoxState SPlaysetConverterSelectedActorsListRow::IsItemEnabled() const
{
	if (!Entry.IsValid())
	{
		return ECheckBoxState::Unchecked;
	}

	return Entry->bIsEnabled ? ECheckBoxState::Checked : ECheckBoxState::Unchecked;
}

void SPlaysetConverterSelectedActorsListRow::HandleCheckStateChanged(ECheckBoxState NewState)
{
	const bool bIsChecked = NewState == ECheckBoxState::Checked;
	Entry->bIsEnabled = bIsChecked;

	if (LabelText.IsValid())
	{
		LabelText->SetTextStyle(GetTextStyle());
	}

	if (OwnerList.IsValid())
	{
		for (const TSharedRef Selected : OwnerList->SelectionTileView->GetSelectedItems())
		{
			Selected->bIsEnabled = bIsChecked;
		}
	}
}

bool SPlaysetConverterSelectedActorsListRow::IsEntryChecked() const
{
	if (!Entry.IsValid())
	{
		return false;
	}

	return Entry->bIsEnabled;
}

FSlateColor SPlaysetConverterSelectedActorsListRow::GetIconColor() const
{
	if (IsEntryChecked())
	{
		return FLinearColor::White;
	}

	return FLinearColor(FColor::FromHex("#444"));
}

FSlateColor SPlaysetConverterSelectedActorsListRow::GetTextColor() const
{
	if (IsEntryChecked())
	{
		return FLinearColor::White;
	}

	return FLinearColor(FColor::FromHex("#ff000088"));;
}

const FTextBlockStyle* SPlaysetConverterSelectedActorsListRow::GetTextStyle() const
{
	if (IsEntryChecked())
	{
		return &FAppStyle::GetWidgetStyle<FTextBlockStyle>("NormalText");
	}

	return &FAppStyle::GetWidgetStyle<FTextBlockStyle>("NormalText.Subdued");
}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION

#undef LOCTEXT_NAMESPACE