// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

class ADirector; // Forward declaration

#include "UObject/NoExportTypes.h"
#include "DirectorFSMState.generated.h"

/**
 * 
 */
UCLASS()
class COLOURINGBOOK_API UDirectorFSMState : public UObject
{
	GENERATED_BODY()

public:
	static enum State {
		BUILD_UP,
		PEAK,
		RELAX,
		NONE
	};

	static TArray<UDirectorFSMState*> CreateFSM(ADirector* director);

private:
	// Sets the reference to the director
	UDirectorFSMState* SetDirector(ADirector* _director) { director = _director; return this; }

protected:
	// A reference to the director that handles the FSM
	ADirector* director;

public:
	UDirectorFSMState();

	// Called when transitioning to this state
	virtual void OnEnter() {}

	// Called every frame while the FSM in this state
	virtual void Tick(float DeltaTime) {}

	// Called when transitioning out of this state
	virtual void OnExit() {}
	
	// Returns whether the FSM should transition to a different state
	virtual State CheckForTransitions() { return State::NONE; }

	// Returns the name of the state
	FORCEINLINE virtual FName GetStateName() const { return FName(TEXT("")); }
};
