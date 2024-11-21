// Copyright © 2024 MajorT. All rights reserved.


#include "Modes/PlaysetEditorModes.h"

#include "PlaysetsEditor.h"
#include "Application/PlaysetEditorApp.h"
#include "Tabs/PlaysetEditorTabFactory.h"
#include "Toolbar/PlaysetEditorToolbar.h"


FPlaysetEditorModeBase::FPlaysetEditorModeBase(FName InModeName, TSharedPtr<class FPlaysetEditorApp> InPlaysetApp)
	: FApplicationMode(InModeName, FPlaysetEditorApp::GetLocalizedMode)
{
	AppPtr = InPlaysetApp;
}

void FPlaysetEditorModeBase::RegisterTabFactories(TSharedPtr<FTabManager> InTabManager)
{
	check(AppPtr.IsValid());
	TSharedPtr<FPlaysetEditorApp> App = AppPtr.Pin();

	// Register tab factories
	App->PushTabFactories(PlaysetTabFactories);
	
	FApplicationMode::RegisterTabFactories(InTabManager);
}

void FPlaysetEditorModeBase::PreDeactivateMode()
{
	FApplicationMode::PreDeactivateMode();

	check(AppPtr.IsValid());
	TSharedPtr<FPlaysetEditorApp> App = AppPtr.Pin();

	// App->SaveEditedObjectState();
}

void FPlaysetEditorModeBase::PostActivateMode()
{
	FApplicationMode::PostActivateMode();
}

FPlaysetEditorMode_Default::FPlaysetEditorMode_Default(TSharedPtr<FPlaysetEditorApp> InPlaysetApp)
	: FPlaysetEditorModeBase(PlaysetEditorIDs::Mode::PlaysetMode_Default, InPlaysetApp)
{
	if (InPlaysetApp->ShouldSpawnDetailsForPlayset(InPlaysetApp->GetPlayset()))
	{
		PlaysetTabFactories.RegisterFactory(MakeShareable(new FPlaysetTabFactory_Details(InPlaysetApp)));
	}
	if (InPlaysetApp->ShouldSpawnViewportForPlayset(InPlaysetApp->GetPlayset()))
	{
		PlaysetTabFactories.RegisterFactory(MakeShareable(new FPlaysetTabFactory_Viewport(InPlaysetApp)));
	}
	if (InPlaysetApp->ShouldSpawnDisplayInfoForPlayset(InPlaysetApp->GetPlayset()))
	{
		PlaysetTabFactories.RegisterFactory(MakeShareable(new FPlaysetTabFactory_DisplayInfo(InPlaysetApp)));
	}
	if (InPlaysetApp->ShouldSpawnPlacementForPlayset(InPlaysetApp->GetPlayset()))
	{
		PlaysetTabFactories.RegisterFactory(MakeShareable(new FPlaysetTabFactory_Placement(InPlaysetApp)));
	}
	
	TabLayout = FTabManager::NewLayout("Standalone_Playset_Layout_v03")
	->AddArea
	(
		FTabManager::NewPrimaryArea()
		->SetOrientation(Orient_Vertical)
		->Split
		(
			FTabManager::NewSplitter()
			->SetOrientation(Orient_Horizontal)
			->Split
			(
				FTabManager::NewSplitter()
				->SetOrientation(Orient_Vertical)
				->SetSizeCoefficient(0.33f)

				->SetSizeCoefficient(0.33f)

				// Viewport
				->Split
				(
					FTabManager::NewStack()
					->SetSizeCoefficient(0.36f)
					->AddTab(PlaysetEditorIDs::Tabs::PlaysetTab_Preview, ETabState::OpenedTab)
				)

				// Display Info
				->Split
				(
					FTabManager::NewStack()
					->SetSizeCoefficient(0.64f)
					->AddTab(PlaysetEditorIDs::Tabs::PlaysetTab_DisplayInfo, ETabState::OpenedTab)
				)
			)
			
			->SetSizeCoefficient(0.67f)

			// Details & Placement
			->Split
			(
				FTabManager::NewStack()
				->AddTab(PlaysetEditorIDs::Tabs::PlaysetTab_Details, ETabState::OpenedTab)
				->AddTab(PlaysetEditorIDs::Tabs::PlaysetTab_Placement, ETabState::OpenedTab)
				->SetForegroundTab(PlaysetEditorIDs::Tabs::PlaysetTab_Details)
			)
		)
	);

	InPlaysetApp->GetToolbarBuilder()->AddModesToolbar(ToolbarExtender);
}

FPlaysetEditorMode_DataList::FPlaysetEditorMode_DataList(TSharedPtr<class FPlaysetEditorApp> InPlaysetApp)
	: FPlaysetEditorModeBase(PlaysetEditorIDs::Mode::PlaysetMode_DataList, InPlaysetApp)
{
	PlaysetTabFactories.RegisterFactory(MakeShareable(new FPlaysetTabFactory_Details(InPlaysetApp)));
	PlaysetTabFactories.RegisterFactory(MakeShareable(new FPlaysetTabFactory_DataList(InPlaysetApp)));

	TabLayout = FTabManager::NewLayout("Standalone_PlaysetDataList_Layout_v01")
	->AddArea
	(
		FTabManager::NewPrimaryArea()
		->SetOrientation(Orient_Vertical)
		->Split
		(
			FTabManager::NewSplitter()
			->SetOrientation(Orient_Horizontal)

			// Data list
			->Split
			(
				FTabManager::NewStack()
				->SetSizeCoefficient(0.4f)
				->AddTab(PlaysetEditorIDs::Tabs::PlaysetTab_DataList, ETabState::OpenedTab)
			)

			// Details
			->Split
			(
				FTabManager::NewStack()
				->SetSizeCoefficient(0.6f)
				->AddTab(PlaysetEditorIDs::Tabs::PlaysetTab_Details, ETabState::OpenedTab)
			)
		)
	);

	InPlaysetApp->GetToolbarBuilder()->AddModesToolbar(ToolbarExtender);
}
