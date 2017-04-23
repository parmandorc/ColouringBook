// Fill out your copyright notice in the Description page of Project Settings.

#include "ColouringBook.h"
#include "ColouringBookGameState.h"
#include "ColouringBookPlayerState.h"

#include "Net/UnrealNetwork.h"

AColouringBookGameState::AColouringBookGameState()
{
	serverState = EServerStateEnum::UNDEFINED;
}

void AColouringBookGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AColouringBookGameState, serverState);
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


