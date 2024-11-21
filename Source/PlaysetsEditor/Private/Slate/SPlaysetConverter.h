// Copyright © 2024 MajorT. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "ContentBrowserDelegates.h"
#include "Widgets/SCompoundWidget.h"

class UPlaysetConverterOptionsObject;
struct FPlaysetSelectedActorEntry;
class SPlaysetConverterBottomToolbar;
class SPlaysetConverterSelectedActorsList;
class SPlaysetConverterOptions;


class SPlaysetConverter : public SCompoundWidget
{
public:
	friend class SPlaysetConverterWindow;
	SLATE_BEGIN_ARGS(SPlaysetConverter)
		{
		}
		SLATE_ARGUMENT(TArray<TWeakObjectPtr<AActor>>, SelectedActors)
		SLATE_ARGUMENT(TSharedPtr<SWidget>, BottomToolbar)
		SLATE_EVENT(FOnAssetsActivated, OnAssetsActivated)
	SLATE_END_ARGS()
	
	void Construct(const FArguments& InArgs);

	TArray<TSharedRef<FPlaysetSelectedActorEntry>> GetSelectedActorEntries() const;
	UPlaysetConverterOptionsObject* GetOptionsObject() const;

private:
	TSharedPtr<SPlaysetConverterOptions> ConverterOptions;
	TSharedPtr<SPlaysetConverterSelectedActorsList> SelectedActorsList;
	TSharedPtr<SPlaysetConverterBottomToolbar> BottomToolbar;
};
