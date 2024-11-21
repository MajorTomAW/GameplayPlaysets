// Copyright © 2024 MajorT. All rights reserved.


#include "SPlaysetDataListRow.h"

#include "ClassViewerModule.h"
#include "Playset.h"
#include "PlaysetDataItem.h"
#include "PropertyCustomizationHelpers.h"
#include "SlateOptMacros.h"
#include "SPlaysetDataList.h"
#include "StateTreeEditorStyle.h"
#include "Application/PlaysetEditorApp.h"
#include "Style/PlaysetStyle.h"
#include "Styling/SlateIconFinder.h"
#include "Widgets/Layout/SWidgetSwitcher.h"

#define LOCTEXT_NAMESPACE "PlaysetDataListRow"

namespace UE::Playset::Editor
{
	FLinearColor LerpColorSRGB(const FLinearColor ColorA, FLinearColor ColorB, float T)
	{
		const FColor A = ColorA.ToFColorSRGB();
		const FColor B = ColorB.ToFColorSRGB();
		return FLinearColor(FColor(
			static_cast<uint8>(FMath::RoundToInt(static_cast<float>(A.R) * (1.f - T) + static_cast<float>(B.R) * T)),
			static_cast<uint8>(FMath::RoundToInt(static_cast<float>(A.G) * (1.f - T) + static_cast<float>(B.G) * T)),
			static_cast<uint8>(FMath::RoundToInt(static_cast<float>(A.B) * (1.f - T) + static_cast<float>(B.B) * T)),
			static_cast<uint8>(FMath::RoundToInt(static_cast<float>(A.A) * (1.f - T) + static_cast<float>(B.A) * T))));
	}
}

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION

