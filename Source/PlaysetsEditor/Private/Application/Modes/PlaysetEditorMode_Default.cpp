// Copyright © 2024 MajorT. All rights reserved.


#include "PlaysetEditorMode_Default.h"

#include "PlaysetsEditor.h"


FPlaysetEditorMode_Default::FPlaysetEditorMode_Default(TWeakPtr<FPlaysetEditorApp> InPlaysetEditorApp)
	: FPlaysetApplicationModeBase(PlaysetEditorIDs::Mode::PlaysetMode_Default, InPlaysetEditorApp)
{
	TabLayout = FTabManager::NewLayout("Standalone_Playset_Layout_v10.1")
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
				->SetSizeCoefficient(0.33)

				// Viewport
				->Split
				(
					FTabManager::NewStack()
					->SetSizeCoefficient(0.36f)
					->AddTab(PlaysetEditorIDs::Tabs::PlaysetTab_Preview, ETabState::OpenedTab)
				)

				// Display
				->Split
				(
					FTabManager::NewStack()
					->SetSizeCoefficient(0.64f)
					->AddTab(PlaysetEditorIDs::Tabs::PlaysetTab_DisplayInfo, ETabState::OpenedTab)
					->AddTab(PlaysetEditorIDs::Tabs::PlaysetTab_DataList, ETabState::OpenedTab)
					->SetForegroundTab(PlaysetEditorIDs::Tabs::PlaysetTab_DisplayInfo)
				)
			)

			->SetSizeCoefficient(0.67f)

			// Details
			->Split
			(
				FTabManager::NewStack()
				->AddTab(PlaysetEditorIDs::Tabs::PlaysetTab_Details, ETabState::OpenedTab)
			)
		)
	);
}
