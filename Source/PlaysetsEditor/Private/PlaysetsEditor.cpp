#include "PlaysetsEditor.h"

#include "Playset.h"
#include "PlaysetThumbnailRenderer.h"
//#include "Application/PlaysetEditorApp.h"
#include "ContentBrowserModule.h"
#include "EngineUtils.h"
#include "IContentBrowserSingleton.h"
#include "PlaysetRootActor.h"
#include "Selection.h"
#include "Application/PlaysetEditorApp.h"
#include "AssetRegistry/AssetRegistryModule.h"
#include "Data/PlaysetDisplayItem_Icon.h"
#include "Framework/Notifications/NotificationManager.h"
#include "Interfaces/IMainFrameModule.h"
#include "Slate/PlaysetDataListCommands.h"
#include "Slate/SPlaysetConverterOptions.h"
#include "Slate/SPlaysetConverterWindow.h"
#include "Style/PlaysetStyle.h"
#include "Styling/SlateIconFinder.h"
#include "Subsystems/EditorAssetSubsystem.h"
#include "ThumbnailRendering/ThumbnailManager.h"
#include "Widgets/Notifications/SNotificationList.h"

#define LOCTEXT_NAMESPACE "FPlaysetsEditorModule"

UE::Playset::Editor::FPlaysetInitArgs::FPlaysetInitArgs(UObject* ConverterObj, TArray<FAssetData> InSelectedActors)
{
	if (UPlaysetConverterOptionsObject* Options = Cast<UPlaysetConverterOptionsObject>(ConverterObj))
	{
		Name = Options->Name;
		ShortDescription = Options->ShortDescription;
		LongDescription = Options->LongDescription;
		SmallThumbnail = FAssetData(Options->SmallThumbnail);
		LargeThumbnail = FAssetData(Options->LargeThumbnail);
		
		bIsSourceActor = (Options->bIsSourceActor) && (InSelectedActors.Num() == 1);

		OffsetType = Options->OffsetType;
	}

	SelectedActors = InSelectedActors;
}

UE::Playset::Editor::FPlaysetInitArgs::FPlaysetInitArgs(
	FText InName, FText InShortDesc, FText InLongDesc, UTexture2D* InSmallThumbnail, UTexture2D* InLargeThumbnail, TArray<FAssetData> InSelectedActors, bool InIsSourceActor, EPlaysetOffsetType InOffsetType)
{
	Name = InName;
	ShortDescription = InShortDesc;
	LongDescription = InLongDesc;
	SmallThumbnail = FAssetData(InSmallThumbnail);
	LargeThumbnail = FAssetData(InLargeThumbnail);
	SelectedActors = InSelectedActors;

	bIsSourceActor = InIsSourceActor && (InSelectedActors.Num() == 1);

	OffsetType = InOffsetType;
}

void UE::Playset::Editor::InitializePlayset(UPlayset* Playset, const FPlaysetInitArgs& Args)
{
	check(Playset);
	
	Playset->ItemName = Args.GetName();
	Playset->ItemShortDescription = Args.GetShortDescription();
	Playset->ItemDescription = Args.GetLongDescription();

	// Set the thumbnails
	if (UTexture2D* SmallThumbnail = Args.GetSmallThumbnail())
	{
		UPlaysetDisplayItem_Icon* SmallIconItem = NewObject<UPlaysetDisplayItem_Icon>(Playset, UPlaysetDisplayItem_Icon::StaticClass(), NAME_None, RF_Public | RF_Standalone);
		SmallIconItem->SetIcon(SmallThumbnail);

		Playset->DisplayListObjects.Add(SmallIconItem);
	}

	if (UTexture2D* LargeThumbnail = Args.GetLargeThumbnail())
	{
		UPlaysetDisplayItem_LargeIcon* LargeIconItem = NewObject<UPlaysetDisplayItem_LargeIcon>(Playset, UPlaysetDisplayItem_LargeIcon::StaticClass(), NAME_None, RF_Public | RF_Standalone);
		LargeIconItem->SetLargeIcon(LargeThumbnail);

		Playset->DisplayListObjects.Add(LargeIconItem);
	}

	// Set source actor
	if (Args.IsSourceActor())
	{
		const FAssetData SourceAsset = Args.GetSelectedActors().Top();
		Playset->SourceActorBlueprint = SourceAsset.GetClass(EResolveClass::No);
	}

	// Initialize all selected actors
	if (Args.GetSelectedActors().Num() > 1) // 1 = source actor, > 1 = multiple actors so we need to initialize them
	{
		InitializePlaysetActiveAssets(Playset, Args);
	}

	Playset->MarkPackageDirty();
}