void SPlaysetDataListRow::Construct(
	const FArguments& InArgs, const TSharedRef<STableViewBase>& InOwnerTableView,
	TSharedRef<FPlaysetDataObjectEntry> InDataItem, const TSharedPtr<SScrollBox>& InViewBox, const TSharedPtr<SPlaysetDataList>& InOwnerList)
{
	OwnerList = InOwnerList;
	DataItem = InDataItem;

	InOwnerList->GetApp()->GetDetailsView()->OnFinishedChangingProperties().AddSP(this, &SPlaysetDataListRow::OnFinishedChangingProperties);
	
	ConstructInternal(STableRow::FArguments()
		.Padding(5.f)
		.Style(&FStateTreeEditorStyle::Get().GetWidgetStyle<FTableRowStyle>("StateTree.Selection"))
		, InOwnerTableView);
	
	this->ChildSlot
	.HAlign(HAlign_Fill)
	.VAlign(VAlign_Fill)
	[
		SNew(SBox)
		[
			SNew(SHorizontalBox)

			// Expander
			+ SHorizontalBox::Slot()
			.VAlign(VAlign_Fill)
			.HAlign(HAlign_Fill)
			.AutoWidth()
			[
				SNew(SExpanderArrow, SharedThis(this))
				.ShouldDrawWires(true)
				.IndentAmount(true)
				.BaseIndentLevel(0)
			]

			// Data item / None item
			+ SHorizontalBox::Slot()
			.VAlign(VAlign_Fill)
			.HAlign(HAlign_Fill)
			.Padding(FMargin(0.f, 4.f))
			.AutoWidth()
			[
				SNew(SBox)
				.HeightOverride(InArgs._HeightOverride)
				.ToolTipText_Lambda([InDataItem]()
				{
					if (InDataItem->GetDataItem() != nullptr)
					{
						return InDataItem->GetDataItem()->GetClass()->GetToolTipText();
					}

					return FText();
				})
				.VAlign(VAlign_Fill)
				[
					SNew(SBorder)
					.BorderImage(FStateTreeEditorStyle::Get().GetBrush("StateTree.State.Border"))
					.BorderBackgroundColor(this, &SPlaysetDataListRow::GetActiveColor)
					[
						SNew(SHorizontalBox)

						+ SHorizontalBox::Slot()
						.VAlign(VAlign_Center)
						.AutoWidth()
						[
							SNew(SBox)
							.HeightOverride(38.f)
							.VAlign(VAlign_Fill)
							[
								SNew(SBorder)
								.BorderImage(FAppStyle::GetBrush("WhiteBrush"))
								.BorderBackgroundColor(this, &SPlaysetDataListRow::GetBackgroundColor)
								.Padding(FMargin(8.f, 0.f, 12.f, 0.f))
								.VAlign(VAlign_Fill)
								[
									SAssignNew(WidgetSwitcher, SWidgetSwitcher)
									.WidgetIndex(0)

									// Active slot for NULL data item
									+ SWidgetSwitcher::Slot()
									[
										SNew(SOverlay)
										+ SOverlay::Slot()
										[
											SNew(SHorizontalBox)

											// Icon
											+ SHorizontalBox::Slot()
											.VAlign(VAlign_Center)
											.AutoWidth()
											[
												SNew(SBox)
												.Padding(FMargin(0.f, 0.f, 4.f, 0.f))
												[
													SNew(SImage)
													.Image(FAppStyle::GetBrush("Icons.Warning"))
												]
											]

											// New Data Item
											+ SHorizontalBox::Slot()
											.VAlign(VAlign_Center)
											.Padding(4.f, 0.f, 0.f, 0.f)
											.AutoWidth()
											[
												SNew(SComboButton)
												.VAlign(VAlign_Center)
												.Method(EPopupMethod::UseCurrentWindow)
												.ButtonStyle(FAppStyle::Get(), "NoBorder")
												.HasDownArrow(true)
												.OnGetMenuContent(this, &SPlaysetDataListRow::HandleCreateNewDataMenuEntry)
												.ButtonContent()
												[
													SNew(STextBlock)
													.Text(LOCTEXT("SelectNewDataObject", "None"))
												]
											]
										]
									]

									// Active slot when date item is set
									+ SWidgetSwitcher::Slot()
									[
										SNew(SOverlay)
										+ SOverlay::Slot()
										[
											SNew(SHorizontalBox)

											// Icon
											+ SHorizontalBox::Slot()
											.VAlign(VAlign_Center)
											.AutoWidth()
											[
												SNew(SBox)
												.Padding(FMargin(0.f, 0.f, 4.f, 0.f))
												[
													SNew(SImage)
													.Image_Lambda([InDataItem]()
													{
														if (InDataItem->GetDataItem() != nullptr)
														{
															return FSlateIconFinder::FindIconBrushForClass(InDataItem->GetDataItem()->GetClass());	
														}

														return FAppStyle::GetBrush("Icons.Warning");
													})
												]
											]

											// Display Name
											+ SHorizontalBox::Slot()
											.VAlign(VAlign_Center)
											.AutoWidth()
											[
												SNew(STextBlock)
												.Text(FText::FromString(InDataItem->GetDisplayName()))
												.TextStyle(FPlaysetStyle::Get(), "PlaysetsEditor.Text.Name")
											]
										]
									]
								]
							]
						]
					]
				]
			]

			// Remove Button
			+ SHorizontalBox::Slot()
			.VAlign(VAlign_Center)
			.Padding(2.f, 0.f)
			.AutoWidth()
			[
				MakeInsertDeleteDuplicateMenu()
			]

			// Description
			+ SHorizontalBox::Slot()
			.VAlign(VAlign_Center)
			.AutoWidth()
			[
				SNew(SHorizontalBox)
				+ SHorizontalBox::Slot()
				.HAlign(HAlign_Center)
				.VAlign(VAlign_Center)
				.Padding(FMargin(8.f, 0.f, 0.f, 0.f))
				.AutoWidth()
				[
					SNew(STextBlock)
					.Text_Lambda([InDataItem]()
					{
						return FText::FromString(InDataItem->GetDescription());
					})
					.TextStyle(FPlaysetStyle::Get(), "PlaysetsEditor.Text.Details")
					.ToolTipText_Lambda([InDataItem]()
					{
						if (InDataItem->GetDataItem() == nullptr)
						{
							return FText::FromString(TEXT("No data item specified."));
						}

						return FText::FromString(InDataItem->GetDescription());
					})
				]
			]
		]
	];

	bRowDirty = true;
}

