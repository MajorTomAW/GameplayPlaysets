// Copyright © 2024 MajorT. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"

#include "PlaysetTypes.generated.h"

/**
 * EPlaysetOffsetType
 *
 * The type of offset to use when spawning the playset.
 */
UENUM(BlueprintType)
enum class EPlaysetOffsetType : uint8
{
	/** Spawn the playset at the center of the bounding box. */
	Center UMETA(DisplayName = "Center"),

	/** Spawn the playset in the upper right corner of the bounding box. */
	CustomOffsetFromCorner UMETA(DisplayName = "Custom Offset from Corner"),
};

USTRUCT(BlueprintType)
struct FPlaysetActorData
{
	GENERATED_BODY()

public:
	FPlaysetActorData();

public:
	/** The actor blueprint that will be used to spawn the playset. If not set, the playset will be spawned as is. */
	UPROPERTY(EditAnywhere, Category = "ActorData")
	TSoftClassPtr<AActor> ActorClass;

	/** The relative transform of the actor. */
	UPROPERTY(EditAnywhere, Category = "ActorData")
	FTransform RelativeTransform = FTransform::Identity;

	/** The influence distance of the actor. (Haven't figured out what this is for yet) */
	UPROPERTY(EditAnywhere, Category = "ActorData")
	float InfluenceDistance = 0.f;

	/** The actor template id. */
	UPROPERTY(EditAnywhere, Category = "ActorData", meta = (DisplayName = "Actor Template ID"))
	int32 ActorTemplateID;
};
