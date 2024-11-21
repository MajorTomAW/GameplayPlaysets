// Copyright © 2024 MajorT. All rights reserved.

#pragma once

#include "Framework/Commands/Commands.h"

class FPlaysetDataListCommands : public  TCommands<FPlaysetDataListCommands>
{
public:
	FPlaysetDataListCommands();

	virtual void RegisterCommands() override;

public:
	TSharedPtr<FUICommandInfo> DeleteDataObject;
	TSharedPtr<FUICommandInfo> DuplicateDataObject;
};