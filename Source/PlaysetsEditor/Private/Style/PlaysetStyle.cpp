// Copyright © 2024 MajorT. All rights reserved.


#include "PlaysetStyle.h"

#include "Brushes/SlateBoxBrush.h"
#include "Styling/SlateStyleRegistry.h"
#include "Brushes/SlateImageBrush.h"
#include "Styling/SlateTypes.h"
#include "Misc/Paths.h"
#include "Styling/CoreStyle.h"
#include "Styling/SlateStyle.h"
#include "SlateOptMacros.h"
#include "Brushes/SlateBoxBrush.h"
#include "Brushes/SlateImageBrush.h"
#include "Styling/SlateTypes.h"
#include "Misc/Paths.h"
#include "Styling/CoreStyle.h"
#include "SlateOptMacros.h"
#include "Styling/SlateStyle.h"
#include "Styling/SlateStyleMacros.h"
#include "Styling/SlateStyleRegistry.h"


/*#define IMAGE_PLUGIN_BRUSH_SVG(RelativePath, ...)	FSlateImageBrush(StyleSet->RootToContentDir(RelativePath, TEXT(".svg")), __VA_ARGS__)
#define IMAGE_BRUSH(RelativePath, ...)				FSlateImageBrush(StyleSet->RootToContentDir(RelativePath, TEXT(".png")), __VA_ARGS__)
#define DEFAULT_FONT(...)							FCoreStyle::GetDefaultFontStyle(__VA_ARGS__)*/

TUniquePtr<FPlaysetStyle> FPlaysetStyle::StyleSet = nullptr;

void FPlaysetStyle::Initialize()
{
	// Const icon sizes
	const FVector2D Icon8x8(8.0f, 8.0f);
	const FVector2D Icon16x16(16.0f, 16.0f);
	const FVector2D Icon20x20(20.0f, 20.0f);
	const FVector2D Icon40x40(40.0f, 40.0f);
	const FVector2D Icon64x64(64.0f, 64.0f);
	const FVector2D Icon300x100(300.0f, 100.0f);

	const FTextBlockStyle& NormalText = FAppStyle::Get().GetWidgetStyle<FTextBlockStyle>("NormalText");
	
	FSlateStyleSet::SetContentRoot(FPaths::EngineContentDir() / TEXT("Editor/Slate"));
	FSlateStyleSet::SetCoreContentRoot(FPaths::EngineContentDir() / TEXT("Slate"));

	Set("PlaysetsEditor.Color.Playset", FLinearColor(FColor::FromHex("#26601EFF")));
	
	Set("ClassIcon.PlaysetDataItem_Display", new IMAGE_BRUSH_SVG(TEXT("Starship/AssetIcons/Texture2D_16"), Icon16x16));

	{ // Details
		const FTextBlockStyle DetailsTextStyle = FTextBlockStyle(NormalText)
    		.SetFont(DEFAULT_FONT("Regular", 10))
    		.SetColorAndOpacity(FLinearColor(230.0f / 255.0f, 230.0f / 255.0f, 230.0f / 255.0f, 0.4f));
    	Set("PlaysetsEditor.Text.Details", DetailsTextStyle);
	}

	{ // Name
		const FTextBlockStyle NameTextStyle = FTextBlockStyle(NormalText)
			.SetFont(DEFAULT_FONT("Bold", 12))
			.SetColorAndOpacity(FStyleColors::White);
		Set("PlaysetsEditor.Text.Name", NameTextStyle);
	}

	// Registers this slate style set
	FSlateStyleRegistry::RegisterSlateStyle(*this);
}

/*#undef IMAGE_PLUGIN_BRUSH_SVG
#undef IMAGE_BRUSH
#undef DEFAULT_FONT*/

void FPlaysetStyle::Shutdown()
{
	if (StyleSet.IsValid())
	{
		FSlateStyleRegistry::UnRegisterSlateStyle(*StyleSet.Get());
		ensure(StyleSet);
		StyleSet.Reset();
	}
}

FPlaysetStyle::FPlaysetStyle()
	: FSlateStyleSet("PlaysetStyle")
{
	Initialize();
}

FPlaysetStyle::~FPlaysetStyle()
{
	Shutdown();
}

FPlaysetStyle& FPlaysetStyle::Get()
{
	if (!StyleSet.IsValid())
	{
		StyleSet = MakeUnique<FPlaysetStyle>();
	}

	return *StyleSet.Get();
}