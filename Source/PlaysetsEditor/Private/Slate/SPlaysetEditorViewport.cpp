// Copyright © 2024 MajorT. All rights reserved.


#include "SPlaysetEditorViewport.h"

#include "AdvancedPreviewScene.h"
#include "NiagaraEditorStyle.h"
#include "Playset.h"
#include "PlaysetRootActor.h"
#include "SlateOptMacros.h"
#include "Kismet/KismetMathLibrary.h"

#define LOCTEXT_NAMESPACE "PlaysetEditorViewport"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION

FPlaysetEditorViewportClient::FPlaysetEditorViewportClient(FAdvancedPreviewScene& InPreviewScene, const TSharedRef<SPlaysetEditorViewport>& InViewport)
	: FEditorViewportClient(nullptr, &InPreviewScene, InViewport)
{
	DrawHelper.bDrawGrid = true;
	DrawHelper.GridColorAxis = FColor(80,80,80);
	DrawHelper.GridColorMajor = FColor(22,22,22);
	DrawHelper.GridColorMinor = FColor(64,64,64);
	DrawHelper.PerspectiveGridSize = UE_BIG_NUMBER;

	EngineShowFlags.SetSnap(false);
	OverrideNearClipPlane(1.f);
	bUsingOrbitCamera = false;
}

void SPlaysetEditorViewport::Construct(const FArguments& InArgs, TSharedPtr<FPlaysetEditorApp> InPlaysetApp)
{
	AppPtr = InPlaysetApp;
	
	AdvancedPreviewScene = MakeShareable(new FAdvancedPreviewScene(FPreviewScene::ConstructionValues()));
	AdvancedPreviewScene->SetFloorVisibility(true);
	AdvancedPreviewScene->SetFloorOffset(5.f);

	const FRotator LightDir(-40.f, 128.f, 0.f);
	AdvancedPreviewScene->SetLightDirection(LightDir);

	SEditorViewport::Construct(SEditorViewport::FArguments());
	Client->EngineShowFlags.SetGrid(true);
	Client->EngineShowFlags.SetPivot(true);
}

SPlaysetEditorViewport::~SPlaysetEditorViewport()
{
	if (PreviewComponent && AdvancedPreviewScene.IsValid())
	{
		AdvancedPreviewScene->RemoveComponent(PreviewComponent);
		PreviewComponent->DestroyComponent();
		PreviewComponent->MarkAsGarbage();
		
		AdvancedPreviewScene->GetScene()->OnWorldCleanup();
		AdvancedPreviewScene.Reset();
	}

	if (SystemViewportClient.IsValid())
	{
		SystemViewportClient->Invalidate();
		SystemViewportClient->Viewport = nullptr;
	}
}

TSharedRef<FEditorViewportClient> SPlaysetEditorViewport::MakeEditorViewportClient()
{
	SystemViewportClient = MakeShareable(new FPlaysetEditorViewportClient(*AdvancedPreviewScene.Get(), SharedThis(this)));
	{
		const FVector ViewLocation = FVector(0.7f, 0.7f, 1.f);
		constexpr float ViewDistance = 400.f;
		
		SystemViewportClient->SetViewLocation(ViewLocation * ViewDistance);

		// Find look at origin
		const FRotator ViewRotation = UKismetMathLibrary::FindLookAtRotation(ViewLocation, FVector::ZeroVector); 
		SystemViewportClient->SetViewRotation(ViewRotation);
		

		SystemViewportClient->ToggleOrbitCamera(false);
		
		SystemViewportClient->bSetListenerPosition = false;
		SystemViewportClient->SetRealtime(true);
		SystemViewportClient->SetGameView(false);
		SystemViewportClient->ExposureSettings.bFixed = true;
		SystemViewportClient->ExposureSettings.FixedEV100 = -1.f;
		SystemViewportClient->SetShowStats(false);
	}
	
	return SystemViewportClient.ToSharedRef();
}

TSharedPtr<SWidget> SPlaysetEditorViewport::MakeViewportToolbar()
{
	return SNew(SCommonEditorViewportToolbarBase, SharedThis(this))
		.AddRealtimeButton(true);
}

