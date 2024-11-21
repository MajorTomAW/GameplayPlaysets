// Copyright © 2024 MajorT. All rights reserved.


#include "SPlaysetConverterWindow.h"

#include "SlateOptMacros.h"
#include "SPlaysetConverter.h"

#define LOCTEXT_NAMESPACE "PlaysetConverterWindow"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION

SPlaysetConverterWindow::SPlaysetConverterWindow()
	: bProceedWithAction(false)
{
}

void SPlaysetConverterWindow::Construct(const FArguments& InArgs)
{
	SWindow::Construct(SWindow::FArguments()
		.Title(LOCTEXT("PlaysetConverterWindowTitle", "Playset Converter"))
		.SizingRule(ESizingRule::UserSized)
		.ClientSize(FVector2d(800.f, 720.f))
		.SupportsMaximize(true)
		.SupportsMinimize(false)
		[
			SAssignNew(PlaysetConverter, SPlaysetConverter)
			.SelectedActors(InArgs._SelectedActors)
			.OnAssetsActivated(this, &SPlaysetConverterWindow::HandleAssetsActivated)
		]);
}

UPlaysetConverterOptionsObject* SPlaysetConverterWindow::GetOptionsObject() const
{
	if (PlaysetConverter.IsValid())
	{
		return PlaysetConverter->GetOptionsObject();
	}

	return nullptr;
}

void SPlaysetConverterWindow::HandleAssetsActivated(const TArray<FAssetData>& InAssets, EAssetTypeActivationMethod::Type ActivationMethod)
{
	SelectedAssets = InAssets;
	bProceedWithAction = InAssets.Num() > 0;
	RequestDestroyWindow();
}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION

#undef LOCTEXT_NAMESPACE