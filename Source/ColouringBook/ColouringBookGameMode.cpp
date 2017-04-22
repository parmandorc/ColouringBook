// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "ColouringBook.h"
#include "ColouringBookGameMode.h"
#include "ColouringBookCharacter.h"
#include "ColouringBookPlayerController.h"

#include "NetworkLobbyHUD.h"


AColouringBookGameMode::ColouringBookGameState AColouringBookGameMode::s_state = AColouringBookGameMode::ColouringBookGameState::UNDEFINED;

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

	// make sure it ticks
	PrimaryActorTick.bStartWithTickEnabled = true;
	PrimaryActorTick.bCanEverTick = true;
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

	multiplayerMode = MultiplayerMode::ONLINE; // FIXME: This should probably come from UI

	switch (multiplayerMode)
	{
	case MultiplayerMode::LOCAL:
		LocalMultiplayerCreatePlayers();
		break;
	case MultiplayerMode::ONLINE:
		if (s_state == ColouringBookGameState::UNDEFINED)
		{
			if (InPlayingMap())
			{
				// Hack: Do not wait for server start as the map is already the playing map
				SetNewState(ColouringBookGameState::PLAYING);
			}
			else
			{
				SetNewState(ColouringBookGameState::WAITING_FOR_TRAVELLING);
			}
		}

		break;
	default:
		break;
	}
}

void AColouringBookGameMode::SetNewState(ColouringBookGameState newState)
{
	if (s_state != newState)
	{
		s_state = newState;

		// set new HUD
		SetHUD();
	}
}

void AColouringBookGameMode::SetHUD()
{
	switch (s_state)
	{
	case ColouringBookGameState::WAITING_FOR_TRAVELLING:
		HUDClass = ANetworkLobbyHUD::StaticClass();
		break;
	default:
		break;
	}

	// Set server HUD
	GetWorld()->GetFirstPlayerController()->ClientSetHUD(HUDClass);

	// TO-DO: The client should be informed about the new HUD
}

void AColouringBookGameMode::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	// update current state
	UpdateState(DeltaSeconds);
}

void AColouringBookGameMode::UpdateState(float deltaSeconds)
{
	switch (s_state)
	{
	case ColouringBookGameState::WAITING_FOR_TRAVELLING:
	{
		// Check for travelling
		APlayerController* serverPlayerController = GetWorld()->GetFirstPlayerController();
		if (serverPlayerController && serverPlayerController->IsInputKeyDown(EKeys::S))
		{
			SetNewState(ColouringBookGameState::SERVER_TRAVELLING);

			FString map = TEXT("/Game/Maps/TwinStickExampleMap");
			map.Append("?Listen"); // make sure that server will accept connecting clients in the travelled map
			GetWorld()->ServerTravel(map);
		}
		break;
	}
	case ColouringBookGameState::SERVER_TRAVELLING:
	{
		// TO-DO: Handle travelling failure
		bool travellingSucceeded = InPlayingMap();
		if (travellingSucceeded)
		{
			SetNewState(ColouringBookGameState::PLAYING);
		}

		break;
	}
	case ColouringBookGameState::PLAYING:
		break;
	default:
		break;
	}
}

bool AColouringBookGameMode::InPlayingMap()
{
	FString mapName = GetWorld()->GetMapName();
	return (mapName.Contains("TwinStickExampleMap"));
}

void AColouringBookGameMode::PostLogin(APlayerController* NewPlayer)
{
	// spawn new player
	AGameModeBase::PostLogin(NewPlayer);
	SpawnNewPlayer(NewPlayer);
}

void AColouringBookGameMode::SpawnNewPlayer(APlayerController* player)
{
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
