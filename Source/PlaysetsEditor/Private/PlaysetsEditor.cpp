#include "PlaysetsEditor.h"

#include "Playset.h"
#include "PlaysetThumbnailRenderer.h"
#include "Application/PlaysetEditorApp.h"
#include "Style/PlaysetStyle.h"
#include "ThumbnailRendering/ThumbnailManager.h"

#define LOCTEXT_NAMESPACE "FPlaysetsEditorModule"

class FPlaysetsEditorModule final : public IPlaysetsEditorModule
{
public:
	using ThisClass = FPlaysetsEditorModule;
	
	//~ Begin IModuleInterface interface
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
	//~ End IModuleInterface interface

	virtual TSharedRef<FWorkflowCentricApplication> CreatePlaysetsEditor(const EToolkitMode::Type Mode, const TSharedPtr<IToolkitHost>& InitToolkitHost, UPlayset* Playset) override;

protected:
	void OnPostEngineInit();
};
IMPLEMENT_MODULE(FPlaysetsEditorModule, PlaysetsEditor);

void FPlaysetsEditorModule::StartupModule()
{
	FCoreDelegates::OnPostEngineInit.AddRaw(this, &ThisClass::OnPostEngineInit);
}

void FPlaysetsEditorModule::ShutdownModule()
{
	IPlaysetsEditorModule::ShutdownModule();

	// Unregister style set
	FPlaysetStyle::Shutdown();
}

TSharedRef<FWorkflowCentricApplication> FPlaysetsEditorModule::CreatePlaysetsEditor(
	const EToolkitMode::Type Mode, const TSharedPtr<IToolkitHost>& InitToolkitHost, UPlayset* Playset)
{
	TSharedRef<FPlaysetEditorApp> App(new FPlaysetEditorApp);
	App->InitPlaysetEditorApp(Mode, InitToolkitHost, Playset);
	return App;
}

void FPlaysetsEditorModule::OnPostEngineInit()
{
	FPlaysetStyle::Get();

	if (GIsEditor)
	{
		UThumbnailManager::Get().RegisterCustomRenderer(UPlayset::StaticClass(), UPlaysetThumbnailRenderer::StaticClass());
	}
}

#undef LOCTEXT_NAMESPACE