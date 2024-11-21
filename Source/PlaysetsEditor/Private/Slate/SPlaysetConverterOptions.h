// Copyright © 2024 MajorT. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "PlaysetTypes.h"
#include "Widgets/SCompoundWidget.h"

#include "SPlaysetConverterOptions.generated.h"

class SPlaysetConverter;

UCLASS(NotBlueprintable, NotBlueprintType)
class UPlaysetConverterOptionsObject : public UObject
{
	GENERATED_BODY()

public:
	FText GetName() const;
	FString GetNameString() const { return GetName().ToString(); }

public:
	/** Playset Name */
	UPROPERTY(EditAnywhere, Category = "Playset Converter")
	FText Name;

	/** Playset show single-line description */
	UPROPERTY(EditAnywhere, Category = "Playset Converter")
	FText ShortDescription;

	/** Playset show multi-line description */
	UPROPERTY(EditAnywhere, Category = "Playset Converter", meta = (MultiLine = true))
	FText LongDescription;

	/** Playset small thumbnail (128x128) */
	UPROPERTY(EditAnywhere, Category = "Playset Converter")
	TObjectPtr<UTexture2D> SmallThumbnail = nullptr;

	/** Playset large thumbnail (256x256) */
	UPROPERTY(EditAnywhere, Category = "Playset Converter")
	TObjectPtr<UTexture2D> LargeThumbnail = nullptr;

	/** Whether to use the selected actor as the source actor blueprint */
	UPROPERTY(EditAnywhere, Category = "Playset Converter")
	bool bIsSourceActor = true;

	/** Whether to automatically calculate the playset bounding box */
	/** Where the playset gets spawned relative to its bounding box */
	UPROPERTY(EditAnywhere, Category = "Playset Converter")
	EPlaysetOffsetType OffsetType = EPlaysetOffsetType::Center;
	
	/** Minimum distance the playset can be in the ground before snapping upwards */
};

class SPlaysetConverterOptions : public SCompoundWidget, public FGCObject
{
public:
	SLATE_BEGIN_ARGS(SPlaysetConverterOptions)
		{
		}
	SLATE_END_ARGS()
	
	void Construct(const FArguments& InArgs, const TSharedRef<SPlaysetConverter>& InOwner);

	virtual void AddReferencedObjects(FReferenceCollector& Collector) override;
	virtual FString GetReferencerName() const override { return TEXT("SPlaysetConverterOptions"); }

	bool IsSingleActorSelected() const;

	UPlaysetConverterOptionsObject* GetOptionsObject() const { return OptionsObject.Get(); }

protected:
	TObjectPtr<UPlaysetConverterOptionsObject> OptionsObject;
	TSharedPtr<class IDetailsView> DetailsView;
	TSharedPtr<SPlaysetConverter> Owner;

	bool HandleIsPropertyReadOnly(const FPropertyAndParent& PropAndParent) const;
};
