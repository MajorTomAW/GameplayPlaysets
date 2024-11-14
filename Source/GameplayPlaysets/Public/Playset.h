// Copyright © 2024 MajorT. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "PlaysetDataItem.h"
#include "PlaysetTypes.h"
#include "UObject/Object.h"
#include "Playset.generated.h"

class UPlaysetDataItem_Display;
/**
 * Adds a system for creating and managing playsets, which are collections of actors that can be used to define gameplay rules, objectives, and more.
 * Multiple actors that are spawned in a world can be converted to a playset that can be re-used, multiple times, in the same or other worlds.
 *
 *
 * A playset can be anything from a simple collection of actors to a complex set of rules, objectives, items, quests, and more.
 */
UCLASS(BlueprintType, Blueprintable, PrioritizeCategories = ("Playset"))
class GAMEPLAYPLAYSETS_API UPlayset : public UPrimaryDataAsset
{
	GENERATED_UCLASS_BODY()

public:
	//~ Begin UObject Interface
	virtual FPrimaryAssetId GetPrimaryAssetId() const override;
	//~ End UObject Interface

	/** Returns the gameplay tags of this playset. */
	UFUNCTION(BlueprintCallable, Category = "Playset")
	virtual const FGameplayTagContainer& GetGameplayTags() const;

	/** Returns the icon of the playset. */
	UFUNCTION(BlueprintCallable, Category = "Playset")
	virtual UTexture2D* GetIcon(const bool bUseLargeIfNotFound = true) const;

public:
	/** The display/friendly name of this playset that will be facing the player. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Display", AssetRegistrySearchable, meta = (DisplayName = "Display Name"))
	FText ItemName;

	/** The description of this playset for summaries. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Display", meta = (MultiLine = true, DisplayName = "Description"))
	FText ItemDescription;

	/** The short description of this playset. Mostly used for tooltips that should be short. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Display", meta = (MultiLine = true, DisplayName = "Short Description"))
	FText ItemShortDescription;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Display", Instanced, meta = (ShowOnlyInnerProperties = true))
	TSet<TObjectPtr<UPlaysetDataItem_Display>> DisplayListObjects;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Instanced, Category = "Playset", meta = (ShowOnlyInnerProperties = true))
	TSet<UPlaysetDataItem*> DataListObjects;

	/** The URL to the documentation of this playset (optional). */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Playset", AdvancedDisplay, meta = (DisplayName = "Documentation URL"))
	FString DocumentationURL;

	/** The source actor blueprint that will be used to spawn the playset. If not set, the playset will be spawned as is. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Actor Data", AssetRegistrySearchable, meta = (MetaClass = "/Script/Engine.Actor", ShowTreeView = true))
	FSoftClassPath SourceActorBlueprint;

	/** Tags that this playset has. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Tags", meta = (DisplayName = "Tags"))
	FGameplayTagContainer PlaysetTags;

	/** Whether this playset should adjust for world collision when dragged into the world. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Placement", meta = (DisplayName = "Adjust for World Collision"))
	uint32 bAdjustForWorldCollision : 1;

	/** The size of this playset on the X-axis. Used as a virtual extent for the placement. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Placement|Extent", meta = (DisplayName = "Size X", ClampMin = 0, UIMin = 0, Units = m, EditCondition = "bAdjustForWorldCollision"))
	float SizeX;

	/** The size of this playset on the Y-axis. Used as a virtual extent for the placement. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Placement|Extent", meta = (DisplayName = "Size Y", ClampMin = 0, UIMin = 0, Units = m, EditCondition = "bAdjustForWorldCollision"))
	float SizeY;

	/** The size of this playset on the Z-axis. Used as a virtual extent for the placement. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Placement|Extent", meta = (DisplayName = "Size Z", ClampMin = 0, UIMin = 0, Units = m, EditCondition = "bAdjustForWorldCollision"))
	float SizeZ;

	/** The default location offset to use when placed into the world. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Placement|Offset", meta = (DisplayName = "Default Location"))
	FVector DefaultLocation;

	/** The default rotation to use when placed into the world. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Placement|Offset", meta = (DisplayName = "Default Rotation"))
	FRotator DefaultRotation;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Placement")
	TMap<FSoftClassPath, int32> ActorClassCount;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Placement")
	TSet<TSoftObjectPtr<UPlayset>> AssociatedPlaysets;

protected:
#if WITH_EDITOR
	//~ Begin UObject Interface
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
	virtual void PostSaveRoot(FObjectPostSaveRootContext ObjectSaveContext) override;
	virtual EDataValidationResult IsDataValid(FDataValidationContext& Context) const override;
	//~ End UObject Interface
#endif

	/** The primary asset type that will be used for this playset. */
	UPROPERTY(AssetRegistrySearchable)
	FName PrimaryAssetType = "Playset";

	/** The primary asset name that will be used for this playset. */
	UPROPERTY(AssetRegistrySearchable)
	FName PrimaryAssetName;
};
