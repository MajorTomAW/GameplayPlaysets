// Copyright © 2024 MajorT. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "ActorFactories/ActorFactory.h"
#include "ActorFactory_Playset.generated.h"

struct FPlaysetActorData;
class AActor;
struct FAssetData;

UCLASS(Config = Editor, CollapseCategories, HideCategories = Object, MinimalAPI)
class UActorFactory_Playset : public UActorFactory
{
	GENERATED_UCLASS_BODY()

	//~ Begin UActorFactory Interface
	PLAYSETSEDITOR_API virtual bool PreSpawnActor(UObject* Asset, FTransform& InOutLocation) override;
	PLAYSETSEDITOR_API virtual void PostSpawnActor(UObject* Asset, AActor* NewActor) override;

	PLAYSETSEDITOR_API virtual FString GetDefaultActorLabel(UObject* Asset) const override;
	
	PLAYSETSEDITOR_API virtual bool CanCreateActorFrom(const FAssetData& AssetData, FText& OutErrorMsg) override;
	PLAYSETSEDITOR_API virtual AActor* GetDefaultActor(const FAssetData& AssetData) override;
	PLAYSETSEDITOR_API virtual UClass* GetDefaultActorClass(const FAssetData& AssetData) override;
	PLAYSETSEDITOR_API virtual AActor* SpawnActor(UObject* InAsset, ULevel* InLevel, const FTransform& InTransform, const FActorSpawnParameters& InSpawnParams) override;
	//~ End UActorFactory Interface

protected:
	PLAYSETSEDITOR_API virtual void SpawnAdditionalActors(AActor* RootActor, const TArray<FPlaysetActorData>& ActorData, const FActorSpawnParameters& InSpawnParams);
};
