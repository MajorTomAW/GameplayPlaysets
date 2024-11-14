// Copyright © 2024 MajorT. All rights reserved.


#include "PlaysetEditorApp.h"

#include "Playset.h"
#include "PlaysetsEditor.h"
#include "Modes/PlaysetEditorMode_Default.h"

#define LOCTEXT_NAMESPACE "PlaysetEditorApp"

FPlaysetEditorApp::FPlaysetEditorApp()
	: FWorkflowCentricApplication()
	, FEditorUndoClient()
{
	Playset = nullptr;
}

void FPlaysetEditorApp::InitPlaysetEditorApp(
	const EToolkitMode::Type Mode, const TSharedPtr<class IToolkitHost>& InitToolkitHost, UObject* Object)
{
	Playset = Cast<UPlayset>(Object);
	TSharedPtr<FPlaysetEditorApp> ThisPtr(SharedThis(this));

	// Get all objects that can be edited.
	TArray<UObject*> ObjectsToEdit;
	if (Playset)
	{
		ObjectsToEdit.Add(Playset);
	}

	// If we're already editing an object, no need to recreate a new editor from scratch but update the existing one.
	const TArray<UObject*>* EditedObjects = GetObjectsCurrentlyBeingEdited();
	if (EditedObjects == nullptr || EditedObjects->Num() == 0)
	{
		InitAssetEditor(Mode, InitToolkitHost, PlaysetEditorIDs::PlaysetEditorApp, FTabManager::FLayout::NullLayout, true, true, ObjectsToEdit);

		// Add the application modes
		AddApplicationMode(PlaysetEditorIDs::Mode::PlaysetMode_Default, MakeShared<FPlaysetEditorMode_Default>(ThisPtr));
	}
	else
	{
		for (UObject* ObjectToEdit : ObjectsToEdit)
		{
			AddEditingObject(ObjectToEdit);
		}
	}

	// Update the mode
	if (Playset)
	{
		SetCurrentMode(PlaysetEditorIDs::Mode::PlaysetMode_Default);
	}

	RegenerateMenusAndToolbars();
}

FName FPlaysetEditorApp::GetToolkitFName() const
{
	return FName("PlaysetEditorApp");
}

FText FPlaysetEditorApp::GetBaseToolkitName() const
{
	return LOCTEXT("ToolkitName", "Playset Editor App");
}

FText FPlaysetEditorApp::GetToolkitName() const
{
	if (Playset)
	{
		return GetLabelForObject(Playset);
	}

	return FText::FromString("Invalid Playset");
}

FText FPlaysetEditorApp::GetToolkitToolTipText() const
{
	return FWorkflowCentricApplication::GetToolkitToolTipText();
}

FString FPlaysetEditorApp::GetWorldCentricTabPrefix() const
{
	return LOCTEXT("WorldCentricTabPrefix", "PlaysetEditor").ToString();
}

FString FPlaysetEditorApp::GetDocumentationLink() const
{
	return FWorkflowCentricApplication::GetDocumentationLink();
}

FLinearColor FPlaysetEditorApp::GetWorldCentricTabColorScale() const
{
	return FLinearColor::Red;
}

void FPlaysetEditorApp::OnToolkitHostingFinished(const TSharedRef<IToolkit>& Toolkit)
{
	FWorkflowCentricApplication::OnToolkitHostingFinished(Toolkit);
}

void FPlaysetEditorApp::OnToolkitHostingStarted(const TSharedRef<IToolkit>& Toolkit)
{
	FWorkflowCentricApplication::OnToolkitHostingStarted(Toolkit);
}

void FPlaysetEditorApp::SetCurrentMode(FName NewMode)
{
	FWorkflowCentricApplication::SetCurrentMode(NewMode);
}

void FPlaysetEditorApp::SaveAsset_Execute()
{
	FWorkflowCentricApplication::SaveAsset_Execute();
}

#undef LOCTEXT_NAMESPACE