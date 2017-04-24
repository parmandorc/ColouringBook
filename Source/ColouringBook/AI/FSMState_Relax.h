// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AI/DirectorFSMState.h"
#include "FSMState_Relax.generated.h"

/**
 * 
 */
UCLASS()
class COLOURINGBOOK_API UFSMState_Relax : public UDirectorFSMState
{
	GENERATED_BODY()
	
public:
	UFSMState_Relax();
	
	// FSMState interface
	virtual void OnEnter() override;
	virtual State CheckForTransitions() override;

private:
	// The handle for the timer
	FTimerHandle timerHandle;

	// Whether the relax minimum timer has elapsed
	bool isTimePassed;

	// Called when the minimum timer elaspes
	void OnTimerElapse();
};
