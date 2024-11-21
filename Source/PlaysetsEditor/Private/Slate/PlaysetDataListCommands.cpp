// Copyright © 2024 MajorT. All rights reserved.


#include "PlaysetDataListCommands.h"

#define LOCTEXT_NAMESPACE "PlaysetDataListCommands"

FPlaysetDataListCommands::FPlaysetDataListCommands()
	: TCommands(
		TEXT("PlaysetEditor"),
		LOCTEXT("PlaysetEditor", "Playset Editor"),
		NAME_None,
		TEXT("PlaysetStyle")
		)
{
}

void FPlaysetDataListCommands::RegisterCommands()
{
	UI_COMMAND(DeleteDataObject, "Delete", "Delete Selected Data Objects", EUserInterfaceActionType::Button, FInputChord(EKeys::Delete));
	UI_COMMAND(DuplicateDataObject, "Duplicate", "Duplicate Selected Data Objects", EUserInterfaceActionType::Button, FInputChord(EKeys::D, EModifierKey::Control));
}

#undef LOCTEXT_NAMESPACE