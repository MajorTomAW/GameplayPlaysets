// Copyright © 2024 MajorT. All rights reserved.


#include "SPlaysetDataList.h"

#include "ClassViewerFilter.h"
#include "ClassViewerModule.h"
#include "Playset.h"
#include "PlaysetDataItem.h"
#include "PlaysetDataListCommands.h"
#include "PropertyCustomizationHelpers.h"
#include "SlateOptMacros.h"
#include "SPlaysetDataListRow.h"
#include "SPositiveActionButton.h"
#include "Application/PlaysetEditorApp.h"
#include "Data/PlaysetDataItem_Display.h"
#include "Style/PlaysetStyle.h"
#include "Widgets/Views/STileView.h"

#define LOCTEXT_NAMESPACE "PlaysetDataList"

bool FPlaysetDataListEntryClassFilter::IsClassAllowed(
	const FClassViewerInitializationOptions& InInitOptions, const UClass* InClass, TSharedRef<class FClassViewerFilterFuncs> InFilterFuncs)
{
	if (InClass == nullptr)
	{
		return false;
	}

	if ( !(!InClass->HasAnyClassFlags(CLASS_Abstract | CLASS_HideDropDown) && InClass->HasAnyClassFlags(CLASS_EditInlineNew)))
	{
		return false;
	}

	// Has to be a child of UPlaysetDataItem
	if (!InClass->IsChildOf(UPlaysetDataItem::StaticClass()))
	{
		return false;
	}

	// Display items are not allowed
	if (InClass->IsChildOf(UPlaysetDataItem_Display::StaticClass()))
	{
		return false;
	}

	// Singleton classes can only be added once
	if (InClass->GetDefaultObject<UPlaysetDataItem>()->bIsSingleton && Playset.IsValid())
	{
		for (UPlaysetDataItem* Existing : Playset->DataListObjects)
		{
			if (Existing == nullptr)
			{
				continue;
			}
			
			if (Existing->GetClass() == InClass)
			{
				return false;
			}
		}
	}

	return true;
}

bool FPlaysetDataListEntryClassFilter::IsUnloadedClassAllowed(
	const FClassViewerInitializationOptions& InInitOptions, const TSharedRef<const class IUnloadedBlueprintData> InUnloadedClassData,
	TSharedRef<class FClassViewerFilterFuncs> InFilterFuncs)
{
	return InUnloadedClassData->IsChildOf(UPlaysetDataItem::StaticClass()) && !InUnloadedClassData->IsChildOf(UPlaysetDataItem_Display::StaticClass());
}

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION

FString FPlaysetDataObjectEntry::GetDescription() const
{
	if (DataItem.IsValid())
	{
		return GetDataItem()->GetDesc();
	}

	return TEXT("!!!! Please specify a data object. !!!!");
}

FString FPlaysetDataObjectEntry::GetDisplayName() const
{
	if (DataItem.IsValid())
	{
		return GetDataItem()->GetClass()->GetDisplayNameText().ToString();
	}

	return FString();
}

SPlaysetDataList::SPlaysetDataList()
{
	ItemHeight = 28.f;
	bItemsDirty = false;
}

