// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine/StaticMeshActor.h"
#include "ColouringBookCanvas.generated.h"

/**
 * 
 */
UCLASS()
class COLOURINGBOOK_API AColouringBookCanvas : public AStaticMeshActor
{
	GENERATED_BODY()
	
	/* The resolution of the canvas texture.
	 * The value will be affected by the actor's scale, so the resolution is will be in terms of world space
	 *	(same resolution fo actors of different shapes and scales will produce textures with the same quality on screen).
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Canvas, meta = (AllowPrivateAccess = "true"))
	float CanvasResolution;

	/* The mask texture to use with the painting */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Canvas, meta = (AllowPrivateAccess = "true"))
	class UTexture2D* MaskTexture;

	/* When the mask debug mode is on, only the pixels of the canvas that are inside the painting (and count towards score) will be colored.*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Canvas, meta = (AllowPrivateAccess = "true"))
	bool MaskDebugModeOn;

public:
	
	AColouringBookCanvas();

	// Begin Actor Interface
	virtual void Tick(float DeltaSeconds) override;
	virtual void PostInitializeComponents() override;
	// End Actor Interface

	// Function to handle the canvas being hit by something
	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	void DiffuseInk(); //TODO implement

private:
	// The array of dynamic materials
	TArray<class UMaterialInstanceDynamic*> dynamicMaterials;

	// The update region of the dynamic texture. Needed for the UpdateTextureRegions function
	FUpdateTextureRegion2D* updateTextureRegion;

	// The dynamic texture that is being painted on, acting as the canvas
	UTexture2D *dynamicTexture;

	// The color matrix that is being painted on the canvas texture
	uint8* dynamicColors;
	
	// The size of the texture. Derived from the CanvasResolution and the shape of the canvas mesh
	int32 canvasTextureWidth, canvasTextureHeight;

	// A bitset that holds an optimized version of the canvas data, used for efficient counting of the score
	TBitArray<FDefaultBitArrayAllocator> scoreBitset;

	// An ongoing count of the score of each player
	TArray<uint32> scoreCounts;

	// The painting mask bitset and its dimensions
	int32 maskTextureWidth, maskTextureHeight;
	TBitArray<FDefaultBitArrayAllocator> maskBitset;

	// The maximum score that can be achieved with the active painting
	uint32 maxScore;
};
