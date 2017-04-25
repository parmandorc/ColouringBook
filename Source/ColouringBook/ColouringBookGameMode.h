// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.
#pragma once

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

	virtual void StartPlay();

	virtual void InitGameState();
	virtual bool ShouldSpawnAtStartSpot(AController* Player) { return false; }
	virtual AActor* ChoosePlayerStart_Implementation(AController* Player);

protected:

	void CreatePlayers(int numPlayers);

	TArray<AActor*> playerStarts;

	UPROPERTY()
	float RunTime;

public:
	/* Returns the color that is associated with a player ID */
	FORCEINLINE FColor GetPlayerColor(uint8 playerIndex) const { return (playerIndex < playerColors.Num()) ? playerColors[playerIndex] : FColor::White; }

	/* Returns the maximum number of players in the game */
	FORCEINLINE int32 GetMaxNumPlayers() const { return playerColors.Num(); }

	float GetRunTime();

	UFUNCTION()
	virtual void Tick(float DeltaSeconds) override;
};



