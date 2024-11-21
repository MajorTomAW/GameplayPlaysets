// Copyright © 2024 MajorT. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "SPlaysetConverterSelectedActorsList.h"
#include "Widgets/SCompoundWidget.h"

/**
 * 
 */
class SPlaysetConverterSelectedActorsListRow : public STableRow<TSharedRef<FPlaysetSelectedActorEntry>>
{
public:
	using Self = SPlaysetConverterSelectedActorsListRow;
	SLATE_BEGIN_ARGS(SPlaysetConverterSelectedActorsListRow)
		{
		}
	SLATE_END_ARGS()
	
	void Construct(const FArguments& InArgs, const TSharedRef<STableViewBase>& InOwnerTable, TSharedRef<FPlaysetSelectedActorEntry>& InEntry, const TSharedRef<SPlaysetConverterSelectedActorsList>& InOwnerList);

protected:
	TSharedPtr<FPlaysetSelectedActorEntry> Entry;
	TSharedPtr<SPlaysetConverterSelectedActorsList> OwnerList;
	TSharedPtr<STextBlock> LabelText;
	TSharedPtr<IToolTip> MakeToolTip() const;
	
	ECheckBoxState IsItemEnabled() const;
	void HandleCheckStateChanged(ECheckBoxState NewState);

	bool IsEntryChecked() const;

	FSlateColor GetIconColor() const;
	FSlateColor GetTextColor() const;
	const FTextBlockStyle* GetTextStyle() const;
};
