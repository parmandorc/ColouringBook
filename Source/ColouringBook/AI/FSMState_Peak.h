// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AI/DirectorFSMState.h"
#include "FSMState_Peak.generated.h"

/**
 * 
 */
UCLASS()
class COLOURINGBOOK_API UFSMState_Peak : public UDirectorFSMState
{
	GENERATED_BODY()
	
public:
	UFSMState_Peak();
	
	// FSMState interface
	virtual void OnEnter() override;
	virtual State CheckForTransitions() override;

private:
	// The number of enemies that were alive in the scene when the FSM entered this state
	int initialNumEnemies;
};
