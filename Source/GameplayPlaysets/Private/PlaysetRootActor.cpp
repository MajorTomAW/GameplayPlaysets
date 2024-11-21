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

void APlaysetRootActor::Destroyed()
{
#if WITH_EDITOR
	if (GEngine->IsEditor())
	{
		bool bDestroy = true;
		GConfig->GetBool(TEXT("/Script/PlaysetsEditor.PlaysetDeveloperSettings"), TEXT("bDeleteAttachedActors"), bDestroy, GEditorIni);

		if (bDestroy)
		{
			TArray<AActor*> Attached;
			GetAttachedActors(Attached);

			for (AActor* Actor : Attached)
			{
				Actor->Destroy();
			}
		}
	}
#endif
	
	Super::Destroyed();
}
