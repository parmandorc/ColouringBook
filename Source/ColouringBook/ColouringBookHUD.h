// Fill out your copyright notice in the Description page of Project Settings.
#pragma once
#include "ColouringBookCanvas.h"
#include "GameFramework/HUD.h"
#include "ColouringBookHUD.generated.h"


/**
 * 
 */
UCLASS()
class COLOURINGBOOK_API AColouringBookHUD : public AHUD
{
	GENERATED_BODY()
	AColouringBookHUD();

	UPROPERTY()
	UFont* HUDFont;

	virtual void DrawHUD() override;
	
public:
	UPROPERTY(Category = Gameplay, BlueprintReadWrite, EditAnywhere)
	AColouringBookCanvas* CanvasInstance;
};
