// Copyright © 2024 MajorT. All rights reserved.


#include "ActorFactory_Playset.h"

#define LOCTEXT_NAMESPACE "ActorFactory_Playset"

#include "LevelEditorViewport.h"
#include "Playset.h"
#include "PlaysetDeveloperSettings.h"
#include "PlaysetRootActor.h"
#include "Subsystems/PlacementSubsystem.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(ActorFactory_Playset)

UActorFactory_Playset::UActorFactory_Playset(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	DisplayName = LOCTEXT("PlaysetDisplayName", "Playset");

	bShowInEditorQuickMenu = false;
	bUsePlacementExtent = true;
}

bool UActorFactory_Playset::CanCreateActorFrom(const FAssetData& AssetData, FText& OutErrorMsg)
{
	if (!AssetData.IsValid() || !AssetData.IsInstanceOf(UPlayset::StaticClass()))
	{
		OutErrorMsg = LOCTEXT("NoPlayset", "No Playset was specified, or the specified Playset is corrupted.");
		return false;
	}

	UPlayset* Playset = Cast<UPlayset>(AssetData.GetAsset());
	if (Playset == nullptr)
	{
		OutErrorMsg = LOCTEXT("NoPlayset", "No Playset was specified, or the specified Playset is corrupted.");
		return false;
	}

	const bool bIsSourceBased = !Playset->SourceActorBlueprint.IsNull();

	if (bIsSourceBased)
	{
		return true;
	}

	const bool bHasAnyData = !Playset->ActorData.IsEmpty();
	if (!bHasAnyData)
	{
		OutErrorMsg = LOCTEXT("NoData", "The Playset does not contain any data.");
		return false;
	}

	return true;
}

AActor* UActorFactory_Playset::GetDefaultActor(const FAssetData& AssetData)
{
	UClass* ActorClass = GetDefaultActorClass(AssetData);
	return ActorClass ? ActorClass->GetDefaultObject<AActor>() : nullptr;
}

UClass* UActorFactory_Playset::GetDefaultActorClass(const FAssetData& AssetData)
{
	if (!AssetData.IsValid() || !AssetData.IsInstanceOf(UPlayset::StaticClass()))
	{
		return nullptr;
	}

	UPlayset* Playset = Cast<UPlayset>(AssetData.GetAsset());
	if (Playset == nullptr)
	{
		return nullptr;
	}

	const bool bIsSourceBased = !Playset->SourceActorBlueprint.IsNull();
	if (bIsSourceBased)
	{
		return Playset->SourceActorBlueprint.LoadSynchronous();
	}

	if (const UPlaysetDeveloperSettings* Settings = UPlaysetDeveloperSettings::Get())
	{
		// Fail-safe to make sure the default root actor class is not null
		if (!Settings->DefaultRootActorClass.IsNull())
		{
			return Settings->DefaultRootActorClass.LoadSynchronous();	
		}
	}

	return APlaysetRootActor::StaticClass();
}

AActor* UActorFactory_Playset::SpawnActor(
	UObject* InAsset, ULevel* InLevel, const FTransform& InTransform, const FActorSpawnParameters& InSpawnParams)
{
	AActor* Spawned = Super::SpawnActor(InAsset, InLevel, InTransform, InSpawnParams);

	UPlayset* Playset = Cast<UPlayset>(InAsset);
	if (Playset == nullptr)
	{
		return Spawned;
	}

	FActorSpawnParameters SpawnParamsCopy = InSpawnParams;
	SpawnParamsCopy.OverrideLevel = InLevel;

	const bool bIsCreatingPreviewElements = FLevelEditorViewportClient::IsDroppingPreviewActor();
	bool bIsPlacementSystemCreatingPreviewElements = false;
	if (UPlacementSubsystem* PlacementSub = GEditor->GetEditorSubsystem<UPlacementSubsystem>())
	{
		bIsPlacementSystemCreatingPreviewElements = PlacementSub->IsCreatingPreviewElements();
	}

	SpawnParamsCopy.bTemporaryEditorActor = bIsCreatingPreviewElements || bIsPlacementSystemCreatingPreviewElements;
	SpawnParamsCopy.bHideFromSceneOutliner = bIsPlacementSystemCreatingPreviewElements;

	SpawnAdditionalActors(Spawned, Playset->ActorData, SpawnParamsCopy);

	return Spawned;
}

FString UActorFactory_Playset::GetDefaultActorLabel(UObject* Asset) const
{
	if (const UPlayset* Playset = Cast<UPlayset>(Asset))
	{
		return Playset->ItemName.ToString();
	}
	
	return Super::GetDefaultActorLabel(Asset);
}

bool UActorFactory_Playset::PreSpawnActor(UObject* Asset, FTransform& InOutLocation)
{
	return Super::PreSpawnActor(Asset, InOutLocation);
}

void UActorFactory_Playset::PostSpawnActor(UObject* Asset, AActor* NewActor)
{
}

void UActorFactory_Playset::SpawnAdditionalActors(AActor* RootActor, const TArray<FPlaysetActorData>& ActorData, const FActorSpawnParameters& InSpawnParams)
{
	check(RootActor);

	UWorld* World = RootActor->GetWorld();
	check(World);

	auto SpawnActor = [RootActor, InSpawnParams, World] (const FPlaysetActorData& InData)
	{
		UClass* Class = InData.ActorClass.LoadSynchronous();
		AActor* NewActor = World->SpawnActor<AActor>(Class, RootActor->GetActorTransform(), InSpawnParams);
		check(NewActor);

		// Disable collision for temporary actors
		if (InSpawnParams.bTemporaryEditorActor)
		{
			NewActor->SetActorEnableCollision(false);
		}

		NewActor->GetRootComponent()->SetMobility(EComponentMobility::Movable);
		NewActor->SetActorTransform(InData.RelativeTransform);

		NewActor->AttachToActor(RootActor, FAttachmentTransformRules::KeepRelativeTransform);
	};

	for (const FPlaysetActorData& Data : ActorData)
	{
		if (Data.ActorClass.IsNull())
		{
			continue;
		}

		SpawnActor(Data);
	}
}

#undef LOCTEXT_NAMESPACE