void UE::Playset::Editor::InitializePlaysetActiveAssets(UPlayset* Playset, const FPlaysetInitArgs& Args)
{
	check(Playset);

	// Initialize all selected actors
	TArray<AActor*> SelectedActors;
	for (const FAssetData& Asset : Args.GetSelectedActors())
	{
		if (AActor* Actor = Cast<AActor>(Asset.GetAsset()))
		{
			SelectedActors.Add(Actor);
		}
	}

	// Spawn the root actor
	UWorld* World = GEditor->GetEditorWorldContext().World();
	FActorSpawnParameters SpawnParams;
	{
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		SpawnParams.bNoFail = true;
	}
	APlaysetRootActor* RootActor = World->SpawnActor<APlaysetRootActor>(APlaysetRootActor::StaticClass(), FTransform::Identity, SpawnParams);
	check(RootActor);

	AActor* FirstActor = SelectedActors[0];
	if (!ensure(FirstActor))
	{
		return;
	}

	RootActor->SetActorLocation(FirstActor->GetActorLocation());
	RootActor->SetActorRotation(FirstActor->GetActorRotation());

	FBoxSphereBounds Bounds = CalculateActiveAssetBounds(RootActor, Args.GetSelectedActors());
	DrawDebugBox(World, Bounds.Origin, Bounds.BoxExtent, FColor::Red, false, 5.f, 0, 1.f);


	// Restore any locations
	const FVector InitLocation = RootActor->GetActorLocation();
	FVector NewLocation = Bounds.Origin;
	NewLocation.Z -= Bounds.BoxExtent.Z;

	RootActor->SetActorLocation(NewLocation);

	for (AActor* Actor : SelectedActors)
	{
		if (Actor == nullptr)
		{
			continue;
		}

		Actor->SetActorLocation(
			(Actor->GetActorLocation() +
			(InitLocation - RootActor->GetActorLocation())));
	}

	// Save the data to the playset
	SaveActorData(Playset, SelectedActors);
	Playset->UpdateExtent(Bounds);

	// Change selection to the root actor
	GEditor->SelectNone(true, true);
	GEditor->SelectActor(RootActor, true, true);
}

void UE::Playset::Editor::SaveActorData(UPlayset* Playset, const TArray<AActor*>& SelectedActors)
{
	for (AActor* Actor : SelectedActors)
	{
		const UClass* Class = Actor->GetClass();
		int32& Count = Playset->ActorClassCount.FindOrAdd(Class);
		Count++;

		FPlaysetActorData& NewData = Playset->ActorData.AddDefaulted_GetRef();
		NewData.ActorClass = Class;
		NewData.InfluenceDistance = Actor->GetSimpleCollisionRadius();
		NewData.RelativeTransform = Actor->GetActorTransform();
	}
}

FBoxSphereBounds UE::Playset::Editor::CalculateActiveAssetBounds(AActor* Origin, const TArray<FAssetData>& SelectedActors)
{
	FBoxSphereBounds SharedBounds;
	SharedBounds.Origin = Origin->GetActorLocation();

	for (const FAssetData& Asset : SelectedActors)
	{
		AActor* Actor = Cast<AActor>(Asset.GetAsset());
		if (!ensure(Actor))
		{
			continue;
		}

		Actor->GetRootComponent()->Mobility = EComponentMobility::Type::Movable;
		Actor->AttachToActor(Origin, FAttachmentTransformRules::KeepWorldTransform);

		SharedBounds = SharedBounds + Actor->GetStreamingBounds();
	}

	return SharedBounds;
}

