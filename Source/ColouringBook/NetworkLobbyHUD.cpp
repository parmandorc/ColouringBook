// Fill out your copyright notice in the Description page of Project Settings.

#include "ColouringBook.h"
#include "NetworkLobbyHUD.h"

#include "ColouringBookGameMode.h"

void ANetworkLobbyHUD::DrawHUD()
{
	Super::DrawHUD();
	
	bool isServer = (GetWorld()->GetAuthGameMode() != nullptr);

	// draw debug info
	FString hudDebugInfo = isServer ? "Server NetworkLobbyHUD - " : "Client NetworkLobbyHUD - ";
	FString thisPlayerName;

	APlayerController* playerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (playerController)
	{
		thisPlayerName = playerController->GetLocalPlayer()->GetName();
	}

	hudDebugInfo.Append(thisPlayerName);
	DrawText(hudDebugInfo, FColor::Cyan, 0, 0);

	// draw status info
	FString statusInfo = isServer ? "Press S to start" : "Waiting for server";
	DrawText(statusInfo, isServer ? FColor::White : FColor::Red, Canvas->SizeX / 2, Canvas->SizeY / 2);

	// draw players that are connected
	
	AGameStateBase* gameState = GetWorld()->GetGameState();
	if (gameState)
	{
		for (int i = 0; i < gameState->PlayerArray.Num(); i++)
		{
			APlayerState* playerState = gameState->PlayerArray[i];

			FString readyPlayer = playerState->GetName() + " ready!";
			DrawText(readyPlayer, FColor::Green, 50, 50 + i*20);
		}
	}
}