void SPlaysetDataList::Construct(const FArguments& InArgs, TSharedPtr<FPlaysetEditorApp> InPlaysetApp, const TSharedRef<FUICommandList>& InCommandList)
{
	AppPtr = InPlaysetApp;
	ItemHeight = InArgs._ItemHeight;

	ScrollBar = SNew(SScrollBar)
		.Orientation(Orient_Vertical)
		.Thickness(FVector2d(10, 10));

	RegenerateDataListFromSource();

	ListView = SNew(STileView<TSharedRef<FPlaysetDataObjectEntry>>)
		.OnGenerateTile(this, &Self::HandleGenerateRow)
		.OnSelectionChanged(this, &Self::HandleSelectionChanged)
		.OnContextMenuOpening(this, &Self::HandleContextMenuOpening)
		.Orientation(Orient_Vertical)
		.ListItemsSource(&DataList)
		.ItemHeight(ItemHeight)
		.ItemWidth_Lambda([this]()
		{
			if (ViewBox.IsValid())
			{
				return ViewBox->GetTickSpaceGeometry().GetLocalSize().X - 1;
			}

			return 0.f;
		})
		.ItemAlignment(EListItemAlignment::Fill)
		.AllowOverscroll(EAllowOverscroll::Yes)
		.SelectionMode(ESelectionMode::Type::Multi)
		.ExternalScrollbar(ScrollBar);
	
	ChildSlot
	[
		SNew(SVerticalBox)
		
		// Titlebar
		+ SVerticalBox::Slot()
		.VAlign(VAlign_Center)
		.AutoHeight()
		[
			SNew(SBorder)
			.BorderImage(FAppStyle::GetBrush(/*"DetailsView.CategoryTop"*/"Menu.Background"))
			.Padding(0.f, 4.f)
			[
				SNew(SHorizontalBox)

				// Label
				+ SHorizontalBox::Slot()
				.VAlign(VAlign_Center)
				.AutoWidth()
				.Padding(4.f, 0.f, 16.f, 0.f)
				[
					SNew(STextBlock)
					.TextStyle(FPlaysetStyle::Get(), "PlaysetsEditor.Text.Details")
					.Text_Lambda([]()
					{
						FString PropertyName = GET_MEMBER_NAME_CHECKED(UPlayset, DataListObjects).ToString();

						// Put spaces between the words
						for (int32 Index = 1; Index < PropertyName.Len(); ++Index)
						{
							if (FChar::IsUpper(PropertyName[Index]))
							{
								PropertyName.InsertAt(Index, TEXT(" "));
								++Index;
							}
						}
						
						return FText::FromString(PropertyName);
					})
					.ToolTipText_Lambda([]()
					{
						if (const FProperty* Property = FindFProperty<FProperty>(UPlayset::StaticClass(), GET_MEMBER_NAME_CHECKED(UPlayset, DataListObjects)))
						{
							return Property->GetToolTipText();
						}
						
						return FText();
					})
				]

				// Items Count
				+ SHorizontalBox::Slot()
				.VAlign(VAlign_Center)
				.AutoWidth()
				.Padding(16.f, 0.f)
				[
					SNew(STextBlock)
					.TextStyle(FPlaysetStyle::Get(), "PlaysetsEditor.Text.Details")
					.Text_Lambda([this]()
					{
						return FText::Format(LOCTEXT("DataListCount", "{0} Items"), DataList.Num());
					})
				]

				// Add Element
				+ SHorizontalBox::Slot()
				.AutoWidth()
				.VAlign(VAlign_Center)
				.Padding(2.f, 0.f)
				[
					PropertyCustomizationHelpers::MakeAddButton(FSimpleDelegate::CreateSP(this, &SPlaysetDataList::HandleNewDataObject))
				]

				// Remove Elements
				+ SHorizontalBox::Slot()
				.AutoWidth()
				.VAlign(VAlign_Center)
				.Padding(2.f, 0.f)
				[
					PropertyCustomizationHelpers::MakeEmptyButton(FSimpleDelegate::CreateSP(this, &SPlaysetDataList::ClearAllDataObjects))
				]
			]
		]

		// Content
		+ SVerticalBox::Slot()
		.Padding(0.f, 6.f, 0.f, 0.f)
		[
			SNew(SHorizontalBox)

			// Data list
			+ SHorizontalBox::Slot()
			.FillWidth(1.f)
			.Padding(0.f)
			[
				SAssignNew(ViewBox, SScrollBox)
				.Orientation(Orient_Horizontal)

				+ SScrollBox::Slot()
				[
					ListView.ToSharedRef()
				]
			]

			// Scroll bar
			+ SHorizontalBox::Slot()
			.AutoWidth()
			[
				ScrollBar.ToSharedRef()
			]
		]
	];
	
	bItemsDirty = true;

	CommandList = InCommandList;
	BindCommands();
}

void SPlaysetDataList::BindCommands()
{
	const FPlaysetDataListCommands& Commands = FPlaysetDataListCommands::Get();

	CommandList->MapAction
	(
		Commands.DeleteDataObject,
		FExecuteAction::CreateSP(this, &SPlaysetDataList::HandleDataObjectDeleted),
		FCanExecuteAction::CreateSP(this, &SPlaysetDataList::HasSelection)
	);

	CommandList->MapAction
	(
		Commands.DuplicateDataObject,
		FExecuteAction::CreateSP(this, &SPlaysetDataList::HandleDataObjectDuplicated),
		FCanExecuteAction::CreateSP(this, &SPlaysetDataList::CanDuplicateSelection)
	);
}

void SPlaysetDataList::Tick(const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime)
{
	if (bItemsDirty )
	{
		UpdateDataList(true);	
	}
}

FReply SPlaysetDataList::OnKeyDown(const FGeometry& MyGeometry, const FKeyEvent& InKeyEvent)
{
	if (CommandList->ProcessCommandBindings(InKeyEvent))
	{
		return FReply::Handled();
	}

	return SCompoundWidget::OnKeyDown(MyGeometry, InKeyEvent);
}

