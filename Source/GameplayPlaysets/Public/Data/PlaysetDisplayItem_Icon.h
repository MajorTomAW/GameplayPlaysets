// Copyright © 2024 MajorT. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "PlaysetDataItem_Display.h"
#include "PlaysetDisplayItem_Icon.generated.h"

/** Item Data for adding an icon for the item to display in the UI. */
UCLASS(meta = (DisplayName = "Icon Item Data"))
class UPlaysetDisplayItem_Icon : public UPlaysetDataItem_Display
{
	GENERATED_UCLASS_BODY()
	friend class UPlayset;

public:
	virtual FString GetDesc() override;

protected:
	/** The icon of this item. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Icon")
	TSoftObjectPtr<UTexture2D> Icon;
};

/** Item data for adding a larger icon for the item to display in the UI. Should be used ONLY if required! */
UCLASS(meta = (DisplayName = "Large Icon Item Data"))
class UPlaysetDisplayItem_LargeIcon : public UPlaysetDataItem_Display
{
	GENERATED_UCLASS_BODY()
	friend class UPlayset;

public:
	virtual FString GetDesc() override;

protected:
	/** The large icon of this item. Should be used ONLY if required! */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Icon")
	TSoftObjectPtr<UTexture2D> LargeIcon;
};
