// Copyright © 2024 MajorT. All rights reserved.

#pragma once

#include "WorkflowOrientedApp/ApplicationMode.h"
#include "WorkflowOrientedApp/WorkflowTabManager.h"

class FPlaysetEditorApp;

/**
 * Default parent mode used for the Playset Editor.
 */
class FPlaysetEditorModeBase : public FApplicationMode
{
public:
	FPlaysetEditorModeBase(FName InModeName, TSharedPtr<FPlaysetEditorApp> InPlaysetApp);

	//~ Begin FApplicationMode interface
	virtual void RegisterTabFactories(TSharedPtr<FTabManager> InTabManager) override;
	virtual void PreDeactivateMode() override;
	virtual void PostActivateMode() override;
	//~ End FApplicationMode interface

protected:
	TWeakPtr<FPlaysetEditorApp> AppPtr;

	// Set of spawnable tabs for this mode
	FWorkflowAllowedTabSet PlaysetTabFactories;
};

/**
 * Default mode used for the Playset Editor.
 */
class FPlaysetEditorMode_Default final : public FPlaysetEditorModeBase
{
public:
	FPlaysetEditorMode_Default(TSharedPtr<class FPlaysetEditorApp> InPlaysetApp);
};

/**
 * Data list mode used for the Playset Editor.
 */
class FPlaysetEditorMode_DataList final : public FPlaysetEditorModeBase
{
public:
	FPlaysetEditorMode_DataList(TSharedPtr<class FPlaysetEditorApp> InPlaysetApp);
};