void SPlaysetDataList::UpdateDataList(bool bForce)
{
	if (bForce)
	{
		RegenerateDataListFromSource();
	}
	
	if (ListView.IsValid())
	{
		ListView->RebuildList();
	}
	
	bItemsDirty = false;
}


bool SPlaysetDataList::IsSelected(const UPlaysetDataItem* InDataItem) const
{
	for (const TSharedRef Selected : ListView->GetSelectedItems())
	{
		if (Selected->GetDataItem() == InDataItem)
		{
			return true;
		}
	}

	return false;
}

bool SPlaysetDataList::IsSelected(const TWeakPtr<FPlaysetDataObjectEntry>& InDataItem) const
{
	if (!InDataItem.IsValid())
	{
		return false;
	}

	return IsSelected(InDataItem.Pin()->GetDataItem());
}

TSharedRef<ITableRow> SPlaysetDataList::HandleGenerateRow(
	TSharedRef<FPlaysetDataObjectEntry> InDataItem, const TSharedRef<STableViewBase>& OwnerTable)
{
	return SNew(SPlaysetDataListRow, OwnerTable, InDataItem, ViewBox, SharedThis(this));
}

TSharedRef<SWidget> SPlaysetDataList::HandleCreateNewDataMenuEntry() const
{
	FClassViewerInitializationOptions Options;
	Options.bShowUnloadedBlueprints = true;
	Options.NameTypeToDisplay = EClassViewerNameTypeToDisplay::DisplayName;
	Options.ClassFilters.Add(MakeShareable(new FPlaysetDataListEntryClassFilter(GetApp()->GetPlayset())));

	FOnClassPicked OnClassPicked = FOnClassPicked::CreateRaw(const_cast<SPlaysetDataList*>(this), &SPlaysetDataList::HandleDataObjectClassPicked);
	
	return SNew(SBox)
		.HeightOverride(240.0f)
		.WidthOverride(200.f)
		[
			FModuleManager::LoadModuleChecked<FClassViewerModule>("ClassViewer").CreateClassViewer(Options, OnClassPicked)
		];
}

TSharedPtr<SWidget> SPlaysetDataList::HandleContextMenuOpening()
{
	FMenuBuilder MenuBuilder(true, CommandList);

	MenuBuilder.AddMenuEntry(FPlaysetDataListCommands::Get().DeleteDataObject);
	MenuBuilder.AddMenuEntry(FPlaysetDataListCommands::Get().DuplicateDataObject);

	return MenuBuilder.MakeWidget();
}

void SPlaysetDataList::HandleDataObjectClassPicked(UClass* InClass)
{
	UPlayset* PlaysetPtr = GetApp() ? GetApp()->GetPlayset() : nullptr;
	if (PlaysetPtr == nullptr)
	{
		return;
	}
	
	FSlateApplication::Get().DismissAllMenus();

	check(InClass);
	check(InClass->IsChildOf(UPlaysetDataItem::StaticClass()));

	const FScopedTransaction Transaction(LOCTEXT("PlaysetDataList_AddDataObject", "Add Data Object"));
	PlaysetPtr->SetFlags(RF_Transactional);
	PlaysetPtr->Modify();

	FProperty* DataListProperty = FindFProperty<FProperty>(UPlayset::StaticClass(), GET_MEMBER_NAME_CHECKED(UPlayset, DataListObjects));
	PlaysetPtr->PreEditChange(DataListProperty);

	//@TODO: Make sure the item doesnt already exist in the list
	//@TODO: but only if the UPlaysetDataItem has the singleton flag set

	UPlaysetDataItem* NewDataItem = NewObject<UPlaysetDataItem>(PlaysetPtr, InClass);
	PlaysetPtr->DataListObjects.Add(NewDataItem);
	
	UpdateDataList(true);

	// Select the new item
	for (const TSharedRef DataItem : DataList)
	{
		if (DataItem->GetDataItem() == NewDataItem)
		{
			ListView->SetItemSelection(DataItem, true);
			break;
		}
	}

	FPropertyChangedEvent PropertyChangedEvent(DataListProperty, EPropertyChangeType::ArrayAdd);
	PlaysetPtr->PostEditChangeProperty(PropertyChangedEvent);
}

void SPlaysetDataList::HandleSelectionChanged(
	TSharedPtr<FPlaysetDataObjectEntry> InDataItem, ESelectInfo::Type SelectInfo)
{
	TArray<TSharedRef<FPlaysetDataObjectEntry>> SelectedItems = ListView->GetSelectedItems();

	if (SelectedItems.IsEmpty())
	{
		GetApp()->GetDetailsView()->SetObject(nullptr);
	}
	else
	{
		TArray<UObject*> AllObjects;
		for (TSharedRef Selected : SelectedItems)
		{
			if (UPlaysetDataItem* Obj = Selected->GetDataItem())
			{
				AllObjects.Add(Obj);
			}
		}

		GetApp()->GetDetailsView()->SetObjects(AllObjects);
	}
}

