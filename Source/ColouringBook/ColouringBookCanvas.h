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
};
