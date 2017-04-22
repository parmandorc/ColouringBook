// Fill out your copyright notice in the Description page of Project Settings.

#include "ColouringBook.h"
#include "NetworkLobbyHUD.h"

#include "ColouringBookGameMode.h"

void ANetworkLobbyHUD::DrawHUD()
{
	Super::DrawHUD();

	FString text = "NetworkLobbyHUD";

	if (GetWorld()->GetAuthGameMode())
	{
		text = "Server NetworkLobbyHUD";
	}
	else
	{
		text = "Client NetworkLobbyHUD";
	}

	DrawText(text, FColor::Green, 50, 50);
}

