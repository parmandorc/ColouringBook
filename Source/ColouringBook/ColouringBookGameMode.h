// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.
#pragma once

#include "GameFramework/GameModeBase.h"
#include "ColouringBookGameMode.generated.h"


UCLASS(minimalapi)
class AColouringBookGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AColouringBookGameMode();

	virtual void StartPlay();

	virtual void InitGameState();
	virtual bool ShouldSpawnAtStartSpot(AController* Player) { return false; }
	virtual AActor* ChoosePlayerStart_Implementation(AController* Player);
	
	enum class MultiplayerMode
	{
		UNDEFINED,

		LOCAL,
		ONLINE,

		NUM_MULTIPLAYER_MODES
	};

	MultiplayerMode GetMultiplayerMode() { return multiplayerMode; }

protected:

	void StartLocalMultiplayerPlay();
	void StartOnlineMultiplayerPlay();

	void LocalMultiplayerCreatePlayers(int numPlayers);

private:

	TArray<AActor*> playerStarts;

	MultiplayerMode multiplayerMode;
};



