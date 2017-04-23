// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/GameStateBase.h"
#include "ColouringBookGameState.generated.h"

UENUM(BlueprintType)
enum class EServerStateEnum : uint8
{
	UNDEFINED				UMETA(DisplayName = "Undefined"),
	WAITING_FOR_TRAVELLING	UMETA(DisplayName = "WaitingForTravelling"),
	TRAVELLING				UMETA(DisplayName = "Travelling"),
	PLAYING					UMETA(DisplayName = "Playing")
};

/**
 * 
 */
UCLASS()
class COLOURINGBOOK_API AColouringBookGameState : public AGameStateBase
{
	GENERATED_BODY()
	
public:

	AColouringBookGameState();

	UPROPERTY(Replicated)
	TEnumAsByte<EServerStateEnum> serverState;

public:

	FColor GetPlayerColor(uint32 playerId);
};
