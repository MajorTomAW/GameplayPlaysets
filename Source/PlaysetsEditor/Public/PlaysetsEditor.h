#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

class FWorkflowCentricApplication;

/** Namespace that stores global IDs for the Playsets editor */
namespace PlaysetEditorIDs
{
	static const FName PlaysetEditorApp = "PlaysetEditorApp";

	namespace Mode
	{
		static const FName PlaysetMode_Default = "PlaysetMode_Default";
	}

	namespace Tabs
	{
		static const FName PlaysetTab_Details = "Tab_PlaysetDetails";
		static const FName PlaysetTab_Preview = "Tab_PlaysetPreview";
		static const FName PlaysetTab_DisplayInfo = "Tab_PlaysetDisplayInfo";

		static const FName PlaysetTab_DataList = "Tab_PlaysetDataList";
	}
}

/** The public interface to this module */
class IPlaysetsEditorModule : public IModuleInterface
{
public:
    /**
     * Singleton-like access to this module's interface.  This is just for convenience!
     * Beware of calling this during the shutdown phase, though.  Your module might have been unloaded already.
     * 
     * @return Singleton instance, loading the module on demand if needed 
     */
    static IPlaysetsEditorModule& Get()
    {
        return FModuleManager::LoadModuleChecked<IPlaysetsEditorModule>("PlaysetsEditor");
    }

    /**
     * Checks to see if this module is loaded and ready.  It is only valid to call Get() if IsAvailable() returns true.
     * 
     * @return True if the module is loaded and ready to use
     */
    static bool IsAvailable()
    {
        return FModuleManager::Get().IsModuleLoaded("PlaysetsEditor");
    }

	/** Creates a new Playsets editor */
	virtual TSharedRef<FWorkflowCentricApplication> CreatePlaysetsEditor(const EToolkitMode::Type Mode, const TSharedPtr< IToolkitHost >& InitToolkitHost, class UPlayset* Playset) = 0;
};
