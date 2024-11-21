// Copyright © 2024 MajorT. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "PlaysetDeveloperSettings.generated.h"

class APlaysetRootActor;
/**
 * Developer settings for the Gameplay Playsets plugin.
 */
UCLASS(meta = (DisplayName = "Playsets Settings"), Config = Editor)
class UPlaysetDeveloperSettings : public UDeveloperSettings
{
	GENERATED_UCLASS_BODY()

public:
	/** Returns the developer settings for the Gameplay Playsets plugin. */
	static UPlaysetDeveloperSettings* Get();
	
	//~ Begin UDeveloperSettings Interface
	virtual FName GetCategoryName() const override;
	virtual FName GetContainerName() const override;
	virtual FName GetSectionName() const override;

#if WITH_EDITOR
	virtual FText GetSectionText() const override;
#endif
	//~ End UDeveloperSettings Interface

	/** Saves the current configuration to the config file. */
	virtual void SavePlaysetConfig() const;

#if WITH_EDITOR
	virtual void PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

public:
#pragma region general

	/** The default root actor class to use when spawning a playset. */
	UPROPERTY(Config, EditDefaultsOnly, Category = "General", meta = (DisplayName = "Default Root Actor Class"))
	TSoftClassPtr<APlaysetRootActor> DefaultRootActorClass;
	
#pragma endregion
	
#pragma region creating_playsets
	
	/** The default directory to save playsets to. */
	UPROPERTY(Config, EditDefaultsOnly, Category = "Creating Playsets", meta = (DisplayName = "Default Save Directory", RelativeToGameContentDir, LongPackageName))
	FDirectoryPath DefaultSaveDirectory;

	/** Whether to deselect all native actors when converting to a playset. */
	UPROPERTY(Config, EditDefaultsOnly, Category = "Creating Playsets", AdvancedDisplay)
	uint32 bDeselectNativeActors : 1;

#pragma endregion

#pragma region level_editor

	/** Whether to delete all the attached actors when deleting the playset root actor. */
	UPROPERTY(Config, EditDefaultsOnly, Category = "Level Editor")
	uint32 bDeleteAttachedActors : 1;

	/** Whether to delete the original actors when converting to a playset. */
	UPROPERTY(Config, VisibleDefaultsOnly, Category = "Level Editor")
	uint32 bDeleteOriginalActors : 1;
#pragma endregion
};
