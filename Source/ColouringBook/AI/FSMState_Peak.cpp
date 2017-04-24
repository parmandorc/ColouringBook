// Fill out your copyright notice in the Description page of Project Settings.

#include "ColouringBook.h"
#include "FSMState_Peak.h"
#include "Director.h"

UFSMState_Peak::UFSMState_Peak()
{

}

void UFSMState_Peak::OnEnter()
{
	initialNumEnemies = director->GetSpawnedEnemiesNum();
}

UDirectorFSMState::State UFSMState_Peak::CheckForTransitions()
{
	// Check if the necesarry amount of enemies have died
	if (initialNumEnemies == 0 || 1.0f - (float)director->GetSpawnedEnemiesNum() / initialNumEnemies >= director->GetPeakEnemiesPercentage())
	{
		return State::BUILD_UP;
	}

	return State::NONE;
}