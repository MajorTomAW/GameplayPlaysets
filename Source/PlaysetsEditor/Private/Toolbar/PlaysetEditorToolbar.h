// Copyright © 2024 MajorT. All rights reserved.

#pragma once

class FPlaysetEditorApp;

class FPlaysetEditorToolbar : public TSharedFromThis<FPlaysetEditorToolbar>
{
public:
	virtual ~FPlaysetEditorToolbar() = default;
	FPlaysetEditorToolbar(TSharedPtr<FPlaysetEditorApp> InPlaysetApp)
		: AppPtr(InPlaysetApp)
	{
	}

	void AddModesToolbar(TSharedPtr<FExtender> Extender);

protected:
	TWeakPtr<FPlaysetEditorApp> AppPtr;

protected:
	TSharedPtr<FPlaysetEditorApp> GetApp() const;
	TSharedPtr<FPlaysetEditorApp> GetApp_Checked() const;
	
	virtual void FillModesToolbar(FToolBarBuilder& ToolBarBuilder);

private:
	void OnSetMode(FName ModeName);
	bool CanSetMode(FName ModeName) const;
	bool IsModeActive(FName ModeName) const;
};

