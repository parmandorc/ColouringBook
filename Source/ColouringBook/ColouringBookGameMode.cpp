// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "ColouringBook.h"
#include "ColouringBookGameMode.h"
#include "ColouringBookCharacter.h"

AColouringBookGameMode::AColouringBookGameMode()
	: multiplayerMode(MultiplayerMode::UNDEFINED)
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/Blueprints/ColouringBookCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}

void AColouringBookGameMode::InitGameState()
{
	AGameModeBase::InitGameState();

	//  Get all PlayerStart
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerStart::StaticClass(), playerStarts);
}

void AColouringBookGameMode::StartPlay()
{
	multiplayerMode = MultiplayerMode::ONLINE; // FIXME: This should probably come from UI

	switch (multiplayerMode)
	{
	case MultiplayerMode::LOCAL:
		StartLocalMultiplayerPlay();
		break;
	case MultiplayerMode::ONLINE:
		StartOnlineMultiplayerPlay();
		break;
	default:
		break;
	}

	AGameModeBase::StartPlay();
}

void AColouringBookGameMode::StartOnlineMultiplayerPlay()
{
	// TO-DO
}

void AColouringBookGameMode::StartLocalMultiplayerPlay()
{
	int numPlayers = GetMaxNumPlayers();
	LocalMultiplayerCreatePlayers(numPlayers);
}

void AColouringBookGameMode::LocalMultiplayerCreatePlayers(int numPlayers)
{
	// TO-DO: Investigate how to surpass MaxSplitscreenPlayers constraint because it is the one that doesn´t allow to create 
	// more than 4 players (regardless of using split screen or not)
	const int MAX_PLAYERS = GetWorld()->GetGameInstance()->GetGameViewportClient()->MaxSplitscreenPlayers;

	// somehow/somewhere the first player is always created...
	// TO-DO: Investigate if we are able to control first player creation at this point
	int32 playerIndex = GetWorld()->GetGameInstance()->GetNumLocalPlayers();

	while (playerIndex < numPlayers && playerIndex < MAX_PLAYERS)
	{
		UGameplayStatics::CreatePlayer(GetWorld());
		playerIndex++;
	}
}

AActor* AColouringBookGameMode::ChoosePlayerStart_Implementation(AController* Player)
{
	// Apparently there is no direct relationship between Player and ControllerId
	// and PlayerStart and ControllerId at this point?
	// Therefore, PlayerStart is choose by name(not ideal if the name changes in the editor...) according to the number of players created so far
	int32 numPlayers = GetWorld()->GetGameInstance()->GetNumLocalPlayers();
	FString playerName = "PlayerStart";
	playerName.AppendInt(numPlayers);
	
	for (AActor* actor : playerStarts)
	{
		if (actor->GetName() == playerName)
		{
			return actor;
		}
	}

	return nullptr;
}

