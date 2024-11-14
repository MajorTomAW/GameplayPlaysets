// Copyright © 2024 MajorT. All rights reserved.

#pragma once

#include "WorkflowOrientedApp/WorkflowCentricApplication.h"

class FPlaysetEditorApp : public FWorkflowCentricApplication, public FNotifyHook, public FEditorUndoClient
{
public:
	friend class FPlaysetApplicationModeBase;
	FPlaysetEditorApp();

	virtual void InitPlaysetEditorApp(const EToolkitMode::Type Mode, const TSharedPtr<class IToolkitHost>& InitToolkitHost, UObject* Object);

	//~ Begin FAssetEditorToolkit Interface
	virtual FName GetToolkitFName() const override;
	virtual FText GetBaseToolkitName() const override;
	virtual FText GetToolkitName() const override;
	virtual FText GetToolkitToolTipText() const override;
	virtual FString GetWorldCentricTabPrefix() const override;
	virtual FString GetDocumentationLink() const override;
	virtual FLinearColor GetWorldCentricTabColorScale() const override;

	virtual void OnToolkitHostingFinished(const TSharedRef<IToolkit>& Toolkit) override;
	virtual void OnToolkitHostingStarted(const TSharedRef<IToolkit>& Toolkit) override;
	virtual void SetCurrentMode(FName NewMode) override;

	virtual void SaveAsset_Execute() override;
	//~ End FAssetEditorToolkit Interface

public:
	class UPlayset* Playset;
};