namespace LevelEditorExtension_Playset
{
	void Execute_ConvertToPlayset()
	{
		TArray<TWeakObjectPtr<AActor>> SelectedActors;
		USelection* Selection = GEditor->GetSelectedActors();
		for (FSelectionIterator It(*Selection); It; ++It)
		{
			if (AActor* Actor = Cast<AActor>(*It))
			{
				SelectedActors.Add(Actor);
			}
		}

		const IMainFrameModule& MainFrame = FModuleManager::LoadModuleChecked<IMainFrameModule>("MainFrame");
		const TSharedPtr<SWindow> ParentWindow = MainFrame.GetParentWindow();

		SPlaysetConverterWindow::FArguments WindowArgs;
		WindowArgs.SelectedActors(SelectedActors);

		TSharedRef<SPlaysetConverterWindow> ConverterWindow = SNew(SPlaysetConverterWindow)
			.SelectedActors(SelectedActors);
		
		FSlateApplication::Get().AddModalWindow(ConverterWindow, ParentWindow, false);

		if (!ConverterWindow->ShouldProceedWithAction() || !ConverterWindow->GetOptionsObject())
		{
			return;
		}

		TArray<FAssetData> AssetsToConvert = ConverterWindow->GetSelectedAssets();
		UPlaysetConverterOptionsObject* Options = ConverterWindow->GetOptionsObject();

		// Setup the options for the new playset
		const FString PlaysetName = TEXT("PID_") + Options->GetNameString();
		const FString PathName = FPaths::GetPath(AssetsToConvert.Top().GetPackage()->GetPathName());

		FSaveAssetDialogConfig SaveConfig;
		{
			SaveConfig.DialogTitleOverride = LOCTEXT("SavePlaysetDialogTitle", "Save Playset");
			SaveConfig.DefaultPath = PathName;
			SaveConfig.DefaultAssetName = PlaysetName;
			SaveConfig.ExistingAssetPolicy = ESaveAssetDialogExistingAssetPolicy::Disallow;
		}

		const FContentBrowserModule& ContentBrowserModule = FModuleManager::LoadModuleChecked<FContentBrowserModule>("ContentBrowser");
		const FString SaveObjectPath = ContentBrowserModule.Get().CreateModalSaveAssetDialog(SaveConfig);
		if (SaveObjectPath.IsEmpty())
		{
			return;
		}

		const FString SavePackageName = FPackageName::ObjectPathToPackageName(SaveObjectPath);
		const FString SavePackagePath = FPaths::GetPath(SavePackageName);
		const FString SaveAssetName = FPaths::GetBaseFilename(SavePackageName);

		UPackage* Package = CreatePackage(*SavePackageName);
		if (ensure(Package))
		{
			UPlayset* NewPlayset = NewObject<UPlayset>(Package, *SaveAssetName, RF_Public | RF_Standalone);
			check(NewPlayset);

			UE::Playset::Editor::FPlaysetInitArgs InitArgs(Options, AssetsToConvert);
			InitializePlayset(NewPlayset, InitArgs);

			UEditorAssetSubsystem* EdSub = GEditor->GetEditorSubsystem<UEditorAssetSubsystem>();
			UAssetEditorSubsystem* AssetEdSub = GEditor->GetEditorSubsystem<UAssetEditorSubsystem>();
			AssetEdSub->OpenEditorForAsset(NewPlayset);

			FAssetRegistryModule::AssetCreated(NewPlayset);
			Package->MarkPackageDirty();
		}

		FSlateApplication::Get().DismissAllMenus();
	}

	bool CanExecute_ConvertToPlayset()
	{
		return true;
	}

	void Execute_CleanUpPlaysetRoots()
	{
		UWorld* World = GEditor->GetEditorWorldContext().World();
		if (!World)
		{
			return;
		}

		const FScopedTransaction Transaction(LOCTEXT("CleanUpPlaysetRoots", "Clean Up Playset Roots"));
		World->Modify();

		int32 NumRemoved = 0;

		// Remove any PlaysetRoot actors that are no longer needed
		for (TActorIterator<APlaysetRootActor> It(World); It; ++It)
		{
			AActor* Actor = *It;
			
			TArray<AActor*> Children;
			Actor->GetAttachedActors(Children);
			
			if (Children.Num() > 0)
			{
				continue;
			}

			// Remove the actor
			Actor->Destroy();
			NumRemoved++;
		}

		// Show a notification
		const FText NotificationText = FText::Format(LOCTEXT("CleanUpPlaysetRootsNotificationText", "Cleaned up {0} PlaysetRoot actor(s)"), FText::AsNumber(NumRemoved));
		FNotificationInfo Info(NotificationText);
		Info.ExpireDuration = 3.f;
		FSlateNotificationManager::Get().AddNotification(Info);
	}