void SPlaysetDataList::RegenerateDataListFromSource()
{
	DataList.Empty();
	DataList.Reserve(GetApp()->GetPlayset()->DataListObjects.Num());
	
	for (TObjectPtr DataItem : GetApp_Checked()->GetPlayset()->DataListObjects)
	{
		DataList.Add(MakeShareable(new FPlaysetDataObjectEntry(DataItem, GetApp()->GetPlayset())));
	}
}

FReply SPlaysetDataList::HandleAddDataButton()
{
	UPlayset* Playset = GetApp() ? GetApp()->GetPlayset() : nullptr;
	if (Playset == nullptr)
	{
		return FReply::Handled();
	}
	
	return FReply::Handled();
}

void SPlaysetDataList::HandleNewDataObject()
{
	UPlayset* Playset = GetApp() ? GetApp()->GetPlayset() : nullptr;
	if (Playset == nullptr)
	{
		return;
	}
	
	FSlateApplication::Get().DismissAllMenus();

	const FScopedTransaction Transaction(LOCTEXT("PlaysetDataList_AddDataObject", "Add Data Object"));
	Playset->SetFlags(RF_Transactional);
	Playset->Modify();

	FProperty* DataListProperty = FindFProperty<FProperty>(UPlayset::StaticClass(), GET_MEMBER_NAME_CHECKED(UPlayset, DataListObjects));
	Playset->PreEditChange(DataListProperty);

	Playset->DataListObjects.Add(nullptr);

	FPropertyChangedEvent PropertyChangedEvent(DataListProperty, EPropertyChangeType::ArrayAdd);
	Playset->PostEditChangeProperty(PropertyChangedEvent);

	bItemsDirty = true;
}

void SPlaysetDataList::ClearAllDataObjects()
{
	UPlayset* Playset = GetApp() ? GetApp()->GetPlayset() : nullptr;
	if (Playset == nullptr)
	{
		return;
	}

	FSlateApplication::Get().DismissAllMenus();

	const FScopedTransaction Transaction(LOCTEXT("PlaysetDataList_ClearDataObjects", "Clear Data Objects"));
	Playset->SetFlags(RF_Transactional);
	Playset->Modify();

	FProperty* DataListProperty = FindFProperty<FProperty>(UPlayset::StaticClass(), GET_MEMBER_NAME_CHECKED(UPlayset, DataListObjects));
	Playset->PreEditChange(DataListProperty);

	Playset->DataListObjects.Empty();

	FPropertyChangedEvent PropertyChangedEvent(DataListProperty, EPropertyChangeType::ArrayClear);
	Playset->PostEditChangeProperty(PropertyChangedEvent);
	
	bItemsDirty = true;
}

void SPlaysetDataList::HandleDataObjectDeleted()
{
	UPlayset* Playset = GetApp() ? GetApp()->GetPlayset() : nullptr;
	if (Playset == nullptr)
	{
		return;
	}

	TArray<TSharedRef<FPlaysetDataObjectEntry>> Selection = ListView->GetSelectedItems();
	if (Selection.Num() > 0)
	{
		const FScopedTransaction Transaction(LOCTEXT("PlaysetDataList_DeleteDataObject", "Delete Data Object"));
		for (TSharedRef Entry : Selection)
		{
			const FSetElementId ElementId = Playset->DataListObjects.FindId(Entry->GetDataItem());
			Playset->DataListObjects.Remove(ElementId);
		}

		Playset->DataListObjects.CompactStable();
		ListView->ClearSelection();

		bItemsDirty = true;
	}
}

void SPlaysetDataList::HandleDataObjectDuplicated()
{
}

bool SPlaysetDataList::HasSelection() const
{
	return ListView->GetNumItemsSelected() > 0;
}

bool SPlaysetDataList::CanDuplicateSelection() const
{
	if (!HasSelection())
	{
		return false;
	}

	const TArray<TSharedRef<FPlaysetDataObjectEntry>> Selection = ListView->GetSelectedItems();
	if (Selection.Num() <= 0)
	{
		return false;
	}

	for (const TSharedRef Entry : Selection)
	{
		if (Entry->GetDataItem() == nullptr)
		{
			return false;
		}

		if (Entry->GetDataItem()->bIsSingleton)
		{
			return false;
		}
	}

	return true;
}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION

#undef LOCTEXT_NAMESPACE