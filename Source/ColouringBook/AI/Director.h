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

	// The radius of the circle where to spawn the enemies
	UPROPERTY(Category = Gameplay, EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true", UIMin = "0.0"))
	float SpawnRadius;

	// The minimum amount of time between enemy spawns
	UPROPERTY(Category = Gameplay, EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true", UIMin = "0.0"))
	float MinSpawnTime;

	// The maximum amount of time between enemy spawns
	UPROPERTY(Category = Gameplay, EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true", UIMin = "0.0"))
	float MaxSpawnTime;

	// The amount of time the director takes to reach maximum spawn rate during build up
	UPROPERTY(Category = Gameplay, EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true", UIMin = "0.0"))
	float BuildUpTimeForMaxSpawnRate;
	
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
	UPROPERTY()
	TArray<UDirectorFSMState*> FSMStates;

	// The current active state of the FSM
	UDirectorFSMState::State currentState;

	// Transitions the FSM to a new state
	void FSMTransitionTo(UDirectorFSMState::State newState);

	//helper to generate random position for enemies to spawn
	FVector GetRandomCirclePosition(FVector center, float radius);

public:
	// Returns the minimum amount of time between enemy spawns
	FORCEINLINE float GetMinSpawnTime() { return MinSpawnTime; }
	// Returns the maximum amount of time between enemy spawns
	FORCEINLINE float GetMaxSpawnTime() { return MaxSpawnTime; }
	// Returns the amount of time the director takes to reach maximum spawn rate during build up
	FORCEINLINE float GetBuildUpTimeForMaxSpawnRate() { return BuildUpTimeForMaxSpawnRate; }
};
