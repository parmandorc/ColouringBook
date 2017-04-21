// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "ColouringBook.h"
#include "ColouringBookGameMode.h"
#include "ColouringBookCharacter.h"
#include "ColouringBookPlayerController.h"

AColouringBookGameMode::AColouringBookGameMode()
	: multiplayerMode(MultiplayerMode::UNDEFINED)
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/Blueprints/ColouringBookCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}

	// set default player controller to our Blueprinted PlayerController
	static ConstructorHelpers::FClassFinder<AColouringBookPlayerController> PlayerControllerBPClass(TEXT("/Game/Blueprints/ColouringBookPlayerController"));
	if (PlayerControllerBPClass.Class != NULL)
	{
		PlayerControllerClass = PlayerControllerBPClass.Class;
	}
	else
	{
		PlayerControllerClass = AColouringBookPlayerController::StaticClass();
	}
	
	// make sure it replicates
	bReplicates = true;
}

void AColouringBookGameMode::InitGameState()
{
	AGameModeBase::InitGameState();

	//  Get all PlayerStart
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerStart::StaticClass(), playerStarts);
}

void AColouringBookGameMode::StartPlay()
{
	AGameModeBase::StartPlay();

	if (Role == ROLE_Authority)
	{
		multiplayerMode = MultiplayerMode::ONLINE; // FIXME: This should probably come from UI

		switch (multiplayerMode)
		{
		case MultiplayerMode::LOCAL:
			LocalMultiplayerCreatePlayers();
			break;
		case MultiplayerMode::ONLINE:
			// TO-DO: ServerTravel ?
			break;
		default:
			break;
		}
	}
}

void AColouringBookGameMode::PostLogin(APlayerController* NewPlayer)
{
	// spawn new player
	AGameModeBase::PostLogin(NewPlayer);
	SpawnNewPlayer(NewPlayer);
}

void AColouringBookGameMode::SpawnNewPlayer(APlayerController* player)
{
	if (Role != ROLE_Authority)
	{
		return;
	}

	// Make sure that the player is spawned at the correct PlayerStart
	AActor* playerStart = GetPlayerStart(player);
	if (playerStart)
	{
		FVector location = playerStart->GetActorLocation();

		AColouringBookCharacter* character = Cast<AColouringBookCharacter>(player->GetCharacter());
		character->SetActorLocation(location);
	}
}

AActor* AColouringBookGameMode::GetPlayerStart(AController* Player)
{
	// PlayerStart is choosen by name(not ideal if the name changes in the editor...) according to the number of players created so far

	int32 numPlayers = GetNumPlayers();
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

void AColouringBookGameMode::LocalMultiplayerCreatePlayers()
{
	// TO-DO: Investigate how to surpass MaxSplitscreenPlayers constraint because it is the one that doesn´t allow to create 
	// more than 4 players (regardless of using split screen or not)
	const int MAX_PLAYERS = GetWorld()->GetGameInstance()->GetGameViewportClient()->MaxSplitscreenPlayers;

	// somehow/somewhere the first player is always created...
	// TO-DO: Investigate if we are able to control first player creation at this point
	int32 playerIndex = GetWorld()->GetGameInstance()->GetNumLocalPlayers();

	while (playerIndex < GetMaxNumPlayers() && playerIndex < MAX_PLAYERS)
	{
		UGameplayStatics::CreatePlayer(GetWorld());
		playerIndex++;
	}
}
