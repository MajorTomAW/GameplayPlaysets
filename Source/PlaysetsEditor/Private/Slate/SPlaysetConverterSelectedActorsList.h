// Copyright © 2024 MajorT. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"
#include "Widgets/Views/STileView.h"

struct FPlaysetSelectedActorEntry
{
public:
	FPlaysetSelectedActorEntry()
		: bIsEnabled(true)
	{
	}
	
	FPlaysetSelectedActorEntry(TWeakObjectPtr<AActor> ActorPtr)
		: AssociatedAsset(ActorPtr.Get(false))
	{
		if (ActorPtr->GetClass()->IsChildOf(UBlueprintGeneratedClass::StaticClass()) ||
			Cast<UBlueprintGeneratedClass>(ActorPtr.Get()->GetClass()))
		{
			bIsEnabled = true;
			bIsNative = false;
		}
		else
		{
			bIsEnabled = false;
			bIsNative = true;
		}
	}

	bool operator==(const FPlaysetSelectedActorEntry& Other) const
	{
		return AssociatedAsset == Other.AssociatedAsset;
	}

	uint32 bIsEnabled:1;
	uint32 bIsNative:1;

	UClass* GetActorClass() const
	{
		return Cast<AActor>(AssociatedAsset.GetAsset())->GetClass();
	}

	FString GetActorLabel() const
	{
		return CastChecked<AActor>(AssociatedAsset.GetAsset())->GetActorLabel();
	}

	FAssetData GetAssetData() const
    {
        return AssociatedAsset;
    }

protected:
	FAssetData AssociatedAsset;
};


class SPlaysetConverterSelectedActorsList : public SCompoundWidget
{
public:
	friend class SPlaysetConverterSelectedActorsListRow;
	using Self = SPlaysetConverterSelectedActorsList;
	
	SLATE_BEGIN_ARGS(SPlaysetConverterSelectedActorsList)
		{
		}
		SLATE_ARGUMENT(TArray<TWeakObjectPtr<AActor>>, SelectedActors)
	SLATE_END_ARGS()
	
	void Construct(const FArguments& InArgs);

	TArray<TSharedRef<FPlaysetSelectedActorEntry>> GetSelectedActorEntries() const;
	
protected:
	FText GetFilterStatusText() const;
	FSlateColor GetFilterStatusTextColor() const;

private:
	TArray<TWeakObjectPtr<AActor>> AllActors;
	
	TSharedPtr<STileView<TSharedRef<FPlaysetSelectedActorEntry>>> SelectionTileView;
	TArray<TSharedRef<FPlaysetSelectedActorEntry>> SelectionEntries;
	TSharedRef<ITableRow> HandleGenerateTile(TSharedRef<FPlaysetSelectedActorEntry> InEntry, const TSharedRef<STableViewBase>& InOwnerTable);

	void PopulateSelectionEntries();
};
