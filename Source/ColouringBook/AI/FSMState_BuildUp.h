// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AI/DirectorFSMState.h"
#include "FSMState_BuildUp.generated.h"

/**
 * 
 */
UCLASS()
class COLOURINGBOOK_API UFSMState_BuildUp : public UDirectorFSMState
{
	GENERATED_BODY()

public:
	UFSMState_BuildUp();

	// FSMState interface
	virtual void OnEnter() override;
	virtual void Tick(float DeltaTime) override;
	virtual void OnExit() override;
	virtual State CheckForTransitions() override;

private:
	// The timer handle for spawning enemies
	FTimerHandle SpawnTimerHandle;

	// Sets the timer for calling the director to spawn a new enemy
	void SetSpawnTimer();

	// Function that is called when the spawning timer elapses
	void SpawnEnemy();

	// The time that this state has been active
	float lifetime;
};
