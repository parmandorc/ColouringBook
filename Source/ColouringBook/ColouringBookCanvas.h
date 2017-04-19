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

public:
	AColouringBookCanvas();

	// Begin Actor Interface
	virtual void Tick(float DeltaSeconds) override;
	virtual void PostInitializeComponents() override;
	// End Actor Interface

	// Function to handle the canvas being hit by something
	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

private:
	// The array of dynamic materials
	TArray<class UMaterialInstanceDynamic*> dynamicMaterials;

	// The update region of the dynamic texture. Needed for the UpdateTextureRegions function
	FUpdateTextureRegion2D* updateTextureRegion;

	// The dynamic texture that is being painted on, acting as the canvas
	UPROPERTY()
	UTexture2D *dynamicTexture;

	// The color matrix that is being painted on the canvas texture
	uint8* dynamicColors;
	
	// The size of the texture. Derived from the CanvasResolution and the shape of the canvas mesh
	int32 canvasTextureWidth, canvasTextureHeight;
};
