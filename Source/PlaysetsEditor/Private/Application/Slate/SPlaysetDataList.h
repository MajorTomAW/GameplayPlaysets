// Copyright © 2024 MajorT. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"
#include "Widgets/Views/STileView.h"

class FPlaysetEditorApp;
class FPlaysetApplicationModeBase;
class UPlayset;
class UPlaysetDataItem;

struct FPlaysetDataObjectEntry
{
public:
	TWeakObjectPtr<UPlaysetDataItem> DataItem;

	FString GetDescription() const;
	FString GetDisplayName() const;
};

class SPlaysetDataList : public SCompoundWidget
{
public:
	using Self = SPlaysetDataList;
	
	SLATE_BEGIN_ARGS(SPlaysetDataList)
		{
		}

	SLATE_END_ARGS()

	/** Constructs this widget with InArgs */
	void Construct(const FArguments& InArgs, TSharedPtr<FPlaysetEditorApp> InAppBase);

protected:
	TSharedRef<ITableRow> HandleGenerateRow(TSharedRef<FPlaysetDataObjectEntry> InDataItem, const TSharedRef<STableViewBase>& OwnerTable);
	virtual void HandleSelectionChanged(TSharedPtr<FPlaysetDataObjectEntry> InDataItem, ESelectInfo::Type SelectInfo);

protected:
	TSharedPtr<FPlaysetEditorApp> AppBase;
	TSharedPtr<SListView<TSharedRef<FPlaysetDataObjectEntry>>> ListView;
	TSharedPtr<SScrollBox> ViewBox;
	TSharedPtr<SScrollBar> ExternalScrollbar;

	TArray<TSharedRef<FPlaysetDataObjectEntry>> DataList;
};
