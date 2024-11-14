// Copyright © 2024 MajorT. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "ThumbnailRendering/TextureThumbnailRenderer.h"
#include "PlaysetThumbnailRenderer.generated.h"

UCLASS()
class UPlaysetThumbnailRenderer : public UTextureThumbnailRenderer
{
	GENERATED_BODY()

public:
	//~ Begin UThumbnailRenderer Interface
	virtual bool CanVisualizeAsset(UObject* Object) override;
	virtual void GetThumbnailSize(UObject* Object, float Zoom, uint32& OutWidth, uint32& OutHeight) const override;
	virtual void Draw(UObject* Object, int32 X, int32 Y, uint32 Width, uint32 Height, FRenderTarget* Viewport, FCanvas* Canvas, bool bAdditionalViewFamily) override;
	//~ End UThumbnailRenderer Interface

protected:
	void ClearThumbnailCache(UObject* Object);
	virtual UTexture2D* GetThumbnailTextureFromObject(UObject* Object) const;
};
