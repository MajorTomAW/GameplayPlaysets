// Copyright © 2024 MajorT. All rights reserved.


#include "SPlaysetConverterOptions.h"

#include "PropertyCustomizationHelpers.h"
#include "PropertyEditorUtils.h"
#include "SlateOptMacros.h"
#include "SPlaysetConverter.h"
#include "SPlaysetConverterSelectedActorsList.h"

#define LOCTEXT_NAMESPACE "PlaysetConverter"

FText UPlaysetConverterOptionsObject::GetName() const
{
	return Name.IsEmpty() ? LOCTEXT("DefaultName", "NewPlayset") : Name;
}

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION

void SPlaysetConverterOptions::Construct(const FArguments& InArgs, const TSharedRef<SPlaysetConverter>& InOwner)
{
	FPropertyEditorModule& PropertyEditorModule = FModuleManager::GetModuleChecked<FPropertyEditorModule>("PropertyEditor");

	Owner = InOwner;
	OptionsObject = NewObject<UPlaysetConverterOptionsObject>(GetTransientPackage());
	check(OptionsObject);

	FDetailsViewArgs ViewArgs;
	{
		ViewArgs.bAllowSearch = false;
		ViewArgs.bShowOptions = false;
		ViewArgs.bShowModifiedPropertiesOption = false;
		ViewArgs.bAllowFavoriteSystem = false;
		ViewArgs.bShowPropertyMatrixButton = false;
		ViewArgs.NameAreaSettings = FDetailsViewArgs::HideNameArea;
	}

	DetailsView = PropertyEditorModule.CreateDetailView(ViewArgs);
	DetailsView->SetObject(OptionsObject);
	DetailsView->SetIsPropertyReadOnlyDelegate(FIsPropertyReadOnly::CreateSP(this, &SPlaysetConverterOptions::HandleIsPropertyReadOnly));
	
	ChildSlot
	[
		SNew(SVerticalBox)

		// Details VIew
		+ SVerticalBox::Slot()
		.FillHeight(1.f)
		[
			DetailsView.ToSharedRef()
		]
	];
}
END_SLATE_FUNCTION_BUILD_OPTIMIZATION

void SPlaysetConverterOptions::AddReferencedObjects(FReferenceCollector& Collector)
{
	if (OptionsObject != nullptr)
	{
		Collector.AddReferencedObject(OptionsObject);
	}
}

bool SPlaysetConverterOptions::IsSingleActorSelected() const
{
	if (!Owner.IsValid())
	{
		return false;
	}

	return Owner->GetSelectedActorEntries().Num() == 1;
}

bool SPlaysetConverterOptions::HandleIsPropertyReadOnly(const FPropertyAndParent& PropAndParent) const
{
	if (!Owner.IsValid())
	{
		return true;
	}

	if (Owner->GetSelectedActorEntries().Num() == 0)
	{
		return true;
	}
	
	if (PropAndParent.Property.GetName() == GET_MEMBER_NAME_CHECKED(UPlaysetConverterOptionsObject, bIsSourceActor))
	{
		return !IsSingleActorSelected();
	}
	
	return false;
}

#undef LOCTEXT_NAMESPACE