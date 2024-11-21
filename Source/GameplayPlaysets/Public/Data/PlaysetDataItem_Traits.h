// Copyright © 2024 MajorT. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "PlaysetDataItem.h"
#include "PlaysetDataItem_Traits.generated.h"

/**
 * Item data for adding tagged traits to the item.
 */
UCLASS(meta = (DisplayName = "Traits Item Data"))
class UPlaysetDataItem_Traits : public UPlaysetDataItem
{
	GENERATED_UCLASS_BODY()

public:
	virtual FString GetDesc() override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Traits", meta = (Categories = "Item.Trait"))
	FGameplayTagContainer Traits;
};
