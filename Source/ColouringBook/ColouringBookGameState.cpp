// Fill out your copyright notice in the Description page of Project Settings.

#include "ColouringBook.h"
#include "ColouringBookGameState.h"
#include "ColouringBookPlayerState.h"

#include "Net/UnrealNetwork.h"

AColouringBookGameState::AColouringBookGameState()
{
	serverState = EServerStateEnum::UNDEFINED;
	maxNumPlayers = 0;
}

void AColouringBookGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AColouringBookGameState, serverState);
	DOREPLIFETIME(AColouringBookGameState, maxNumPlayers);
}

FColor AColouringBookGameState::GetPlayerColor(uint32 playerId)
{
	FColor color = FColor::Black;

	for (auto player : PlayerArray)
	{
		if (player->PlayerId == playerId)
		{
			AColouringBookPlayerState* playerState = Cast<AColouringBookPlayerState>(player);
			color = playerState->color;
			break;
		}
	}

	return color;
}

int8 AColouringBookGameState::GetPlayerIndex(uint32 playerId)
{
	int8 index = -1;
	int8 currentIndex = 0;
	for (auto player : PlayerArray)
	{
		if (player->PlayerId == playerId)
		{
			index = currentIndex;
			break;
		}
		currentIndex++;
	}

	return index;
}


