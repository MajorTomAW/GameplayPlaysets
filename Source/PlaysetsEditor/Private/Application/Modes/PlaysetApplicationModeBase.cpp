// Copyright © 2024 MajorT. All rights reserved.


#include "PlaysetApplicationModeBase.h"

#include "NiagaraEditorStyle.h"
#include "Playset.h"
#include "PlaysetsEditor.h"
#include "Application/PlaysetEditorApp.h"
#include "Application/Slate/SPlaysetDataList.h"
#include "Editor/PropertyEditor/Private/SDetailsView.h"
#include "DetailsViewArgs.h"
#include "IDetailsView.h"

#define LOCTEXT_NAMESPACE "PlaysetApplicationModeBase"


FPlaysetApplicationModeBase::FPlaysetApplicationModeBase(
	FName InModeName, TWeakPtr<class FPlaysetEditorApp> InPlaysetEditorApp)
		: FApplicationMode(InModeName)
		, AppPtr(InPlaysetEditorApp)
{
}

void FPlaysetApplicationModeBase::RegisterTabFactories(TSharedPtr<FTabManager> InTabManager)
{
	InTabManager->RegisterTabSpawner(PlaysetEditorIDs::Tabs::PlaysetTab_Preview, FOnSpawnTab::CreateSP(this, &ThisClass::SpawnTab_Preview))
		.SetDisplayName(LOCTEXT("PlaysetTab_Preview", "Preview"))
		.SetGroup(WorkspaceMenuCategory.ToSharedRef())
		.SetIcon(FSlateIcon(FNiagaraEditorStyle::Get().GetStyleSetName(), "Tab.Viewport"));

	InTabManager->RegisterTabSpawner(PlaysetEditorIDs::Tabs::PlaysetTab_DisplayInfo, FOnSpawnTab::CreateSP(this, &ThisClass::SpawnTab_DisplayInfo))
		.SetDisplayName(LOCTEXT("PlaysetTab_DisplayInfo", "Display"))
		.SetGroup(WorkspaceMenuCategory.ToSharedRef())
		.SetIcon(FSlateIcon(FAppStyle::GetAppStyleSetName(), "AssetEditor.SaveThumbnail"));

	InTabManager->RegisterTabSpawner(PlaysetEditorIDs::Tabs::PlaysetTab_Details, FOnSpawnTab::CreateSP(this, &ThisClass::SpawnTab_Details))
		.SetDisplayName(LOCTEXT("PlaysetTab_Details", "Details"))
		.SetGroup(WorkspaceMenuCategory.ToSharedRef())
		.SetIcon(FSlateIcon(FAppStyle::GetAppStyleSetName(), "Icons.Details"));

	InTabManager->RegisterTabSpawner(PlaysetEditorIDs::Tabs::PlaysetTab_DataList, FOnSpawnTab::CreateSP(this, &ThisClass::SpawnTab_DataList))
		.SetDisplayName(LOCTEXT("PlaysetTab_DataList", "Data List"))
		.SetGroup(WorkspaceMenuCategory.ToSharedRef())
		.SetIcon(FSlateIcon(FAppStyle::GetAppStyleSetName(), "LevelEditor.Tabs.Outliner"));
}

void FPlaysetApplicationModeBase::PostActivateMode()
{
	FApplicationMode::PostActivateMode();
}

TSharedRef<SDockTab> FPlaysetApplicationModeBase::SpawnTab_Preview(const FSpawnTabArgs& Args) const
{
	check(Args.GetTabId().TabType == PlaysetEditorIDs::Tabs::PlaysetTab_Preview);
	TSharedPtr<FPlaysetEditorApp> App = StaticCastSharedPtr<FPlaysetEditorApp>(AppPtr.Pin());

	TSharedRef<SDockTab> SpawnedTab = SNew(SDockTab)
		[
			SNew(STextBlock)
			.Text(LOCTEXT("DummyText", "This is a dummy tab for the Playset Preview"))
		];

	return SpawnedTab;
}

TSharedRef<SDockTab> FPlaysetApplicationModeBase::SpawnTab_DisplayInfo(const FSpawnTabArgs& Args) const
{
	check(Args.GetTabId().TabType == PlaysetEditorIDs::Tabs::PlaysetTab_DisplayInfo);
	TSharedPtr<FPlaysetEditorApp> App = StaticCastSharedPtr<FPlaysetEditorApp>(AppPtr.Pin());

	FPropertyEditorModule& PropertyEditorModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");

	FDetailsViewArgs DetailsViewArgs;
	DetailsViewArgs.bAllowSearch = 1;
	TSharedRef<IDetailsView> TestDetailsView = PropertyEditorModule.CreateDetailView(DetailsViewArgs);
	TestDetailsView->SetObject(App->Playset);

	TSharedRef<SDockTab> SpawnedTab = SNew(SDockTab)
		[
			TestDetailsView
		];

	return SpawnedTab;
}

TSharedRef<SDockTab> FPlaysetApplicationModeBase::SpawnTab_Details(const FSpawnTabArgs& Args)
{
	check(Args.GetTabId().TabType == PlaysetEditorIDs::Tabs::PlaysetTab_Details);
	TSharedPtr<FPlaysetEditorApp> App = StaticCastSharedPtr<FPlaysetEditorApp>(AppPtr.Pin());

	FPropertyEditorModule& PropertyEditorModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");
	FDetailsViewArgs DetailsViewArgs;
	DetailsViewArgs.NameAreaSettings = FDetailsViewArgs::HideNameArea;

	DetailsView = PropertyEditorModule.CreateDetailView(DetailsViewArgs);
	DetailsView->SetObject(nullptr);
	// DetailsView->OnFinishedChangingProperties()

	TSharedRef<SDockTab> SpawnedTab = SNew(SDockTab)
		[
			DetailsView.ToSharedRef()
		];

	return SpawnedTab;
}

TSharedRef<SDockTab> FPlaysetApplicationModeBase::SpawnTab_DataList(const FSpawnTabArgs& Args) const
{
	check(Args.GetTabId().TabType == PlaysetEditorIDs::Tabs::PlaysetTab_DataList);
	TSharedPtr<FPlaysetEditorApp> App = StaticCastSharedPtr<FPlaysetEditorApp>(AppPtr.Pin());
	
	TSharedRef<SDockTab> SpawnedTab = SNew(SDockTab)
		[
			SNew(SPlaysetDataList, App)
		];

	return SpawnedTab;
}

#undef LOCTEXT_NAMESPACE
