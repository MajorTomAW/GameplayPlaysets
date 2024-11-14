// Copyright © 2024 MajorT. All rights reserved.


#include "PlaysetThumbnailRenderer.h"

#include "ObjectTools.h"
#include "Playset.h"

bool UPlaysetThumbnailRenderer::CanVisualizeAsset(UObject* Object)
{
	if (GetThumbnailTextureFromObject(Object) == nullptr)
	{
		ClearThumbnailCache(Object);
		return false;
	}

	return true;
}

void UPlaysetThumbnailRenderer::GetThumbnailSize(UObject* Object, float Zoom, uint32& OutWidth, uint32& OutHeight) const
{
	Super::GetThumbnailSize(GetThumbnailTextureFromObject(Object), Zoom, OutWidth, OutHeight);
}

void UPlaysetThumbnailRenderer::Draw(
	UObject* Object, int32 X, int32 Y, uint32 Width, uint32 Height, FRenderTarget* Viewport, FCanvas* Canvas, bool bAdditionalViewFamily)
{
	UTexture2D* ThumbnailTexture = GetThumbnailTextureFromObject(Object);
	if (ThumbnailTexture == nullptr)
	{
		ClearThumbnailCache(Object);
		return;
	}
	
	Super::Draw(ThumbnailTexture, X, Y, Width, Height, Viewport, Canvas, bAdditionalViewFamily);
}

void UPlaysetThumbnailRenderer::ClearThumbnailCache(UObject* Object)
{
	const UPlayset* Playset = Cast<UPlayset>(Object);
	check(Playset);

	UPackage* Package = Playset->GetOutermost();
	ThumbnailTools::CacheEmptyThumbnail(Playset->GetFullName(), Package);
}

UTexture2D* UPlaysetThumbnailRenderer::GetThumbnailTextureFromObject(UObject* Object) const
{
	UPlayset* Playset = Cast<UPlayset>(Object);
	if (Playset && Playset->GetIcon())
	{
		UTexture2D* ThumbnailImage = Playset->GetIcon();
		
		// Avoid calling UpdateResource on cooked texture as doing so will destroy the texture's data
		if (!Playset->GetPackage()->HasAnyPackageFlags(PKG_Cooked | PKG_FilterEditorOnly))
		{
			ThumbnailImage->FinishCachePlatformData();
			ThumbnailImage->UpdateResource();
		}
		return ThumbnailImage;
	}

	return nullptr;
}
