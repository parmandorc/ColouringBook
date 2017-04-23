// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/PlayerState.h"
#include "ColouringBookPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class COLOURINGBOOK_API AColouringBookPlayerState : public APlayerState
{
	GENERATED_BODY()
	
public:

	AColouringBookPlayerState(const FObjectInitializer& ObjectInitializer);
	
	UPROPERTY(Replicated)
	FColor color;

	

};
