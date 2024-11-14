// Copyright © 2024 MajorT. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "PlaysetDataItem.generated.h"

/**
 * UPlaysetItemData
 *
 * Represents a single fragment in the data list of a playset item.
 */
UCLASS(EditInlineNew, Abstract, CollapseCategories, BlueprintType)
class GAMEPLAYPLAYSETS_API UPlaysetDataItem : public UObject
{
	GENERATED_UCLASS_BODY()

public:

protected:
	/** The name of the data object. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Data", meta = (EditCondition = "false", EditConditionHides))
	FString DataObjectName;

public:
	virtual FString GetDesc() override;
};
