// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "ColouringBook.h"
#include "ColouringBookGameMode.h"
#include "ColouringBookCharacter.h"

AColouringBookGameMode::AColouringBookGameMode()
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
	int numPlayers = GetMaxNumPlayers(); // FIXME: This number should problably come from UI, unless we are using a players pop in/out feature
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

	while (playerIndex < numPlayers && playerIndex < MAX_PLAYERS)
	{
		APlayerController* player = UGameplayStatics::CreatePlayer(GetWorld());
		Cast<AColouringBookCharacter>(player->GetPawn())->SetPlayerID(playerIndex);
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

