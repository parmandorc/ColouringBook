// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "ColouringBook.h"
#include "ColouringBookGameMode.h"
#include "ColouringBookPawn.h"

AColouringBookGameMode::AColouringBookGameMode()
{
	// set default pawn class to our character class
	DefaultPawnClass = AColouringBookPawn::StaticClass();
}

void AColouringBookGameMode::StartPlay()
{
	int numPlayers = 4; // FIXME: This number should problably come from UI, unless we are using a players pop in/out feature
	CreatePlayers(numPlayers);

	AGameModeBase::StartPlay();
}

void AColouringBookGameMode::CreatePlayers(int numPlayers)
{
	// TO-DO: Investigate how to surpass MaxSplitscreenPlayers constraint because it is the one that doesn´t allow to create 
	// more than 4 players (regardless of using split screen or not)
	const int MAX_PLAYERS = GetWorld()->GetGameInstance()->GetGameViewportClient()->MaxSplitscreenPlayers;

	// somehow/somewhere the first player is always created...
	// TO-DO: Investigate if we are able to control first player creation at this point
	int32 playerIndex = GetWorld()->GetGameInstance()->GetNumLocalPlayers();

	int numPlayersCreated = playerIndex;

	while (playerIndex < numPlayers && playerIndex < MAX_PLAYERS)
	{
		if (UGameplayStatics::CreatePlayer(GetWorld()))
		{
			numPlayersCreated++;
		}

		playerIndex++;
	}

	// print debug message
	FString debugMessage;
	debugMessage.AppendInt(numPlayersCreated);
	debugMessage.Append(" Players created");
	GEngine->AddOnScreenDebugMessage(INDEX_NONE, 5.0f, FColor::Green, debugMessage);
}