	bool CanExecute_CleanUpPlaysetRoots()
	{
		return true;
	}
	
	void AddLevelEditorExtensions()
	{
		// Get the icon
		const FSlateIcon AssetIcon = FSlateIconFinder::FindIconForClass(UPlayset::StaticClass());

		UToolMenu* Menu = UToolMenus::Get()->FindMenu("LevelEditor.ActorContextMenu");
		if (Menu == nullptr)
		{
			return;
		}

		FToolMenuSection& Section = Menu->FindOrAddSection("PlaysetOptions", LOCTEXT("AssetOptionsHeading", "Playset Options"));
		Section.InsertPosition.Position = EToolMenuInsertType::First;
		Section.Label = LOCTEXT("AssetOptionsHeading", "Playset Options");

		// Convert to Playset
		Section.AddMenuEntry
		(
			"Playset_ConvertToPlayset",
			LOCTEXT("Playset_ConvertToPlayset", "Convert to Playset"),
			LOCTEXT("Playset_ConvertToPlayset_Tooltip", "Convert the selected actor(s) to a Playset"),
			AssetIcon,
			FUIAction
			(
				FExecuteAction::CreateStatic(&Execute_ConvertToPlayset),
				FCanExecuteAction::CreateStatic(&CanExecute_ConvertToPlayset)
			)
		);

		// Clean up PlaysetRoots
		Section.AddMenuEntry
		(
			"Playset_CleanUpPlaysetRoots",
			LOCTEXT("Playset_CleanUpPlaysetRoots", "Clean Up Playset Roots"),
			LOCTEXT("Playset_CleanUpPlaysetRoots_Tooltip", "Remove any PlaysetRoot actors that are no longer needed"),
			FSlateIcon(FAppStyle::GetAppStyleSetName(), "GraphEditor.Clean"),
			FUIAction
			(
				FExecuteAction::CreateStatic(&Execute_CleanUpPlaysetRoots),
				FCanExecuteAction::CreateStatic(&CanExecute_CleanUpPlaysetRoots)
			)
		);
	}
}


class FPlaysetsEditorModule final : public IPlaysetsEditorModule
{
public:
	using ThisClass = FPlaysetsEditorModule;
	
	//~ Begin IModuleInterface interface
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
	//~ End IModuleInterface interface

	virtual TSharedRef<FWorkflowCentricApplication> CreatePlaysetsEditor(const EToolkitMode::Type Mode, const TSharedPtr<IToolkitHost>& InitToolkitHost, UObject* InObject) override;

protected:
	void OnPostEngineInit();
};
IMPLEMENT_MODULE(FPlaysetsEditorModule, PlaysetsEditor);

void FPlaysetsEditorModule::StartupModule()
{
	FCoreDelegates::OnPostEngineInit.AddRaw(this, &ThisClass::OnPostEngineInit);
}

void FPlaysetsEditorModule::ShutdownModule()
{
	IPlaysetsEditorModule::ShutdownModule();

	// Unregister style set
	FPlaysetStyle::Shutdown();
	FPlaysetDataListCommands::Unregister();
}

TSharedRef<FWorkflowCentricApplication> FPlaysetsEditorModule::CreatePlaysetsEditor(
	const EToolkitMode::Type Mode, const TSharedPtr<IToolkitHost>& InitToolkitHost, UObject* InObject)
{
	TSharedRef<FPlaysetEditorApp> NewPlaysetEditor(new FPlaysetEditorApp());
	NewPlaysetEditor->CreatePlaysetEditorApp(Mode, InitToolkitHost, InObject);
	return NewPlaysetEditor;
}

void FPlaysetsEditorModule::OnPostEngineInit()
{
	FPlaysetStyle::Get();
	FPlaysetDataListCommands::Register();
	LevelEditorExtension_Playset::AddLevelEditorExtensions();

	if (GIsEditor)
	{
		UThumbnailManager::Get().RegisterCustomRenderer(UPlayset::StaticClass(), UPlaysetThumbnailRenderer::StaticClass());
	}
}

#undef LOCTEXT_NAMESPACE
