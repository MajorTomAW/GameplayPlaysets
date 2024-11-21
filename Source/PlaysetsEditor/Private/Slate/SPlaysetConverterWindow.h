// Copyright © 2024 MajorT. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "ContentBrowserDelegates.h"
#include "IAssetTypeActions.h"
#include "Widgets/SCompoundWidget.h"

class UPlaysetConverterOptionsObject;

class SPlaysetConverterWindow : public SWindow
{
public:
	SPlaysetConverterWindow();
	
	SLATE_BEGIN_ARGS(SPlaysetConverterWindow)
		{
		}
		SLATE_ARGUMENT(TArray<TWeakObjectPtr<AActor>>, SelectedActors)
	SLATE_END_ARGS()
	
	void Construct(const FArguments& InArgs);

	bool ShouldProceedWithAction() const { return bProceedWithAction; }
	TArray<FAssetData> GetSelectedAssets() const { return SelectedAssets; }
	UPlaysetConverterOptionsObject* GetOptionsObject() const;

protected:
	TSharedPtr<class SPlaysetConverter> PlaysetConverter;
	void HandleAssetsActivated(const TArray<FAssetData>& InAssets, EAssetTypeActivationMethod::Type ActivationMethod);

private:
	bool bProceedWithAction;
	TArray<FAssetData> SelectedAssets;
};
