// Copyright © 2024 MajorT. All rights reserved.


#include "Factory_Playset.h"

#include "Playset.h"

UFactory_Playset::UFactory_Playset(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	SupportedClass = UPlayset::StaticClass();
	bCreateNew = true;
	bEditAfterNew = true;
}

bool UFactory_Playset::ConfigureProperties()
{
	return true;
}

UObject* UFactory_Playset::FactoryCreateNew(
	UClass* InClass, UObject* InParent, FName InName, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn, FName CallingContext)
{
	check(InClass->IsChildOf(UPlayset::StaticClass()));
	return NewObject<UPlayset>(InParent, InClass, InName, Flags | RF_Transactional, Context);
}

FText UFactory_Playset::GetDisplayName() const
{
	return FText::FromString("Playset Definition");
}

FString UFactory_Playset::GetDefaultNewAssetName() const
{
	return FString(TEXT("New") + UPlayset::StaticClass()->GetName());
}
