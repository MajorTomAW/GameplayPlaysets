// Copyright © 2024 MajorT. All rights reserved.


#include "SPlaysetDataList.h"

#include "Playset.h"
#include "SlateOptMacros.h"
#include "SPlaysetDataListRow.h"
#include "SPositiveActionButton.h"
#include "Application/PlaysetEditorApp.h"
#include "Application/Modes/PlaysetApplicationModeBase.h"

#define LOCTEXT_NAMESPACE "PlaysetDataList"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION

FString FPlaysetDataObjectEntry::GetDescription() const
{
	if (DataItem.IsValid())
	{
		return DataItem.Get()->GetDesc();
	}

	return TEXT("");
}

FString FPlaysetDataObjectEntry::GetDisplayName() const
{
	if (DataItem.IsValid())
	{
		return DataItem.Get()->GetClass()->GetDisplayNameText().ToString();
	}

	return TEXT("Invalid");
}

void SPlaysetDataList::Construct(const FArguments& InArgs, TSharedPtr<FPlaysetEditorApp> InAppBase)
{
	AppBase = InAppBase;
	
	for (UPlaysetDataItem* DataItem : AppBase->Playset->DataListObjects)
	{
		if (DataItem == nullptr)
		{
			continue;
		}
		
		DataList.Add(MakeShareable(new FPlaysetDataObjectEntry(DataItem)));
	}
	
	TSharedRef<SScrollBar> HorizontalScrollBar = SNew(SScrollBar)
		.Orientation(Orient_Horizontal)
		.Thickness(FVector2D(12.0f, 12.0f));

	TSharedRef<SScrollBar> VerticalScrollBar = SNew(SScrollBar)
		.Orientation(Orient_Vertical)
		.Thickness(FVector2D(12.0f, 12.0f));
	
	ListView = SNew(STileView<TSharedRef<FPlaysetDataObjectEntry>>)
		.OnGenerateTile(this, &Self::HandleGenerateRow)
		.OnSelectionChanged(this, &Self::HandleSelectionChanged)
		.Orientation(Orient_Vertical)
		.ListItemsSource(&DataList)
		.ItemHeight(38.f)
		.ItemWidth_Lambda([this]()
		{
			if (ViewBox.IsValid())
			{
				return ViewBox->GetTickSpaceGeometry().GetLocalSize().X;
			}
			
			return 0.f;
		})
		.ItemAlignment(EListItemAlignment::Fill)
		.AllowOverscroll(EAllowOverscroll::No)
		.SelectionMode(ESelectionMode::SingleToggle)
		.ExternalScrollbar(VerticalScrollBar);
	
	ChildSlot
	[
		SNew(SVerticalBox)

		// Top Border
		+ SVerticalBox::Slot()
		.VAlign(VAlign_Center)
		.AutoHeight()
		[
			SNew(SBorder)
			.BorderImage(FAppStyle::GetBrush("ToolPanel.GroupBorder"))
			.Padding(2.f)
			[
				SNew(SHorizontalBox)

				// New Data Entry
				+ SHorizontalBox::Slot()
				.VAlign(VAlign_Center)
				.Padding(4.f, 2.f)
				.AutoWidth()
				[
					SNew(SPositiveActionButton)
					.ToolTipText(LOCTEXT("AddDataToolTip", "Add a new data entry to the list."))
					.Icon(FAppStyle::Get().GetBrush("Icons.Plus"))
					.Text(LOCTEXT("AddData", "Add Data"))
				]
			]
		]

		// Content
		+ SVerticalBox::Slot()
		.Padding(0.f, 6.f, 0.f ,0.f)
		[
			SNew(SHorizontalBox)
			+ SHorizontalBox::Slot()
			.FillWidth(1.f)
			.Padding(0.f)
			[
				SAssignNew(ViewBox, SScrollBox)
				.Orientation(Orient_Horizontal)
				.ExternalScrollbar(HorizontalScrollBar)

				+ SScrollBox::Slot()
				.FillSize(1.f)
				[
					ListView.ToSharedRef()
				]
			]

			+ SHorizontalBox::Slot()
			.AutoWidth()
			[
				VerticalScrollBar
			]
		]

		+ SVerticalBox::Slot()
		.AutoHeight()
		[
			HorizontalScrollBar
		]
	];

	ListView->RebuildList();
}

TSharedRef<ITableRow> SPlaysetDataList::HandleGenerateRow(
	TSharedRef<FPlaysetDataObjectEntry> InDataItem, const TSharedRef<STableViewBase>& OwnerTable)
{
	return SNew(SPlaysetDataListRow, OwnerTable, InDataItem, ViewBox);
}

void SPlaysetDataList::HandleSelectionChanged(TSharedPtr<FPlaysetDataObjectEntry> InDataItem, ESelectInfo::Type SelectInfo)
{
	for (const auto& Item : ListView->GetItems())
	{
		if (TSharedPtr<SPlaysetDataListRow> RowWidget = StaticCastSharedPtr<SPlaysetDataListRow>( ListView->WidgetFromItem(Item)))
		{
			RowWidget->bIsSelected = Item == InDataItem;
		}
	}

	if (InDataItem.IsValid())
	{
		if (TSharedPtr<FPlaysetApplicationModeBase> Mode = StaticCastSharedPtr<FPlaysetApplicationModeBase>(AppBase->GetCurrentModePtr()))
		{
			Mode->DetailsView->SetObject(InDataItem->DataItem.Get());
		}	
	}
	else
	{
		if (TSharedPtr<FPlaysetApplicationModeBase> Mode = StaticCastSharedPtr<FPlaysetApplicationModeBase>(AppBase->GetCurrentModePtr()))
		{
			Mode->DetailsView->SetObject(nullptr);
		}
	}
}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION

#undef LOCTEXT_NAMESPACE