// Copyright © 2024 MajorT. All rights reserved.


#include "PlaysetEditorApp.h"

#include "Playset.h"
#include "PlaysetsEditorModule.h"
#include "Details/PlaysetDetailCustomization.h"
#include "Modes/PlaysetEditorModes.h"
#include "Slate/SPlaysetDataList.h"
#include "Slate/SPlaysetEditorViewport.h"
#include "Toolbar/PlaysetEditorToolbar.h"
#include "WorkflowOrientedApp/WorkflowCentricApplication.h"

#define LOCTEXT_NAMESPACE "PlaysetEditorApp"

FPlaysetEditorApp::FPlaysetEditorApp()
{
	Playset = nullptr;
	DataListCommandList = MakeShareable(new FUICommandList());
}

void FPlaysetEditorApp::CreatePlaysetEditorApp(
	const EToolkitMode::Type Mode, const TSharedPtr<IToolkitHost>& InitToolkitHost, UObject* InObject)
{
	UPlayset* PlaysetToEdit = Cast<UPlayset>(InObject);
	if (PlaysetToEdit != nullptr)
	{
		Playset = PlaysetToEdit;
	}

	TSharedPtr<FPlaysetEditorApp> ThisPtr(SharedThis(this));
	TArray<UObject*> ObjectsToEdit;
	if (Playset)
	{
		ObjectsToEdit.Add(Playset);
	}

	if (!ToolbarBuilder.IsValid())
	{
		ToolbarBuilder = MakeShareable(new FPlaysetEditorToolbar(ThisPtr));
	}

	const TArray<UObject*>* EditedObjects = GetObjectsCurrentlyBeingEdited();
	if (EditedObjects == nullptr || EditedObjects->Num() == 0)
	{
		constexpr bool bCreateDefaultStandaloneMenu = true;
		constexpr bool bCreateDefaultToolbar = true;
		InitAssetEditor(Mode, InitToolkitHost, PlaysetEditorIDs::PlaysetEditorApp, FTabManager::FLayout::NullLayout, bCreateDefaultStandaloneMenu, bCreateDefaultToolbar, ObjectsToEdit);

		CreateDetailsView();
		
		AddApplicationMode(PlaysetEditorIDs::Mode::PlaysetMode_Default, MakeShared<FPlaysetEditorMode_Default>(ThisPtr));
		AddApplicationMode(PlaysetEditorIDs::Mode::PlaysetMode_DataList, MakeShared<FPlaysetEditorMode_DataList>(ThisPtr));
	}
	else
	{
		for (UObject* ObjectToEdit : ObjectsToEdit)
		{
			if (!EditedObjects->Contains(ObjectToEdit))
			{
				AddEditingObject(ObjectToEdit);
			}
		}
	}

	if (Playset != nullptr)
	{
		SetCurrentMode(PlaysetEditorIDs::Mode::PlaysetMode_Default);
	}

	//OnClassListUpdated();
	RegenerateMenusAndToolbars();
}

FText FPlaysetEditorApp::GetLocalizedMode(FName InMode)
{
	return LOCTEXT("LocalizedMode", "Playset Editor");
}

FName FPlaysetEditorApp::GetToolkitFName() const
{
	return "Playset Editor";
}

FText FPlaysetEditorApp::GetBaseToolkitName() const
{
	return LOCTEXT("AppLabel", "Playset Editor");
}

FText FPlaysetEditorApp::GetToolkitName() const
{
	if (Playset != nullptr)
	{
		return GetLabelForObject(Playset);
	}

	return FText();
}

FText FPlaysetEditorApp::GetToolkitToolTipText() const
{
	if (Playset != nullptr)
	{
		return GetToolTipTextForObject(Playset);
	}

	return FText();
}

FString FPlaysetEditorApp::GetWorldCentricTabPrefix() const
{
	return LOCTEXT("WorldCentricTabPrefix", "Playset ").ToString();
}

FLinearColor FPlaysetEditorApp::GetWorldCentricTabColorScale() const
{
	return FLinearColor(0.f, 0.f, 0.2f, 0.5f);
}

bool FPlaysetEditorApp::IncludeAssetInRestoreOpenAssetsPrompt(UObject* Asset) const
{
	return Asset != nullptr;
}

void FPlaysetEditorApp::RegisterTabSpawners(const TSharedRef<FTabManager>& InTabManager)
{
	FWorkflowCentricApplication::RegisterTabSpawners(InTabManager);
}

void FPlaysetEditorApp::SetCurrentMode(FName NewMode)
{
	FWorkflowCentricApplication::SetCurrentMode(NewMode);

	if (NewMode == PlaysetEditorIDs::Mode::PlaysetMode_Default)
	{
		if (PrimaryDetailsView.IsValid())
		{
			PrimaryDetailsView->SetObject(Playset);
		}

		if (SecondaryDetailsView.IsValid())
		{
			SecondaryDetailsView->SetObject(Playset);
		}

		if (DisplayDetailsView.IsValid())
		{
			DisplayDetailsView->SetObject(Playset);
		}
	}
	else if (NewMode == PlaysetEditorIDs::Mode::PlaysetMode_DataList)
	{
		if (PrimaryDetailsView.IsValid())
		{
			PrimaryDetailsView->SetObject(nullptr);
		}
	}
}