void SPlaysetDataListRow::Tick(const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime)
{
	if (bRowDirty)
	{
		if (DataItem.IsValid() &&
			DataItem.Pin()->GetDataItem() != nullptr &&
			WidgetSwitcher.IsValid())
		{
			WidgetSwitcher->SetActiveWidgetIndex(1);
		}
		else if (WidgetSwitcher.IsValid())
		{
			WidgetSwitcher->SetActiveWidgetIndex(0);
		}

		bRowDirty = false;
	}
}

void SPlaysetDataListRow::OnFinishedChangingProperties(const FPropertyChangedEvent& PropertyChangedEvent)
{
	bRowDirty = true;
}

TSharedRef<SWidget> SPlaysetDataListRow::HandleCreateNewDataMenuEntry() const
{
	FClassViewerInitializationOptions Options;
	Options.bShowUnloadedBlueprints = true;
	Options.NameTypeToDisplay = EClassViewerNameTypeToDisplay::DisplayName;
	Options.ClassFilters.Add(MakeShareable(new FPlaysetDataListEntryClassFilter(DataItem.Pin()->GetPlayset())));

	FOnClassPicked OnClassPicked = FOnClassPicked::CreateSP(const_cast<SPlaysetDataListRow*>(this), &SPlaysetDataListRow::HandleClassPicked);

	return SNew(SBox)
		.HeightOverride(240.f)
		.WidthOverride(200.f)
		[
			FModuleManager::LoadModuleChecked<FClassViewerModule>("ClassViewer").CreateClassViewer(Options, OnClassPicked)	
		];
}

TSharedRef<SWidget> SPlaysetDataListRow::MakeInsertDeleteDuplicateMenu() const
{
	const UPlaysetDataItem* DataItemPtr = DataItem.Pin()->GetDataItem();
	SPlaysetDataListRow* ThisPtr = const_cast<SPlaysetDataListRow*>(this);

	FExecuteAction InsertAction = FExecuteAction();
	FExecuteAction DeleteAction = FExecuteAction();
	FExecuteAction DuplicateAction = FExecuteAction();

	// Insert Action idk yet
	{
		
	}
	
	// Duplicate Action only if its not a singleton
	if (DataItemPtr != nullptr &&
		!DataItemPtr->bIsSingleton)
	{
		DuplicateAction = FExecuteAction::CreateSP(ThisPtr, &SPlaysetDataListRow::HandleDataItemDuplicate);
	}

	// Delete action should always be available
	{
		DeleteAction = FExecuteAction::CreateSP(ThisPtr, &SPlaysetDataListRow::HandleDataItemDeleted);
	}

	return PropertyCustomizationHelpers::MakeInsertDeleteDuplicateButton(InsertAction, DeleteAction, DuplicateAction);
}

void SPlaysetDataListRow::HandleDataItemDeleted()
{
	UPlayset* PlaysetPtr = DataItem.Pin()->GetPlayset();
	if (PlaysetPtr == nullptr)
	{
		return;
	}

	const FScopedTransaction Transaction(LOCTEXT("DeleteDataItem", "Delete Data Item"));
	PlaysetPtr->SetFlags(RF_Transactional);
	PlaysetPtr->Modify();

	FProperty* DataListProperty = FindFProperty<FProperty>(UPlayset::StaticClass(), GET_MEMBER_NAME_CHECKED(UPlayset, DataListObjects));
	PlaysetPtr->PreEditChange(DataListProperty);

	FSetElementId ElementId = PlaysetPtr->DataListObjects.FindId(DataItem.Pin()->GetDataItem());
	PlaysetPtr->DataListObjects.Remove(ElementId);

	// Re-order the list to remove the gap
	PlaysetPtr->DataListObjects.CompactStable();

	FPropertyChangedEvent PropertyChangedEvent(DataListProperty, EPropertyChangeType::ArrayRemove);
	PlaysetPtr->PostEditChangeProperty(PropertyChangedEvent);

	OwnerList->MarkItemsDirty();
}

