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
	

protected:

	void CreatePlayers(int numPlayers);

	TArray<AActor*> playerStarts;
};



