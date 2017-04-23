// Fill out your copyright notice in the Description page of Project Settings.

#include "ColouringBook.h"
#include "ColouringBookGameState.h"

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



