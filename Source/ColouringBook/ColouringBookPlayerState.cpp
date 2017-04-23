// Fill out your copyright notice in the Description page of Project Settings.

#include "ColouringBook.h"
#include "ColouringBookPlayerState.h"

#include "Net/UnrealNetwork.h"

AColouringBookPlayerState::AColouringBookPlayerState(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	color = FColor::Black;
}

void AColouringBookPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AColouringBookPlayerState, color);
}
