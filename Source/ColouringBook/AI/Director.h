// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "DirectorFSMState.h"
#include "Director.generated.h"

UCLASS(Blueprintable)
class COLOURINGBOOK_API ADirector : public AActor
{
	GENERATED_BODY()
	
	// The class to use for spawning enemies in the scene
	UPROPERTY(Category = Gameplay, EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<ACharacter> EnemyBP;
	
public:	
	// Sets default values for this actor's properties
	ADirector();

	// AActor interface
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	//allows custom timing for spawning enemies
	void SpawnEnemy();

private:
	// The list of states that make up the Finite State Machine
	TArray<UDirectorFSMState*> FSMStates;

	// The current active state of the FSM
	UDirectorFSMState::State currentState;

	// Transitions the FSM to a new state
	void FSMTransitionTo(UDirectorFSMState::State newState);

	//helper to generate random position for enemies to spawn
	FVector GetRandomCirclePosition(FVector center, float radius);
};
