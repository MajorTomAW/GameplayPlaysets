#pragma once

#include "CoreMinimal.h"
#include "Playset.h"
#include "Modules/ModuleManager.h"

class FWorkflowCentricApplication;

/** Namespace that stores global IDs for the Playsets editor */
namespace PlaysetEditorIDs
{
	static const FName PlaysetEditorApp = "PlaysetEditorApp";

	namespace Mode
	{
		static const FName PlaysetMode_Default = "PlaysetMode_Default";
		static const FName PlaysetMode_DataList = "PlaysetMode_DataList";
	}

	namespace Tabs
	{
		static const FName PlaysetTab_Details = "Tab_PlaysetDetails";
		static const FName PlaysetTab_Preview = "Tab_PlaysetPreview";
		static const FName PlaysetTab_DisplayInfo = "Tab_PlaysetDisplayInfo";
		static const FName PlaysetTab_Placement = "Tab_PlaysetPlacement";
		static const FName PlaysetTab_DataList = "Tab_PlaysetDataList";
	}
}

namespace UE::Playset::Editor
{
	struct FPlaysetInitArgs
	{
	public:
		FPlaysetInitArgs(UObject* ConverterObj, TArray<FAssetData> InSelectedActors);
		FPlaysetInitArgs(FText InName, FText InShortDesc, FText InLongDesc, UTexture2D* InSmallThumbnail, UTexture2D* InLargeThumbnail, TArray<FAssetData> InSelectedActors, bool InIsSourceActor, EPlaysetOffsetType InOffsetType);

	public:
		FText GetName() const
		{
			return Name;
		}

		FText GetShortDescription() const
        {
            return ShortDescription;
        }

		FText GetLongDescription() const
        {
            return LongDescription;
        }

		bool IsSourceActor() const
        {
            return bIsSourceActor;
        }

		UTexture2D* GetSmallThumbnail() const
        {
            return Cast<UTexture2D>(SmallThumbnail.GetAsset());
        }

		UTexture2D* GetLargeThumbnail() const
        {
            return Cast<UTexture2D>(LargeThumbnail.GetAsset());
        }

		const TArray<FAssetData>& GetSelectedActors() const
        {
            return SelectedActors;
        }

		EPlaysetOffsetType GetOffsetType() const
        {
            return OffsetType;
        }
		
	protected:
		FText Name;
		FText ShortDescription;
		FText LongDescription;
		bool bIsSourceActor = false;
		FAssetData SmallThumbnail;
		FAssetData LargeThumbnail;
		TArray<FAssetData> SelectedActors;
		EPlaysetOffsetType OffsetType;
	};
	
	void InitializePlayset(UPlayset* Playset, const FPlaysetInitArgs& Args);
	void InitializePlaysetActiveAssets(UPlayset* Playset, const FPlaysetInitArgs& Args);
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
	virtual TSharedRef<FWorkflowCentricApplication> CreatePlaysetsEditor(const EToolkitMode::Type Mode, const TSharedPtr< IToolkitHost >& InitToolkitHost, class UObject* InObject) = 0;
};
