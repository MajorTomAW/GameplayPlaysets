// Copyright © 2024 MajorT. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "PlaysetApplicationModeBase.h"

class FPlaysetEditorMode_Default : public FPlaysetApplicationModeBase
{
public:
	FPlaysetEditorMode_Default(TWeakPtr<FPlaysetEditorApp> InPlaysetEditorApp);
};