void SPlaysetEditorViewport::PopulateViewportOverlays(TSharedRef<SOverlay> Overlay)
{
	Overlay->AddSlot()
	.VAlign(VAlign_Center)
	.HAlign(HAlign_Center)
	[
		SAssignNew(StatusText, STextBlock)
		.Text(this, &SPlaysetEditorViewport::GetViewportStatusText)
		.TextStyle(FNiagaraEditorStyle::Get(), "NiagaraEditor.Viewport.CompileOverlay")
		.ColorAndOpacity(FLinearColor::White)
		.ShadowOffset(0.8f)
		.ShadowColorAndOpacity(FLinearColor(0.f, 0.f, 0.f, 0.9f))
	];
	
	SEditorViewport::PopulateViewportOverlays(Overlay);
}

TSharedRef<class SEditorViewport> SPlaysetEditorViewport::GetViewportWidget()
{
	return SharedThis(this);
}

TSharedPtr<FExtender> SPlaysetEditorViewport::GetExtenders() const
{
	TSharedPtr<FExtender> Result(MakeShareable(new FExtender));
	return Result;
}

void SPlaysetEditorViewport::OnFloatingButtonClicked()
{
}

void SPlaysetEditorViewport::AddReferencedObjects(FReferenceCollector& Collector)
{
	if (PreviewComponent != nullptr)
	{
		Collector.AddReferencedObject(PreviewComponent);
	}

	Collector.AddReferencedObjects(AdditionalComponents);
}

void SPlaysetEditorViewport::CleanUpScene()
{
	if (!AdvancedPreviewScene.IsValid())
	{
		return;
	}

	AdvancedPreviewScene->RemoveComponent(PreviewComponent);
	PreviewComponent->DestroyComponent();
	PreviewComponent->MarkAsGarbage();
	PreviewComponent = nullptr;

	for (auto It = AdditionalComponents.CreateIterator(); It; ++It)
	{
		USceneComponent* Component = *It;
		AdvancedPreviewScene->RemoveComponent(Component);
		Component->DestroyComponent();
		Component->MarkAsGarbage();
	}

	AdditionalComponents.Reset();
}


void SPlaysetEditorViewport::UpdateViewport(UPlayset* OwningPlayset)
{
	if (!AdvancedPreviewScene.IsValid())
	{
		return;
	}
	
	if (PreviewComponent && OwningPlayset == nullptr)
	{
		CleanUpScene();

		AdvancedPreviewScene->GetScene()->OnWorldCleanup();
		SystemViewportClient->Invalidate();
	}
	else if (OwningPlayset == nullptr)
	{
		AdvancedPreviewScene->GetScene()->OnWorldCleanup();
		SystemViewportClient->Invalidate();
	}

	if (OwningPlayset == nullptr)
	{
		return;
	}

	FActorSpawnParameters SpawnParams;
	{
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		SpawnParams.ObjectFlags = RF_Transient | RF_Transactional;
	}
	APlaysetRootActor* RootActor = AdvancedPreviewScene->GetWorld()->SpawnActor<APlaysetRootActor>(APlaysetRootActor::StaticClass(), FTransform::Identity, SpawnParams);
	check(RootActor);

	PreviewComponent = RootActor->GetRootComponent();
	AdvancedPreviewScene->AddComponent(PreviewComponent, FTransform::Identity);

	for (const FPlaysetActorData& Data : OwningPlayset->ActorData)
	{
		UClass* Class = Data.ActorClass.LoadSynchronous();
		if (Class == nullptr)
		{
			continue;
		}

		AActor* Actor = AdvancedPreviewScene->GetWorld()->SpawnActor<AActor>(Class, Data.RelativeTransform, SpawnParams);
		check(Actor);

		Actor->AttachToActor(RootActor, FAttachmentTransformRules::KeepRelativeTransform);
		AdvancedPreviewScene->AddComponent(Actor->GetRootComponent(), FTransform::Identity);
	}
}


FText SPlaysetEditorViewport::GetViewportStatusText() const
{
	if (PreviewComponent)
	{
		return FText::GetEmpty();
	}

	return LOCTEXT("PlaysetEditorViewportNoPreview", "Nothing to show here.");
}


END_SLATE_FUNCTION_BUILD_OPTIMIZATION

#undef LOCTEXT_NAMESPACE