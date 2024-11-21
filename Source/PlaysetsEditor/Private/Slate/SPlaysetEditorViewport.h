// Copyright © 2024 MajorT. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "SCommonEditorViewportToolbarBase.h"
#include "SEditorViewport.h"

class UPlayset;
class FAdvancedPreviewScene;
class FPlaysetEditorApp;

class FPlaysetEditorViewportClient : public FEditorViewportClient
{
public:
	FPlaysetEditorViewportClient(FAdvancedPreviewScene& InPreviewScene, const TSharedRef<class SPlaysetEditorViewport>& InViewport);

protected:
	TWeakPtr<class SPlaysetEditorViewport> ViewportPtr;
	FAdvancedPreviewScene* AdvancedPreviewScene = nullptr;
};

class SPlaysetEditorViewport : public SEditorViewport, public FGCObject, public ICommonEditorViewportToolbarInfoProvider
{
public:
	SLATE_BEGIN_ARGS(SPlaysetEditorViewport)
		{
		}

	SLATE_END_ARGS()
	
	void Construct(const FArguments& InArgs, TSharedPtr<FPlaysetEditorApp> InPlaysetApp);
	virtual ~SPlaysetEditorViewport();

	//~ Begin SEditorViewport Interface
	virtual TSharedRef<FEditorViewportClient> MakeEditorViewportClient() override;
	virtual TSharedPtr<SWidget> MakeViewportToolbar() override;
	virtual void PopulateViewportOverlays(TSharedRef<SOverlay> Overlay) override;
	//~ End SEditorViewport Interface

	//~ Begin ICyberEditorViewportToolbarInfoProvider Interface
	virtual TSharedRef<class SEditorViewport> GetViewportWidget() override;
	virtual TSharedPtr<FExtender> GetExtenders() const override;
	virtual void OnFloatingButtonClicked() override;
	//~ End ICyberEditorViewportToolbarInfoProvider Interface

	virtual void AddReferencedObjects(FReferenceCollector& Collector) override;
	virtual FString GetReferencerName() const override { return TEXT("SPlaysetEditorViewport"); }

	void UpdateViewport(UPlayset* OwningPlayset);

protected:
	void CleanUpScene();

protected:
	TObjectPtr<USceneComponent> PreviewComponent;
	TArray<TObjectPtr<USceneComponent>> AdditionalComponents;
	
	TWeakPtr<FPlaysetEditorApp> AppPtr;
	TSharedPtr<FAdvancedPreviewScene> AdvancedPreviewScene;
	TSharedPtr<class FEditorViewportClient> SystemViewportClient;
	TSharedPtr<STextBlock> StatusText;

private:
	FText GetViewportStatusText() const;
};

