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

	// The percentage of enemies that have to die to transition out of the Peak state
	UPROPERTY(Category = Gameplay, EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true", UIMin = "0.0", UIMax = "1.0"))
	float PeakEnemiesPercentage;

	// The minimum time the director stays in Relax mode
	UPROPERTY(Category = Gameplay, EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true", UIMin = "0.0"))
	float RelaxMinTime;
	
public:	
	// Sets default values for this actor's properties
	ADirector();

	// AActor interface
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	//allows custom timing for spawning enemies
	void SpawnEnemy();

	// Call back event when an enemy spawned by this director dies
	void OnEnemyDeath(AActor* enemy);

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

	// The number of enemies that where spawned by this director
	TArray<AActor*> spawnedEnemies;

public:
	// Returns the minimum amount of time between enemy spawns
	FORCEINLINE float GetMinSpawnTime() const { return MinSpawnTime; }
	// Returns the maximum amount of time between enemy spawns
	FORCEINLINE float GetMaxSpawnTime() const { return MaxSpawnTime; }
	// Returns the amount of time the director takes to reach maximum spawn rate during build up
	FORCEINLINE float GetBuildUpTimeForMaxSpawnRate() const { return BuildUpTimeForMaxSpawnRate; }
	// Returns the amount of enemies that are currently alive in the scene
	FORCEINLINE int GetSpawnedEnemiesNum() const { return spawnedEnemies.Num(); }
	// Return he percentage of enemies that have to die to transition out of the Peak state
	FORCEINLINE float GetPeakEnemiesPercentage() const { return PeakEnemiesPercentage; }
	// Returns the minimum time the director stays in Relax mode
	FORCEINLINE float GetRelaxMinTime() const { return RelaxMinTime; }
};
