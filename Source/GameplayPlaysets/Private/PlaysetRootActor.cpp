// Copyright © 2024 MajorT. All rights reserved.


#include "PlaysetRootActor.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(PlaysetRootActor)

APlaysetRootActor::APlaysetRootActor(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = false;

	SceneComponent = ObjectInitializer.CreateDefaultSubobject<USceneComponent>(this, TEXT("SceneComponent"));
	SetRootComponent(SceneComponent);
}