void SPlaysetDataListRow::HandleDataItemDuplicate()
{
	UPlayset* PlaysetPtr = DataItem.Pin()->GetPlayset();
	if (PlaysetPtr == nullptr)
	{
		return;
	}

	UPlaysetDataItem* DataItemPtr = DataItem.Pin()->GetDataItem();
	if (DataItemPtr == nullptr)
	{
		return;
	}

	const FScopedTransaction Transaction(LOCTEXT("DuplicateDataItem", "Duplicate Data Item"));
	PlaysetPtr->SetFlags(RF_Transactional);
	PlaysetPtr->Modify();

	FProperty* DataListProperty = FindFProperty<FProperty>(UPlayset::StaticClass(), GET_MEMBER_NAME_CHECKED(UPlayset, DataListObjects));
	PlaysetPtr->PreEditChange(DataListProperty);

	// Create a duplicate of the data item
	UPlaysetDataItem* Dupe = Cast<UPlaysetDataItem>(StaticDuplicateObject(DataItemPtr, PlaysetPtr));
	PlaysetPtr->DataListObjects.Add(Dupe);

	FPropertyChangedEvent PropertyChangedEvent(DataListProperty, EPropertyChangeType::ArrayAdd);
	PlaysetPtr->PostEditChangeProperty(PropertyChangedEvent);

	OwnerList->MarkItemsDirty();
}


void SPlaysetDataListRow::HandleClassPicked(UClass* InClass)
{
	UPlayset* PlaysetPtr = DataItem.Pin()->GetPlayset();
	if (PlaysetPtr == nullptr)
	{
		return;
	}

	FSlateApplication::Get().DismissAllMenus();

	check(InClass);
	check(InClass->IsChildOf(UPlaysetDataItem::StaticClass()));

	const FScopedTransaction Transaction(LOCTEXT("AddDataItem", "Change Data Item"));
	PlaysetPtr->SetFlags(RF_Transactional);
	PlaysetPtr->Modify();

	FProperty* DataListProperty = FindFProperty<FProperty>(UPlayset::StaticClass(), GET_MEMBER_NAME_CHECKED(UPlayset, DataListObjects));
	PlaysetPtr->PreEditChange(DataListProperty);

	UPlaysetDataItem* NewDataItem = NewObject<UPlaysetDataItem>(PlaysetPtr, InClass);
	
	FSetElementId LastElementId = PlaysetPtr->DataListObjects.FindId(nullptr);
	PlaysetPtr->DataListObjects[LastElementId] = NewDataItem;

	DataItem.Pin()->UpdateDataItem(NewDataItem);

	FPropertyChangedEvent PropertyChangedEvent(DataListProperty, EPropertyChangeType::ValueSet);
	PlaysetPtr->PostEditChangeProperty(PropertyChangedEvent);

	bRowDirty = true;
	OwnerList->MarkItemsDirty();
}

FSlateColor SPlaysetDataListRow::GetActiveColor() const
{
	if (OwnerList && OwnerList->IsSelected(DataItem))
	{
		return FStyleColors::Select;
	}
	
	return FLinearColor::Transparent;
}

FSlateColor SPlaysetDataListRow::GetBackgroundColor() const
{
	FColor Color = FLinearColor(FStyleColors::Warning.GetSpecifiedColor()).ToFColor(true);
	FColor Target = FColor::Black;
	float T = 0.4f;
	
	if (DataItem.Pin()->GetDataItem() != nullptr)
	{
		Color = FLinearColor(FStyleColors::Secondary.GetSpecifiedColor()).ToFColor(true);
		Target = FColor::White;
		T = 0.1f;
	}

	return FSlateColor(UE::Playset::Editor::LerpColorSRGB(Color, Target, T));
}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION


#undef LOCTEXT_NAMESPACE