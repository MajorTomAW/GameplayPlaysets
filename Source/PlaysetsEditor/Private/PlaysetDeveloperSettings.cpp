// Copyright © 2024 MajorT. All rights reserved.


#include "PlaysetDeveloperSettings.h"

#include "PlaysetRootActor.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(PlaysetDeveloperSettings)

UPlaysetDeveloperSettings::UPlaysetDeveloperSettings(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	DefaultRootActorClass = APlaysetRootActor::StaticClass();
	
	DefaultSaveDirectory.Path = TEXT("/Game");
	bDeselectNativeActors = true;

	bDeleteAttachedActors = true;
	bDeleteOriginalActors = true;
}

UPlaysetDeveloperSettings* UPlaysetDeveloperSettings::Get()
{
	UPlaysetDeveloperSettings* Settings = GetMutableDefault<UPlaysetDeveloperSettings>();
	return Settings;
}

FName UPlaysetDeveloperSettings::GetCategoryName() const
{
	return FApp::GetProjectName();
}

FName UPlaysetDeveloperSettings::GetContainerName() const
{
	return "Editor";
}

FName UPlaysetDeveloperSettings::GetSectionName() const
{
	return "Playsets";
}

#if WITH_EDITOR
FText UPlaysetDeveloperSettings::GetSectionText() const
{
	return FText::FromString(TEXT("Playsets"));
}

void UPlaysetDeveloperSettings::PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	if (PropertyChangedEvent.Property->GetName() == GET_MEMBER_NAME_CHECKED(UPlaysetDeveloperSettings, DefaultRootActorClass))
	{
		// Null is not allowed
		if (DefaultRootActorClass.IsNull())
		{
			DefaultRootActorClass = APlaysetRootActor::StaticClass();
		}
	}
}
#endif

void UPlaysetDeveloperSettings::SavePlaysetConfig() const
{
	UPlaysetDeveloperSettings* ThisPtr = Get();

	FString DefaultEditorIni = FPaths::SourceConfigDir() / TEXT("DefaultEditor.ini");
	DefaultEditorIni = FConfigCacheIni::NormalizeConfigIniPath(DefaultEditorIni);
	ThisPtr->SaveConfig(CPF_Config, *DefaultEditorIni);
}
