// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/HUD.h"
#include "ColouringBook_HUD.generated.h"

/**
 * 
 */
UCLASS()
class COLOURINGBOOK_API AColouringBook_HUD : public AHUD
{
	GENERATED_BODY()
		AColouringBook_HUD();
	
	UPROPERTY()
		UFont* HUDFont;

	virtual void DrawHUD() override;
	
	
};
