// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.
#pragma once

#include "ColouringBookGameState.h"

#include "GameFramework/GameModeBase.h"
#include "ColouringBookGameMode.generated.h"


UCLASS(minimalapi)
class AColouringBookGameMode : public AGameModeBase
{
	GENERATED_BODY()

	/* The colors that identify each player */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Gameplay, meta = (AllowPrivateAccess = "true"))
	TArray<FColor> playerColors;

public:
	AColouringBookGameMode();

	virtual void StartPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual void Tick(float DeltaSeconds) override;

	virtual void InitGameState() override;
	virtual bool ShouldSpawnAtStartSpot(AController* Player) override { return false; }
	virtual void PostLogin(APlayerController* NewPlayer) override;
	
	/* Returns the color that is associated with a player ID */
	FORCEINLINE FColor GetPlayerColor(uint8 playerIndex) const { return (playerIndex < playerColors.Num()) ? playerColors[playerIndex] : FColor::Black; }

	/* Returns the maximum number of players in the game */
	FORCEINLINE int32 GetMaxNumPlayers() const { return playerColors.Num(); }

	enum class MultiplayerMode
	{
		UNDEFINED,

		LOCAL,
		ONLINE,

		NUM_MULTIPLAYER_MODES
	};

	MultiplayerMode GetMultiplayerMode() { return multiplayerMode; }

protected:

	void SetServerState(EServerStateEnum newState);
	void SetHUD();

	void LocalMultiplayerCreatePlayers();

	void SpawnNewPlayer(APlayerController* player);

	AActor* GetPlayerStart(AController* Player);
	FColor GetPlayerColor(AController* Player);

	void UpdateState(float deltaSeconds);

	bool InPlayingMap();

private:

	TArray<AActor*> playerStarts;

	MultiplayerMode multiplayerMode;

	static EServerStateEnum s_serverState;
};



