// Fill out your copyright notice in the Description page of Project Settings.

#include "ColouringBook.h"
#include "ColouringBookCanvas.h"

// Function for enqueueing tasks on the render thread.
// This is taken from: https://wiki.unrealengine.com/Dynamic_Textures and https://wiki.unrealengine.com/Procedural_Materials
void UpdateTextureRegions(UTexture2D* Texture, int32 MipIndex, uint32 NumRegions, FUpdateTextureRegion2D* Regions, uint32 SrcPitch, uint32 SrcBpp, uint8* SrcData, bool bFreeData)
{
	if (Texture->Resource)
	{
		struct FUpdateTextureRegionsData
		{
			FTexture2DResource* Texture2DResource;
			int32 MipIndex;
			uint32 NumRegions;
			FUpdateTextureRegion2D* Regions;
			uint32 SrcPitch;
			uint32 SrcBpp;
			uint8* SrcData;
		};
 
		FUpdateTextureRegionsData* RegionData = new FUpdateTextureRegionsData;
 
		RegionData->Texture2DResource = (FTexture2DResource*)Texture->Resource;
		RegionData->MipIndex = MipIndex;
		RegionData->NumRegions = NumRegions;
		RegionData->Regions = Regions;
		RegionData->SrcPitch = SrcPitch;
		RegionData->SrcBpp = SrcBpp;
		RegionData->SrcData = SrcData;
 
		ENQUEUE_UNIQUE_RENDER_COMMAND_TWOPARAMETER(
			UpdateTextureRegionsData,
			FUpdateTextureRegionsData*, RegionData, RegionData,
			bool, bFreeData, bFreeData,
			{
			for (uint32 RegionIndex = 0; RegionIndex < RegionData->NumRegions; ++RegionIndex)
			{
				int32 CurrentFirstMip = RegionData->Texture2DResource->GetCurrentFirstMip();
				if (RegionData->MipIndex >= CurrentFirstMip)
				{
					RHIUpdateTexture2D(
						RegionData->Texture2DResource->GetTexture2DRHI(),
						RegionData->MipIndex - CurrentFirstMip,
						RegionData->Regions[RegionIndex],
						RegionData->SrcPitch,
						RegionData->SrcData
						+ RegionData->Regions[RegionIndex].SrcY * RegionData->SrcPitch
						+ RegionData->Regions[RegionIndex].SrcX * RegionData->SrcBpp
						);
				}
			}
			if (bFreeData)
			{
				FMemory::Free(RegionData->Regions);
				FMemory::Free(RegionData->SrcData);
			}
			delete RegionData;
		});
	}
}

AColouringBookCanvas::AColouringBookCanvas()
{
	// Defaults
	CanvasResolution = 10;

	// Set callback for when canvas is hit by something
	GetStaticMeshComponent()->OnComponentHit.AddDynamic(this, &AColouringBookCanvas::OnHit);
}

void AColouringBookCanvas::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

void AColouringBookCanvas::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	// Convert the static material in our mesh into a dynamic one, and store it (please note that if you have more than one material that you wish to mark dynamic, do so here).
	dynamicMaterials.Add(GetStaticMeshComponent()->CreateAndSetMaterialInstanceDynamic(0));

	// Set canvas texture size
	FVector scale = GetActorScale3D();
	canvasTextureWidth = CanvasResolution * scale.X;
	canvasTextureHeight = canvasTextureWidth * (scale.Y / scale.X); // Respect the aspect ratio of the actor

	// Create a dynamic texture with the default compression (B8G8R8A8)
	dynamicTexture = UTexture2D::CreateTransient(canvasTextureWidth, canvasTextureHeight);
	dynamicTexture->CompressionSettings = TextureCompressionSettings::TC_VectorDisplacementmap; //Make sure it won't be compressed
	dynamicTexture->SRGB = 0; //Turn off Gamma-correction
	dynamicTexture->AddToRoot(); //Guarantee no garbage collection by adding it as a root reference
	dynamicTexture->UpdateResource(); //Update the texture with new variable values.

	// Initalize our dynamic pixel array with data size
	dynamicColors = new uint8[canvasTextureWidth * canvasTextureHeight * 4]; // * 4 because each color is made out of 4 uint8

	// Initialize texture colors to white
	for (int j = 0; j < canvasTextureHeight; j++)
	{
		for (int i = 0; i < canvasTextureWidth; i++)
		{
			int pixelIndex = (i + j * canvasTextureWidth) * 4;
			dynamicColors[pixelIndex + 0] = 255; // blue
			dynamicColors[pixelIndex + 1] = 255; // green
			dynamicColors[pixelIndex + 2] = 255; // red
			dynamicColors[pixelIndex + 3] = 255; // alpha
		}
	}

	// Create a new texture region with the width and height of our dynamic texture
	updateTextureRegion = new FUpdateTextureRegion2D(0, 0, 0, 0, canvasTextureWidth, canvasTextureHeight);

	// Update texture and assign it to material
	UpdateTextureRegions(dynamicTexture, 0, 1, updateTextureRegion, (uint32)(canvasTextureWidth * 4), (uint32)4, dynamicColors, false);
	dynamicMaterials[0]->SetTextureParameterValue("DynamicTextureParam", dynamicTexture);
}

void AColouringBookCanvas::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if ((OtherActor != NULL) && (OtherActor != this))
	{
		// Get the relative coordinates of the collision inside the canvas
		// Note: FTransform::InverseTransformPosition transforms the coordinates from world space to local space.
		//		Local space is given in the range [-50, 50], and we need coordinates in [0, 1].
		FVector localCoords = (GetTransform().InverseTransformPosition(Hit.ImpactPoint) + 50.0f) * 0.01f;

		// Get texture coordinates of the centre of the circle and its radius
		int ci = localCoords.X * (canvasTextureWidth - 1);
		int cj = localCoords.Y * (canvasTextureHeight - 1);
		static const float SQRT3_INV = 1.0f / FMath::Sqrt(3);
		float radius = OtherActor->GetComponentsBoundingBox().GetSize().Size() * SQRT3_INV * 0.5f; // world space
		radius = GetTransform().InverseTransformVector(GetActorForwardVector() * radius).Size() * 0.01f; // local space
		int r = FMath::Round(radius * canvasTextureWidth);

		// Paint the pixels of the circle
		for (int j = FMath::Max<int>(cj - r, 0); j <= FMath::Min(cj + r, canvasTextureHeight - 1); j++)
		{
			for (int i = FMath::Max<int>(ci - r, 0); i <= FMath::Min<int>(ci + r, canvasTextureWidth - 1); i++)
			{
				int di = FMath::Abs<int>(ci - i);
				int dj = FMath::Abs<int>(cj - j);
				if (di * di + dj * dj <= (r + 0.5f) * (r + 0.5f)) // Only paint the pixels inside the circle
				{
					// Update pixel color
					int pixelIndex = (i + j * canvasTextureWidth) * 4;
					dynamicColors[pixelIndex + 0] = 0; // blue
					dynamicColors[pixelIndex + 1] = 0; // green
				}
			}
		}

		// Update texture and assign it to material
		UpdateTextureRegions(dynamicTexture, 0, 1, updateTextureRegion, (uint32)(canvasTextureWidth * 4), (uint32)4, dynamicColors, false);
		dynamicMaterials[0]->SetTextureParameterValue("DynamicTextureParam", dynamicTexture);

		// Destroy ink drop
		OtherActor->Destroy();
	}
}