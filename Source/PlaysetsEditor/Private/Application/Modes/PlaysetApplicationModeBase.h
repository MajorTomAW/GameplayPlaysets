// Copyright © 2024 MajorT. All rights reserved.

#pragma once

#include "WorkflowOrientedApp/ApplicationMode.h"

class FPlaysetApplicationModeBase : public FApplicationMode
{
public:
	using ThisClass = FPlaysetApplicationModeBase;
	
	FPlaysetApplicationModeBase(FName InModeName, TWeakPtr<class FPlaysetEditorApp> InPlaysetEditorApp);
	virtual void RegisterTabFactories(TSharedPtr<FTabManager> InTabManager) override;
	virtual void PostActivateMode() override;

public:
	TWeakPtr<FPlaysetEditorApp> AppPtr;
	TSharedPtr<class IDetailsView> DetailsView;

protected:
	virtual TSharedRef<SDockTab> SpawnTab_Preview(const FSpawnTabArgs& Args) const;
	virtual TSharedRef<SDockTab> SpawnTab_DisplayInfo(const FSpawnTabArgs& Args) const;
	virtual TSharedRef<SDockTab> SpawnTab_Details(const FSpawnTabArgs& Args);
	virtual TSharedRef<SDockTab> SpawnTab_DataList(const FSpawnTabArgs& Args) const;
};

