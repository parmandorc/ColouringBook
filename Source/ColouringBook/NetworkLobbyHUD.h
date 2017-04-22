// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/HUD.h"
#include "NetworkLobbyHUD.generated.h"

/**
 * 
 */
UCLASS()
class COLOURINGBOOK_API ANetworkLobbyHUD : public AHUD
{
	GENERATED_BODY()
	
public:

	virtual void DrawHUD() override;
	
};
