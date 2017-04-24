// Fill out your copyright notice in the Description page of Project Settings.

#include "ColouringBook.h"
#include "Director.h"
#include "AI/PuppetEnemyCharacter.h"

// Sets default values
ADirector::ADirector()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Default values
	SpawnRadius = 1200.0f;
	MinSpawnTime = 1.0f;
	MaxSpawnTime = 3.0f;
	BuildUpTimeForMaxSpawnRate = 15.0f;
	PeakEnemiesPercentage = 1.0f;
	RelaxMinTime = 5.0f;
}

// Called when the game starts or when spawned
void ADirector::BeginPlay()
{
	Super::BeginPlay();

	// Initialize FSM
	FSMStates = UDirectorFSMState::CreateFSM(this);
	currentState = UDirectorFSMState::State::BUILD_UP;
	FSMStates[currentState]->OnEnter();
}

// Called every frame
void ADirector::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Check if the FSM should transition to another state
	UDirectorFSMState::State stateToTransition = FSMStates[currentState]->CheckForTransitions();

	if (stateToTransition == UDirectorFSMState::State::NONE)
	{
		// Call the update function for the current state
		FSMStates[currentState]->Tick(DeltaTime);
	}
	else
	{
		FSMTransitionTo(stateToTransition);
	}
}

FVector ADirector::GetRandomCirclePosition(FVector center, float radius)
{
	float angle = FMath::RandRange(0.0f, 360.0f);

	FVector position;
	position.X = center.X + radius*FMath::Sin(angle*FMath::DegreesToRadians(angle));
	position.Y = center.Y + radius*FMath::Cos(angle*FMath::DegreesToRadians(angle));
	position.Z = center.Z;

	return position;
}

void ADirector::SpawnEnemy()
{
	if (Role != ROLE_Authority)
	{
		return;
	}

	//Handle spawning of enemies
	UWorld* const World = GetWorld();
	if (World != nullptr)
	{
		FVector PuppetLocation = GetRandomCirclePosition(GetActorLocation(), SpawnRadius);
		APuppetEnemyCharacter* SpawnedPuppet = World->SpawnActor<APuppetEnemyCharacter>(EnemyBP, PuppetLocation, (GetActorLocation() - PuppetLocation).ToOrientationRotator());

		// Register the newly spawned enemy
		if (SpawnedPuppet != nullptr)
		{
			SpawnedPuppet->SetDirector(this);
			spawnedEnemies.Add(SpawnedPuppet);
		}
	}
}

void ADirector::FSMTransitionTo(UDirectorFSMState::State newState)
{
	UE_LOG(LogTemp, Log, TEXT("FSM transition to state: %d"), static_cast<int>(newState));
	FSMStates[currentState]->OnExit();
	currentState = newState;
	FSMStates[currentState]->OnEnter();
}

void ADirector::OnEnemyDeath(AActor* enemy)
{
	spawnedEnemies.Remove(enemy);
}