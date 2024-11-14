// Copyright © 2024 MajorT. All rights reserved.

#pragma once

#include "Styling/SlateStyle.h"

class FPlaysetStyle final : public FSlateStyleSet
{
	friend class FPlaysetsEditorModule;

protected:
	/** Initialize the Playset style set. (Creating it if it doesn't exist) */
	void Initialize();

	/** Shutdown the Playset style set. (Destroying it if it exists) */
	static void Shutdown();

public:
	FPlaysetStyle();
	virtual ~FPlaysetStyle();
	
	/** Returns the slate style set for the Playset editor */
	static FPlaysetStyle& Get();

private:
	static TUniquePtr<FPlaysetStyle> StyleSet;
};

