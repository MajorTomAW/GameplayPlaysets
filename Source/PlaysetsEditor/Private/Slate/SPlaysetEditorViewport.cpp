// Copyright © 2024 MajorT. All rights reserved.


#include "SPlaysetEditorViewport.h"

#include "AdvancedPreviewScene.h"
#include "NiagaraEditorStyle.h"
#include "SlateOptMacros.h"

#define LOCTEXT_NAMESPACE "PlaysetEditorViewport"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION

FPlaysetEditorViewportClient::FPlaysetEditorViewportClient(FAdvancedPreviewScene& InPreviewScene, const TSharedRef<SPlaysetEditorViewport>& InViewport)
	: FEditorViewportClient(nullptr, &InPreviewScene, InViewport)
{
	DrawHelper.bDrawGrid = true;
	DrawHelper.GridColorAxis = FColor(180,80,80);
	DrawHelper.GridColorMajor = FColor(72,72,72);
	DrawHelper.GridColorMinor = FColor(64,64,64);
	DrawHelper.PerspectiveGridSize = UE_BIG_NUMBER;

	EngineShowFlags.SetSnap(false);
	OverrideNearClipPlane(1.f);
	bUsingOrbitCamera = false;
}

void SPlaysetEditorViewport::Construct(const FArguments& InArgs, TSharedPtr<FPlaysetEditorApp> InPlaysetApp)
{
	AppPtr = InPlaysetApp;
	
	AdvancedPreviewScene = MakeShareable(new FAdvancedPreviewScene(FPreviewScene::ConstructionValues()));
	AdvancedPreviewScene->SetFloorVisibility(false);

	const FRotator LightDir(-40.f, 128.f, 0.f);
	AdvancedPreviewScene->SetLightDirection(LightDir);

	SEditorViewport::Construct(SEditorViewport::FArguments());
	Client->EngineShowFlags.SetGrid(true);
	Client->EngineShowFlags.SetPivot(true);
}

SPlaysetEditorViewport::~SPlaysetEditorViewport()
{
	AdvancedPreviewScene.Reset();
}

TSharedRef<FEditorViewportClient> SPlaysetEditorViewport::MakeEditorViewportClient()
{
	SystemViewportClient = MakeShareable(new FPlaysetEditorViewportClient(*AdvancedPreviewScene.Get(), SharedThis(this)));
	{
		SystemViewportClient->SetViewLocation(FVector::ZeroVector);
		SystemViewportClient->SetViewRotation(FRotator::ZeroRotator);
		SystemViewportClient->SetViewLocationForOrbiting(FVector::ZeroVector, 750.f);
		SystemViewportClient->bSetListenerPosition = false;
		SystemViewportClient->SetRealtime(true);
		SystemViewportClient->SetGameView(false);
		SystemViewportClient->ExposureSettings.bFixed = true;
		SystemViewportClient->ExposureSettings.FixedEV100 = -1.f;
		SystemViewportClient->SetShowStats(false);
	}
	
	return SystemViewportClient.ToSharedRef();
}

TSharedPtr<SWidget> SPlaysetEditorViewport::MakeViewportToolbar()
{
	return SNew(SCommonEditorViewportToolbarBase, SharedThis(this))
		.AddRealtimeButton(true);
}

void SPlaysetEditorViewport::PopulateViewportOverlays(TSharedRef<SOverlay> Overlay)
{
	Overlay->AddSlot()
	.VAlign(VAlign_Center)
	.HAlign(HAlign_Center)
	[
		SAssignNew(StatusText, STextBlock)
		.Text_Raw(this, &SPlaysetEditorViewport::GetViewportStatusText)
		.TextStyle(FNiagaraEditorStyle::Get(), "NiagaraEditor.Viewport.CompileOverlay")
		.ColorAndOpacity(FLinearColor::White)
		.ShadowOffset(0.8f)
		.ShadowColorAndOpacity(FLinearColor(0.f, 0.f, 0.f, 0.9f))
	];
	
	SEditorViewport::PopulateViewportOverlays(Overlay);
}

TSharedRef<class SEditorViewport> SPlaysetEditorViewport::GetViewportWidget()
{
	return SharedThis(this);
}

TSharedPtr<FExtender> SPlaysetEditorViewport::GetExtenders() const
{
	TSharedPtr<FExtender> Result(MakeShareable(new FExtender));
	return Result;
}

void SPlaysetEditorViewport::OnFloatingButtonClicked()
{
}

void SPlaysetEditorViewport::AddReferencedObjects(FReferenceCollector& Collector)
{
	if (PreviewComponent != nullptr)
	{
		Collector.AddReferencedObject(PreviewComponent);
	}
}

FText SPlaysetEditorViewport::GetViewportStatusText() const
{
	if (PreviewComponent)
	{
		return FText::GetEmpty();
	}

	return LOCTEXT("PlaysetEditorViewportNoPreview", "Nothing to show here.");
}


END_SLATE_FUNCTION_BUILD_OPTIMIZATION

#undef LOCTEXT_NAMESPACE