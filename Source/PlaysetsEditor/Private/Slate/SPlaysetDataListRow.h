// Copyright © 2024 MajorT. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"

class SPlaysetDataList;
struct FPlaysetDataObjectEntry;

class SPlaysetDataListRow : public STableRow<TSharedRef<FPlaysetDataObjectEntry>>
{
public:
	SLATE_BEGIN_ARGS(SPlaysetDataListRow)
		: _HeightOverride(28.f)
		{
		}
		SLATE_ARGUMENT(float_t, HeightOverride)
	SLATE_END_ARGS()
	
	void Construct(const FArguments& InArgs, const TSharedRef<STableViewBase>& InOwnerTableView, TSharedRef<FPlaysetDataObjectEntry> InDataItem, const TSharedPtr<SScrollBox>& InViewBox, const TSharedPtr<SPlaysetDataList>& InOwnerList);
	virtual void Tick(const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime) override;

	void OnFinishedChangingProperties(const FPropertyChangedEvent& PropertyChangedEvent);
	TSharedRef<SWidget> HandleCreateNewDataMenuEntry() const;
	TSharedRef<SWidget> MakeInsertDeleteDuplicateMenu() const;

	void HandleClassPicked(UClass* InClass);
	
	void HandleDataItemDeleted();
	void HandleDataItemDuplicate();

protected:
	FSlateColor GetActiveColor() const;
	FSlateColor GetBackgroundColor() const;

private:
	TSharedPtr<SWidgetSwitcher> WidgetSwitcher;
	TSharedPtr<SPlaysetDataList> OwnerList;
	TWeakPtr<FPlaysetDataObjectEntry> DataItem;

	bool bRowDirty = false;
};
