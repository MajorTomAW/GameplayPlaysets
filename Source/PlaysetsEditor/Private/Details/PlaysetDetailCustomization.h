// Copyright © 2024 MajorT. All rights reserved.

#pragma once

#include "IDetailCustomization.h"
#include "Playset.h"

class UPlayset;

struct FPlaysetDetailsViewArgs
{
public:
	TArray<FName> CategoryBlacklist;
	TArray<FName> CategoryWhitelist;
	
	TArray<FName> PropertyBlacklist;
	TArray<FName> PropertyWhitelist;

	UClass* CustomClass = nullptr;

public:
	static FPlaysetDetailsViewArgs DisplayInfo()
	{
		FPlaysetDetailsViewArgs Args;
		Args.CategoryWhitelist.Add("Display");
		Args.CustomClass = UPlayset::StaticClass();
		Args.PropertyBlacklist.Add(GET_MEMBER_NAME_CHECKED(UPlayset, DataListObjects));
		return Args;
	}

	static FPlaysetDetailsViewArgs Placement()
	{
		FPlaysetDetailsViewArgs Args;
		Args.CategoryWhitelist.Add("Placement");
		Args.CustomClass = UPlayset::StaticClass();
		Args.PropertyBlacklist.Add(GET_MEMBER_NAME_CHECKED(UPlayset, DataListObjects));
		return Args;
	}

	static FPlaysetDetailsViewArgs Default()
	{
		FPlaysetDetailsViewArgs Args;
		Args.CategoryBlacklist.Append({"Display", "Placement"});
		Args.PropertyBlacklist.Add(GET_MEMBER_NAME_CHECKED(UPlayset, DataListObjects));
		Args.CustomClass = UPlayset::StaticClass();
		return Args;
	}
};

class FPlaysetDetailCustomization : public IDetailCustomization
{
public:
	FPlaysetDetailCustomization();
	static TSharedRef<IDetailCustomization> MakeInstance(FPlaysetDetailsViewArgs InArgs, TSharedPtr<class FPlaysetEditorApp> InPlaysetApp);

	virtual void CustomizeDetails(IDetailLayoutBuilder& DetailBuilder) override;

protected:
	FPlaysetDetailsViewArgs Args;
	TWeakPtr<FPlaysetEditorApp> AppPtr;
};
