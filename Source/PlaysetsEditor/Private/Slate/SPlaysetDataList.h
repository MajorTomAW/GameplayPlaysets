// Copyright © 2024 MajorT. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "ClassViewerFilter.h"
#include "PlaysetDataItem.h"
#include "Widgets/SCompoundWidget.h"

class UPlayset;
class UPlaysetDataItem;
class FPlaysetEditorApp;

class FPlaysetDataListEntryClassFilter : public IClassViewerFilter
{
public:
	FPlaysetDataListEntryClassFilter(UPlayset* InPlayset)
		: Playset(InPlayset)
	{
	}
	
	virtual bool IsClassAllowed(const FClassViewerInitializationOptions& InInitOptions, const UClass* InClass, TSharedRef<class FClassViewerFilterFuncs> InFilterFuncs) override;
	virtual bool IsUnloadedClassAllowed(const FClassViewerInitializationOptions& InInitOptions, const TSharedRef<const class IUnloadedBlueprintData> InUnloadedClassData, TSharedRef<class FClassViewerFilterFuncs> InFilterFuncs) override;

protected:
	TWeakObjectPtr<UPlayset> Playset;
};

struct FPlaysetDataObjectEntry
{
	FPlaysetDataObjectEntry(UPlaysetDataItem* InDataItem, UPlayset* InPlayset)
		: DataItem(InDataItem)
		, Playset(InPlayset)
	{
	}

	void UpdateDataItem(UPlaysetDataItem* InDataItem)
	{
		DataItem = InDataItem;
	}

public:
	FString GetDescription() const;
	FString GetDisplayName() const;
	UPlaysetDataItem* GetDataItem() const { return DataItem.Get(); }
	UPlayset* GetPlayset() const { return Playset.Get(); }
	
protected:
	TWeakObjectPtr<UPlaysetDataItem> DataItem;
	TWeakObjectPtr<UPlayset> Playset;
};

class SPlaysetDataList : public SCompoundWidget
{
public:
	using Self = SPlaysetDataList;
	SLATE_BEGIN_ARGS(SPlaysetDataList)
		{
		}
		SLATE_ARGUMENT(float, ItemHeight);
	SLATE_END_ARGS()

	SPlaysetDataList();
	
	void Construct(const FArguments& InArgs, TSharedPtr<FPlaysetEditorApp> InPlaysetApp, const TSharedRef<FUICommandList>& InCommandList);
	void BindCommands();
	
	virtual void Tick(const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime) override;
	virtual FReply OnKeyDown(const FGeometry& MyGeometry, const FKeyEvent& InKeyEvent) override;

	bool IsSelected(const UPlaysetDataItem* InDataItem) const;
	bool IsSelected(const TWeakPtr<FPlaysetDataObjectEntry>& InDataItem) const;

	void UpdateDataList(bool bForce = false);

	void MarkItemsDirty() { bItemsDirty = true; }
	
	TSharedPtr<FPlaysetEditorApp> GetApp() const { return AppPtr.Pin(); }
	TSharedPtr<FPlaysetEditorApp> GetApp_Checked() const
	{
		TSharedPtr<FPlaysetEditorApp> App = GetApp();
		check(App.IsValid());
		return App;
	}

protected:
	TSharedRef<ITableRow> HandleGenerateRow(TSharedRef<FPlaysetDataObjectEntry> InDataItem, const TSharedRef<STableViewBase>& OwnerTable);
	TSharedRef<SWidget> HandleCreateNewDataMenuEntry() const;
	TSharedPtr<SWidget> HandleContextMenuOpening();
	
	virtual void HandleSelectionChanged(TSharedPtr<FPlaysetDataObjectEntry> InDataItem, ESelectInfo::Type SelectInfo);
	void HandleDataObjectClassPicked(UClass* InClass);
	
	void RegenerateDataListFromSource();

	FReply HandleAddDataButton();

	void HandleNewDataObject();
	void ClearAllDataObjects();


	// Action handlers
	void HandleDataObjectDeleted();
	void HandleDataObjectDuplicated();

	bool HasSelection() const;
	bool CanDuplicateSelection() const;

private:
	TWeakPtr<FPlaysetEditorApp> AppPtr;
	TSharedPtr<SListView<TSharedRef<FPlaysetDataObjectEntry>>> ListView;
	TSharedPtr<SScrollBox> ViewBox;
	TSharedPtr<SScrollBar> ScrollBar;
	
	TArray<TSharedRef<FPlaysetDataObjectEntry>> DataList;

	TSharedPtr<FUICommandList> CommandList;

	float ItemHeight = 0.f;
	bool bItemsDirty;
};
