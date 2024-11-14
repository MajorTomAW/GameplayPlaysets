// Copyright © 2024 MajorT. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "PlaysetDataItem.h"
#include "Widgets/SCompoundWidget.h"


struct FPlaysetDataObjectEntry;
/**
 * 
 */
class SPlaysetDataListRow : public STableRow<TSharedRef<FPlaysetDataObjectEntry>>
{
public:
	SLATE_BEGIN_ARGS(SPlaysetDataListRow)
		{
		}
	SLATE_END_ARGS()

	/** Constructs this widget with InArgs */
	void Construct(const FArguments& InArgs, const TSharedRef<STableViewBase>& InOwnerTableView, TSharedRef<FPlaysetDataObjectEntry> InDataItem, const TSharedPtr<SScrollBox>& ViewBox);

	uint32 bIsSelected :1 = false;
};
