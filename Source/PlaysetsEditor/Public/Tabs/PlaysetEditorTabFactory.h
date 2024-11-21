// Copyright © 2024 MajorT. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "WorkflowOrientedApp/WorkflowTabFactory.h"

struct FPlaysetEditorTabFactory : FWorkflowTabFactory
{
	FPlaysetEditorTabFactory(FName InTabID, TSharedPtr<class FPlaysetEditorApp> InPlaysetApp);

	//~ Begin FWorkflowTabFactory interface
	virtual TSharedRef<SWidget> CreateTabBody(const FWorkflowTabSpawnInfo& Info) const override;
	virtual FText GetTabToolTipText(const FWorkflowTabSpawnInfo& Info) const override;
	//~ End FWorkflowTabFactory interface

protected:
	TWeakPtr<class FPlaysetEditorApp> AppPtr;
};

struct FPlaysetTabFactory_Details final : FPlaysetEditorTabFactory
{
	explicit FPlaysetTabFactory_Details(TSharedPtr<class FPlaysetEditorApp> InPlaysetApp);

	//~ Begin FWorkflowTabFactory interface
	virtual TSharedRef<SWidget> CreateTabBody(const FWorkflowTabSpawnInfo& Info) const override;
	virtual FText GetTabToolTipText(const FWorkflowTabSpawnInfo& Info) const override;
	//~ End FWorkflowTabFactory interface
};

struct FPlaysetTabFactory_Viewport final : FPlaysetEditorTabFactory
{
	explicit  FPlaysetTabFactory_Viewport(TSharedPtr<FPlaysetEditorApp> InPlaysetApp);

	//~ Begin FWorkflowTabFactory interface
	virtual TSharedRef<SWidget> CreateTabBody(const FWorkflowTabSpawnInfo& Info) const override;
	virtual FText GetTabToolTipText(const FWorkflowTabSpawnInfo& Info) const override;
	//~ End FWorkflowTabFactory interface
};

struct FPlaysetTabFactory_DisplayInfo final : FPlaysetEditorTabFactory
{
	explicit FPlaysetTabFactory_DisplayInfo(TSharedPtr<FPlaysetEditorApp> InPlaysetApp);

	//~ Begin FWorkflowTabFactory interface
	virtual TSharedRef<SWidget> CreateTabBody(const FWorkflowTabSpawnInfo& Info) const override;
	virtual FText GetTabToolTipText(const FWorkflowTabSpawnInfo& Info) const override;
	//~ End FWorkflowTabFactory interface
};

struct FPlaysetTabFactory_Placement final : FPlaysetEditorTabFactory
{
	explicit FPlaysetTabFactory_Placement(TSharedPtr<FPlaysetEditorApp> InPlaysetApp);

	//~ Begin FWorkflowTabFactory interface
	virtual TSharedRef<SWidget> CreateTabBody(const FWorkflowTabSpawnInfo& Info) const override;
	virtual FText GetTabToolTipText(const FWorkflowTabSpawnInfo& Info) const override;
	//~ End FWorkflowTabFactory interface
};

struct FPlaysetTabFactory_DataList final : FPlaysetEditorTabFactory
{
	explicit FPlaysetTabFactory_DataList(TSharedPtr<FPlaysetEditorApp> InPlaysetApp);

	//~ Begin FWorkflowTabFactory interface
	virtual TSharedRef<SWidget> CreateTabBody(const FWorkflowTabSpawnInfo& Info) const override;
	virtual FText GetTabToolTipText(const FWorkflowTabSpawnInfo& Info) const override;
	//~ End FWorkflowTabFactory interface
};