TSharedRef<SWidget> FPlaysetEditorApp::SpawnTab_Details()
{
	PrimaryDetailsView->SetObject(Playset);
	
	return SNew(SVerticalBox)
		+ SVerticalBox::Slot()
		.FillHeight(1.f)
		.HAlign(HAlign_Fill)
		[
			PrimaryDetailsView.ToSharedRef()
		];
}

TSharedRef<SWidget> FPlaysetEditorApp::SpawnTab_Viewport()
{
	if (!Viewport.IsValid())
	{
		Viewport = SNew(SPlaysetEditorViewport, SharedThis(this));
	}
	
	TSharedRef<SVerticalBox> SpawnedWidget = SNew(SVerticalBox)
		+ SVerticalBox::Slot()
		.FillHeight(1.f)
		.HAlign(HAlign_Fill)
		[
			Viewport.ToSharedRef()
		];

	Viewport->UpdateViewport(Playset);

	// Viewport->OnAddedToTab(SpawnedWidget);

	return SpawnedWidget;
}

TSharedRef<SWidget> FPlaysetEditorApp::SpawnTab_DisplayInfo()
{
	DisplayDetailsView->SetObject(Playset);
	
	return SNew(SVerticalBox)
		+ SVerticalBox::Slot()
		.FillHeight(1.f)
		.HAlign(HAlign_Fill)
		[
			DisplayDetailsView.ToSharedRef()
		];
}

TSharedRef<SWidget> FPlaysetEditorApp::SpawnTab_Placement()
{
	SecondaryDetailsView->SetObject(Playset);
	
	return SNew(SVerticalBox)
		+ SVerticalBox::Slot()
		.FillHeight(1.f)
		.HAlign(HAlign_Fill)
		[
			SecondaryDetailsView.ToSharedRef()
		];
}

TSharedRef<SWidget> FPlaysetEditorApp::SpawnTab_DataList()
{
	return SNew(SVerticalBox)
		+ SVerticalBox::Slot()
		.FillHeight(1.f)
		.HAlign(HAlign_Fill)
		[
			SNew(SPlaysetDataList, SharedThis(this), DataListCommandList.ToSharedRef())
			.ItemHeight(38.f)
		];
}

void FPlaysetEditorApp::CreateDetailsView()
{
	FPropertyEditorModule& PropertyModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");
	TSharedPtr<FPlaysetEditorApp> ThisPtr(SharedThis(this));

	FDetailsViewArgs ViewArgs;
	{
		ViewArgs.NameAreaSettings = FDetailsViewArgs::HideNameArea;
		ViewArgs.NotifyHook = this;
		ViewArgs.DefaultsOnlyVisibility = EEditDefaultsOnlyNodeVisibility::Show;
		ViewArgs.bAllowFavoriteSystem = false;
		ViewArgs.bShowPropertyMatrixButton = false;
	}
	
	if (!PrimaryDetailsView.IsValid())
	{ // Primary Details View
		
		PrimaryDetailsView = PropertyModule.CreateDetailView(ViewArgs);
	
		PrimaryDetailsView->RegisterInstancedCustomPropertyLayout(UPlayset::StaticClass(), GetPrimaryDetailCustomization());
		PrimaryDetailsView->SetObject(nullptr);
		PrimaryDetailsView->OnFinishedChangingProperties().AddSP(this, &FPlaysetEditorApp::OnFinishedChangingProperties);
	}

	if (!SecondaryDetailsView.IsValid())
	{ // Secondary Details View (Placement by default)
		
		SecondaryDetailsView = PropertyModule.CreateDetailView(ViewArgs);
		
		SecondaryDetailsView->RegisterInstancedCustomPropertyLayout(UPlayset::StaticClass(), GetSecondaryDetailCustomization());
		SecondaryDetailsView->SetObject(nullptr);
		SecondaryDetailsView->OnFinishedChangingProperties().AddSP(this, &FPlaysetEditorApp::OnFinishedChangingProperties);
	}

	if (!DisplayDetailsView.IsValid())
	{ // Display Details VIew
		
		DisplayDetailsView = PropertyModule.CreateDetailView(ViewArgs);
		
		DisplayDetailsView->RegisterInstancedCustomPropertyLayout(UPlayset::StaticClass(), GetDisplayDetailCustomization());
		DisplayDetailsView->SetObject(nullptr);
		DisplayDetailsView->OnFinishedChangingProperties().AddSP(this, &FPlaysetEditorApp::OnFinishedChangingProperties);
	}
}

void FPlaysetEditorApp::OnFinishedChangingProperties(const FPropertyChangedEvent& PropertyChangedEvent)
{
}

void FPlaysetEditorApp::NotifyPostChange(const FPropertyChangedEvent& PropertyChangedEvent, FProperty* PropertyThatChanged)
{
	FNotifyHook::NotifyPostChange(PropertyChangedEvent, PropertyThatChanged);
}

void FPlaysetEditorApp::PostUndo(bool bSuccess)
{
	FEditorUndoClient::PostUndo(bSuccess);
}

void FPlaysetEditorApp::PostRedo(bool bSuccess)
{
	FEditorUndoClient::PostRedo(bSuccess);
}

#undef LOCTEXT_NAMESPACE
