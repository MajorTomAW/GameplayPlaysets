// Copyright © 2024 MajorT. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "ContentBrowserDelegates.h"
#include "Widgets/SCompoundWidget.h"

class SPlaysetConverter;
/**
 * 
 */
class SPlaysetConverterBottomToolbar : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SPlaysetConverterBottomToolbar)
		{
		}
		SLATE_EVENT(FOnAssetsActivated, OnAssetsActivated)
	SLATE_END_ARGS()

	/** Constructs this widget with InArgs */
	void Construct(const FArguments& InArgs, const TSharedRef<SPlaysetConverter>& InOwner);

protected:
	TSharedPtr<SPlaysetConverter> Owner;

	bool IsSaveButtonEnabled() const;
	FReply HandleSaveButtonClicked();

private:
	FOnAssetsActivated OnAssetsActivatedDelegate;
};
