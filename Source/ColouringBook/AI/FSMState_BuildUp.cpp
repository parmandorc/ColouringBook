// Fill out your copyright notice in the Description page of Project Settings.

#include "ColouringBook.h"
#include "FSMState_BuildUp.h"
#include "Director.h"
#include "EngineUtils.h"
#include "ColouringBookCharacter.h"
#include "IntensityTracker.h"

UFSMState_BuildUp::UFSMState_BuildUp()
{
	
}

void UFSMState_BuildUp::OnEnter()
{
	lifetime = 0.0f;

	SetSpawnTimer();
}

void UFSMState_BuildUp::Tick(float DeltaTime)
{
	lifetime += DeltaTime;
}

void UFSMState_BuildUp::OnExit()
{
	UWorld* world = GetOuter()->GetWorld();
	if (world != nullptr)
	{
		world->GetTimerManager().ClearTimer(SpawnTimerHandle);
	}
}

UDirectorFSMState::State UFSMState_BuildUp::CheckForTransitions()
{
	// Check to see if any player has reached max intensity
	for (TActorIterator<AColouringBookCharacter> PlayerItr(GetOuter()->GetWorld()); PlayerItr; ++PlayerItr) 
	{
		UIntensityTracker* intensityTracker = (*PlayerItr)->GetIntensityTracker();
		if (intensityTracker->GetIntensity() >= 1.0f)
		{
			return UDirectorFSMState::State::BUILD_UP; // Reset State (later, it will be a transition to a different state)
		}
	}

	return UDirectorFSMState::State::NONE;
}

void UFSMState_BuildUp::SetSpawnTimer()
{
	UWorld* world = GetOuter()->GetWorld();
	if (world != nullptr)
	{
		float t = FMath::Min(lifetime / director->GetBuildUpTimeForMaxSpawnRate(), 1.0f);
		float maxRate = FMath::Lerp(director->GetMinSpawnTime(), director->GetMaxSpawnTime(), 1 - t);
		float timerRate = FMath::FRandRange(director->GetMinSpawnTime(), maxRate);
		world->GetTimerManager().SetTimer(SpawnTimerHandle, this, &UFSMState_BuildUp::SpawnEnemy, timerRate, false);
	}
}

void UFSMState_BuildUp::SpawnEnemy()
{
	director->SpawnEnemy();

	SetSpawnTimer();
}