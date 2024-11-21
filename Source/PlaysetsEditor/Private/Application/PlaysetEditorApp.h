// Copyright © 2024 MajorT. All rights reserved.

#pragma once

#include "Details/PlaysetDetailCustomization.h"
#include "WorkflowOrientedApp/WorkflowCentricApplication.h"

class SPlaysetEditorViewport;
class UPlayset;
class FPlaysetEditorToolbar;

class FPlaysetEditorApp : public FWorkflowCentricApplication, public FNotifyHook, public FEditorUndoClient
{
public:
	friend class FPlaysetAppModeBase;
	FPlaysetEditorApp();

	static FText GetLocalizedMode(FName InMode);
	void CreatePlaysetEditorApp(const EToolkitMode::Type Mode, const TSharedPtr<IToolkitHost>& InitToolkitHost, UObject* InObject);

	//~ Begin FWorkflowCentricApplication interface
	virtual void RegisterTabSpawners(const TSharedRef<FTabManager>& InTabManager) override;
	virtual void SetCurrentMode(FName NewMode) override;
	//~ End FWorkflowCentricApplication interface

	//~ Begin FNotifyHook interface
	virtual void NotifyPostChange(const FPropertyChangedEvent& PropertyChangedEvent, FProperty* PropertyThatChanged) override;
	//~ End FNotifyHook interface

	//~ Begin FEditorUndoClient interface
	virtual void PostUndo(bool bSuccess) override;
	virtual void PostRedo(bool bSuccess) override;
	//~ End FEditorUndoClient interface

	//~ Begin IToolkit interface
	virtual FName GetToolkitFName() const override;
	virtual FText GetBaseToolkitName() const override;
	virtual FText GetToolkitName() const override;
	virtual FText GetToolkitToolTipText() const override;
	virtual FString GetWorldCentricTabPrefix() const override;
	virtual FLinearColor GetWorldCentricTabColorScale() const override;
	//~ End IToolkit interface

	//~ Begin IAssetEditorInstance interface
	virtual bool IncludeAssetInRestoreOpenAssetsPrompt(UObject* Asset) const override;
	//~ End IAssetEditorInstance interface

public:
	TSharedRef<SWidget> SpawnTab_Details();
	TSharedRef<SWidget> SpawnTab_Viewport();
	TSharedRef<SWidget> SpawnTab_DisplayInfo();
	TSharedRef<SWidget> SpawnTab_Placement();
	TSharedRef<SWidget> SpawnTab_DataList();

	UPlayset* GetPlayset() const { return Playset; }
	TSharedPtr<FPlaysetEditorToolbar> GetToolbarBuilder() const { return ToolbarBuilder; }
	TSharedPtr<IDetailsView> GetDetailsView() const { return PrimaryDetailsView; }
	TSharedPtr<SPlaysetEditorViewport> GetViewport() const { return Viewport; }
	
	virtual bool ShouldSpawnDetailsForPlayset(const UPlayset* InPlayset) const { return true; }
	virtual bool ShouldSpawnViewportForPlayset(const UPlayset* InPlayset) const { return true; }
	virtual bool ShouldSpawnDisplayInfoForPlayset(const UPlayset* InPlayset) const { return true; }
	virtual bool ShouldSpawnPlacementForPlayset(const UPlayset* InPlayset) const { return true; }

protected:
	void CreateDetailsView();
	void OnFinishedChangingProperties(const FPropertyChangedEvent& PropertyChangedEvent);

private:
	TSharedPtr<SPlaysetEditorViewport> Viewport;
	
	TSharedPtr<IDetailsView> PrimaryDetailsView;
	TSharedPtr<IDetailsView> SecondaryDetailsView;
	TSharedPtr<IDetailsView> DisplayDetailsView;
	
	FOnGetDetailCustomizationInstance GetPrimaryDetailCustomization();
	FOnGetDetailCustomizationInstance GetSecondaryDetailCustomization();
	FOnGetDetailCustomizationInstance GetDisplayDetailCustomization();
	
	
	TSharedPtr<FPlaysetEditorToolbar> ToolbarBuilder;
	TSharedPtr<FUICommandList> DataListCommandList;
	
	UPlayset* Playset;
};

inline FOnGetDetailCustomizationInstance FPlaysetEditorApp::GetPrimaryDetailCustomization()
{
	return FOnGetDetailCustomizationInstance::CreateLambda([this]()
	{
		return FPlaysetDetailCustomization::MakeInstance(FPlaysetDetailsViewArgs::Default(), SharedThis(this));
	});
}

inline FOnGetDetailCustomizationInstance FPlaysetEditorApp::GetSecondaryDetailCustomization()
{
	return FOnGetDetailCustomizationInstance::CreateLambda([this]()
	{
		return FPlaysetDetailCustomization::MakeInstance(FPlaysetDetailsViewArgs::Placement(), SharedThis(this));
	});
}

inline FOnGetDetailCustomizationInstance FPlaysetEditorApp::GetDisplayDetailCustomization()
{
	return FOnGetDetailCustomizationInstance::CreateLambda([this]()
	{
		return FPlaysetDetailCustomization::MakeInstance(FPlaysetDetailsViewArgs::DisplayInfo(), SharedThis(this));
	});
}