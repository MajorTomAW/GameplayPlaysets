// Copyright © 2024 MajorT. All rights reserved.


#include "Tabs/PlaysetEditorTabFactory.h"

#include "NiagaraEditorStyle.h"
#include "PlaysetsEditor.h"
#include "Application/PlaysetEditorApp.h"

#define LOCTEXT_NAMESPACE "PlaysetEditorTabFactory"

FPlaysetEditorTabFactory::FPlaysetEditorTabFactory(FName InTabID, TSharedPtr<class FPlaysetEditorApp> InPlaysetApp)
	: FWorkflowTabFactory(InTabID, InPlaysetApp)
{
	AppPtr = InPlaysetApp;
	bIsSingleton = true;
}

TSharedRef<SWidget> FPlaysetEditorTabFactory::CreateTabBody(const FWorkflowTabSpawnInfo& Info) const
{
	return FWorkflowTabFactory::CreateTabBody(Info);
}

FText FPlaysetEditorTabFactory::GetTabToolTipText(const FWorkflowTabSpawnInfo& Info) const
{
	return FWorkflowTabFactory::GetTabToolTipText(Info);
}

FPlaysetTabFactory_Details::FPlaysetTabFactory_Details(TSharedPtr<class FPlaysetEditorApp> InPlaysetApp)
	: FPlaysetEditorTabFactory(PlaysetEditorIDs::Tabs::PlaysetTab_Details, InPlaysetApp)
{
	TabLabel = LOCTEXT("DetailsLabel", "Details");
	TabIcon = FSlateIcon(FAppStyle::GetAppStyleSetName(), "LevelEditor.Tabs.Details");
	ViewMenuDescription = LOCTEXT("DetailsView", "Details");
	ViewMenuTooltip = LOCTEXT("DetailsView_ToolTip", "Show the details view");
}

TSharedRef<SWidget> FPlaysetTabFactory_Details::CreateTabBody(const FWorkflowTabSpawnInfo& Info) const
{
	check(AppPtr.IsValid());
	return AppPtr.Pin()->SpawnTab_Details();
}

FText FPlaysetTabFactory_Details::GetTabToolTipText(const FWorkflowTabSpawnInfo& Info) const
{
	return LOCTEXT("DetailsToolTip", "The details tab is for editing playset properties.");
}



FPlaysetTabFactory_Viewport::FPlaysetTabFactory_Viewport(TSharedPtr<FPlaysetEditorApp> InPlaysetApp)
	: FPlaysetEditorTabFactory(PlaysetEditorIDs::Tabs::PlaysetTab_Preview, InPlaysetApp)
{
	TabLabel = LOCTEXT("ViewportLabel", "Preview");
	TabIcon = FSlateIcon(FNiagaraEditorStyle::Get().GetStyleSetName(), "Tab.Viewport");
	ViewMenuDescription = LOCTEXT("ViewportView", "Preview");
	ViewMenuTooltip = LOCTEXT("ViewportView_ToolTip", "Show the playset preview tab");
}

TSharedRef<SWidget> FPlaysetTabFactory_Viewport::CreateTabBody(const FWorkflowTabSpawnInfo& Info) const
{
	check(AppPtr.IsValid());
	return AppPtr.Pin()->SpawnTab_Viewport();
}

FText FPlaysetTabFactory_Viewport::GetTabToolTipText(const FWorkflowTabSpawnInfo& Info) const
{
	return LOCTEXT("ViewportToolTip", "The viewport tab is for previewing the playset.");
}


FPlaysetTabFactory_DisplayInfo::FPlaysetTabFactory_DisplayInfo(TSharedPtr<FPlaysetEditorApp> InPlaysetApp)
	: FPlaysetEditorTabFactory(PlaysetEditorIDs::Tabs::PlaysetTab_DisplayInfo, InPlaysetApp)
{
	TabLabel = LOCTEXT("DisplayInfoLabel", "Display");
	TabIcon = FSlateIcon(FAppStyle::GetAppStyleSetName(), "AssetEditor.SaveThumbnail");
	ViewMenuDescription = LOCTEXT("DisplayInfoView", "Display Info");
	ViewMenuTooltip = LOCTEXT("DisplayInfoView_ToolTip", "Show the playset display info tab");
}

TSharedRef<SWidget> FPlaysetTabFactory_DisplayInfo::CreateTabBody(const FWorkflowTabSpawnInfo& Info) const
{
	check(AppPtr.IsValid());
	return AppPtr.Pin()->SpawnTab_DisplayInfo();
}

FText FPlaysetTabFactory_DisplayInfo::GetTabToolTipText(const FWorkflowTabSpawnInfo& Info) const
{
	return LOCTEXT("DisplayInfoToolTip", "The display info tab is for editing playset display properties.");
}

FPlaysetTabFactory_Placement::FPlaysetTabFactory_Placement(TSharedPtr<FPlaysetEditorApp> InPlaysetApp)
 	: FPlaysetEditorTabFactory(PlaysetEditorIDs::Tabs::PlaysetTab_Placement, InPlaysetApp)
{
	TabLabel = LOCTEXT("PlacementLabel", "Placement");
	TabIcon = FSlateIcon(FAppStyle::GetAppStyleSetName(), "LevelEditor.Tabs.PlacementBrowser");
	ViewMenuDescription = LOCTEXT("PlacementView", "Placement");
	ViewMenuTooltip = LOCTEXT("PlacementView_ToolTip", "Show the playset placement tab");
}

TSharedRef<SWidget> FPlaysetTabFactory_Placement::CreateTabBody(const FWorkflowTabSpawnInfo& Info) const
{
	check(AppPtr.IsValid());
	return AppPtr.Pin()->SpawnTab_Placement();
}

FText FPlaysetTabFactory_Placement::GetTabToolTipText(const FWorkflowTabSpawnInfo& Info) const
{
	return LOCTEXT("PlacementToolTip", "The placement tab is for editing playset placement properties.");
}

FPlaysetTabFactory_DataList::FPlaysetTabFactory_DataList(TSharedPtr<FPlaysetEditorApp> InPlaysetApp)
	: FPlaysetEditorTabFactory(PlaysetEditorIDs::Tabs::PlaysetTab_DataList, InPlaysetApp)
{
	TabLabel = LOCTEXT("DataListLabel", "Data List");
	TabIcon = FSlateIcon(FAppStyle::GetAppStyleSetName(), "LevelEditor.Tabs.Outliner");
	ViewMenuDescription = LOCTEXT("DataListView", "Data List");
	ViewMenuTooltip = LOCTEXT("DataListView_ToolTip", "Show the playset data list tab");
}

TSharedRef<SWidget> FPlaysetTabFactory_DataList::CreateTabBody(const FWorkflowTabSpawnInfo& Info) const
{
	check(AppPtr.IsValid());
	return AppPtr.Pin()->SpawnTab_DataList();
}

FText FPlaysetTabFactory_DataList::GetTabToolTipText(const FWorkflowTabSpawnInfo& Info) const
{
	return LOCTEXT("DataListToolTip", "The data list tab is for editing playset data list properties.");
}

#undef LOCTEXT_NAMESPACE
