// Copyright © 2024 MajorT. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PlaysetRootActor.generated.h"

UCLASS(meta = (DontUseGenericSpawnObject), NotBlueprintable, NotBlueprintType, NotPlaceable)
class GAMEPLAYPLAYSETS_API APlaysetRootActor : public AActor
{
	GENERATED_UCLASS_BODY()

protected:
	/** The default scene root component of this actor. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components")
	TObjectPtr<class USceneComponent> SceneComponent;

protected:
	//~ Begin AActor Interface
	virtual void Destroyed() override;
	//~ End AActor Interface
};
