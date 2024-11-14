// Copyright © 2024 MajorT. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"

#include "PlaysetTypes.generated.h"

/**
 * FPlaysetDataList
 *
 * Stores data about the playset such as the icon, traits, and levels.
 */
USTRUCT(BlueprintType)
struct GAMEPLAYPLAYSETS_API FPlaysetDataList
{
	GENERATED_BODY()

public:
	FPlaysetDataList();

	/** Gets the thumbnail image of the playset. Performs a blocking load if required. */
	UTexture2D* GetThumbnailImage() const;

public:
	/** The minimum level of the playset. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Playset", meta = (UIMin = "0"))
	int32 MinLevel;

	/** The maximum level of the playset. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Playset", meta = (UIMin = "0"))
	int32 MaxLevel;
	
	/** The icon of this playset, also used as a thumbnail. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Playset")
	TSoftObjectPtr<UTexture2D> Icon;

	/** The large icon of this playset. Will be used as a fallback thumbnail if the icon is not found or too small. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Playset")
	TSoftObjectPtr<UTexture2D> LargeIcon;

	/** Traits that this playset has. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Playset", meta = (Categories = "Item.Trait"))
	FGameplayTagContainer Traits;
};

USTRUCT()
struct FPlaysetActorData
{
	GENERATED_BODY()

public:
	FPlaysetActorData();

public:
	/** The actor blueprint that will be used to spawn the playset. If not set, the playset will be spawned as is. */
	
};
