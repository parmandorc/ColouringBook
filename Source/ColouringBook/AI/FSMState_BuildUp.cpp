// Fill out your copyright notice in the Description page of Project Settings.

#include "ColouringBook.h"
#include "FSMState_BuildUp.h"
#include "Director.h"

UFSMState_BuildUp::UFSMState_BuildUp()
{
	
}

void UFSMState_BuildUp::OnEnter()
{
	SetSpawnTimer();
}

void UFSMState_BuildUp::OnExit()
{
	UWorld* world = GetWorld();
	if (world != nullptr)
	{
		world->GetTimerManager().ClearTimer(SpawnTimerHandle);
	}
}

void UFSMState_BuildUp::SetSpawnTimer()
{
	UWorld* world = GetOuter()->GetWorld();
	if (world != nullptr)
	{
		float timerRate = FMath::FRandRange(director->GetMinSpawnTime(), director->GetMaxSpawnTime());
		world->GetTimerManager().SetTimer(SpawnTimerHandle, this, &UFSMState_BuildUp::SpawnEnemy, timerRate, false);
	}
}

void UFSMState_BuildUp::SpawnEnemy()
{
	director->SpawnEnemy();

	SetSpawnTimer();
}