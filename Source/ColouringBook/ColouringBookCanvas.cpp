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

}

void AColouringBookCanvas::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

void AColouringBookCanvas::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	//Convert the static material in our mesh into a dynamic one, and store it (please note that if you have more than one material that you wish to mark dynamic, do so here).
	dynamicMaterials.Add(GetStaticMeshComponent()->CreateAndSetMaterialInstanceDynamic(0));

	//Create a dynamic texture with the default compression (B8G8R8A8)
	int32 w, h;
	w = 210;
	h = 297;
	dynamicTexture = UTexture2D::CreateTransient(w, h);
	dynamicTexture->CompressionSettings = TextureCompressionSettings::TC_VectorDisplacementmap; //Make sure it won't be compressed
	dynamicTexture->SRGB = 0; //Turn off Gamma-correction
	dynamicTexture->AddToRoot(); //Guarantee no garbage collection by adding it as a root reference
	dynamicTexture->UpdateResource(); //Update the texture with new variable values.

	// Initalize our dynamic pixel array with data size
	dynamicColors = new uint8[w * h * 4]; // * 4 because each color is made out of 4 uint8

	// Initialize colors
	for (int i = 0; i < h; i++)
	{
		for (int j = 0; j < w; j++) 
		{
			int pixelIndex = (i * w + j) * 4;

			if ((i - h * 0.5f) * (i - h * 0.5f) + (j - w * 0.5f) * (j - w * 0.5f) < 1000)
			{
				// draw blue circle in the middle
				dynamicColors[pixelIndex + 0] = 255; // blue
				dynamicColors[pixelIndex + 2] = 0;
			}
			else
			{
				// black & red checkered background
				dynamicColors[pixelIndex + 0] = 0; // blue
				dynamicColors[pixelIndex + 2] = (i % 40 < 20) == (j % 40 < 20) ? 0 : 255; // red
			}

			dynamicColors[pixelIndex + 1] = 0; // green
			dynamicColors[pixelIndex + 3] = 255; // alpha
		}
	}

	// Create a new texture region with the width and height of our dynamic texture
	updateTextureRegion = new FUpdateTextureRegion2D(0, 0, 0, 0, w, h);

	// Update texture and assign it to material
	UpdateTextureRegions(dynamicTexture, 0, 1, updateTextureRegion, (uint32)(w * 4), (uint32)4, dynamicColors, false);
	dynamicMaterials[0]->SetTextureParameterValue("DynamicTextureParam", dynamicTexture);
}