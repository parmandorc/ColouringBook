// Fill out your copyright notice in the Description page of Project Settings.

#include "ColouringBook.h"
#include "Director.h"

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

//TODO make enemies not spawn near player
void ADirector::SpawnEnemy()
{
	//Handle spawning of enemies
	UWorld* const World = GetWorld();
	if (World != nullptr)
	{
		FVector PuppetLocation = GetRandomCirclePosition(GetActorLocation(), SpawnRadius);
		ACharacter* SpawnedPuppet = World->SpawnActor<ACharacter>(EnemyBP, PuppetLocation, (GetActorLocation() - PuppetLocation).ToOrientationRotator());
	}
}

void ADirector::FSMTransitionTo(UDirectorFSMState::State newState)
{
	FSMStates[currentState]->OnExit();
	currentState = newState;
	FSMStates[currentState]->OnEnter();
}