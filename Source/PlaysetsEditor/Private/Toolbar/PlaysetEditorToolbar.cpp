// Copyright © 2024 MajorT. All rights reserved.


#include "PlaysetEditorToolbar.h"

#include "PlaysetsEditorModule.h"
#include "Application/PlaysetEditorApp.h"
#include "WorkflowOrientedApp/SModeWidget.h"

#define LOCTEXT_NAMESPACE "PlaysetEditorToolbar"

void FPlaysetEditorToolbar::AddModesToolbar(TSharedPtr<FExtender> Extender)
{
	TSharedPtr<FPlaysetEditorApp> App = GetApp_Checked();

	Extender->AddToolBarExtension
	(
		"Asset",
		EExtensionHook::After,
		App->GetToolkitCommands(),
		FToolBarExtensionDelegate::CreateSP(this, &FPlaysetEditorToolbar::FillModesToolbar)
	);
}

TSharedPtr<FPlaysetEditorApp> FPlaysetEditorToolbar::GetApp() const
{
	return AppPtr.Pin();
}

TSharedPtr<FPlaysetEditorApp> FPlaysetEditorToolbar::GetApp_Checked() const
{
	check(AppPtr.IsValid());
	return GetApp();
}

void FPlaysetEditorToolbar::FillModesToolbar(FToolBarBuilder& ToolBarBuilder)
{
	TSharedPtr<FPlaysetEditorApp> App = GetApp_Checked();

	ToolBarBuilder.BeginSection("Playset");
	{
		// Default Mode
		ToolBarBuilder.AddToolBarButton
		(
			FUIAction
			(
				FExecuteAction::CreateSP(this, &FPlaysetEditorToolbar::OnSetMode, PlaysetEditorIDs::Mode::PlaysetMode_Default),
				FCanExecuteAction::CreateSP(this, &FPlaysetEditorToolbar::CanSetMode, PlaysetEditorIDs::Mode::PlaysetMode_Default),
				FIsActionChecked::CreateSP(this, &FPlaysetEditorToolbar::IsModeActive, PlaysetEditorIDs::Mode::PlaysetMode_Default)
			),
			NAME_None,
			LOCTEXT("LabelOverride_Default", "Default"),
			LOCTEXT("TooltipOverride_Default", "Switch to the default mode"),
			FSlateIcon(FAppStyle::GetAppStyleSetName(), "Icons.Details"),
			EUserInterfaceActionType::ToggleButton
		);

		// Data List Mode
		ToolBarBuilder.AddToolBarButton
		(
			FUIAction
			(
				FExecuteAction::CreateSP(this, &FPlaysetEditorToolbar::OnSetMode, PlaysetEditorIDs::Mode::PlaysetMode_DataList),
				FCanExecuteAction::CreateSP(this, &FPlaysetEditorToolbar::CanSetMode, PlaysetEditorIDs::Mode::PlaysetMode_DataList),
				FIsActionChecked::CreateSP(this, &FPlaysetEditorToolbar::IsModeActive, PlaysetEditorIDs::Mode::PlaysetMode_DataList)
			),
			NAME_None,
			LOCTEXT("LabelOverride_DataList", "Data List"),
			LOCTEXT("TooltipOverride_DataList", "Switch to the data list mode"),
			FSlateIcon(FAppStyle::GetAppStyleSetName(), "LevelEditor.Tabs.Outliner"),
			EUserInterfaceActionType::ToggleButton
		);
	}
}

void FPlaysetEditorToolbar::OnSetMode(FName ModeName)
{
	GetApp()->SetCurrentMode(ModeName);
}

bool FPlaysetEditorToolbar::CanSetMode(FName ModeName) const
{
	return true;
}

bool FPlaysetEditorToolbar::IsModeActive(FName ModeName) const
{
	return GetApp()->GetCurrentMode() == ModeName;
}

#undef LOCTEXT_NAMESPACE
