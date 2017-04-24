// Fill out your copyright notice in the Description page of Project Settings.

#include "ColouringBook.h"
#include "FSMState_Relax.h"
#include "Director.h"
#include "ColouringBookCharacter.h"
#include "IntensityTracker.h"

UFSMState_Relax::UFSMState_Relax()
{

}

void UFSMState_Relax::OnEnter()
{
	isTimePassed = false;

	UWorld* world = GetOuter()->GetWorld();
	if (world != nullptr)
	{
		world->GetTimerManager().SetTimer(timerHandle, this, &UFSMState_Relax::OnTimerElapse, director->GetRelaxMinTime(), false);
	}
}

UDirectorFSMState::State UFSMState_Relax::CheckForTransitions()
{
	if (isTimePassed)
	{
		// Check to see if all players are below max intensity
		for (TActorIterator<AColouringBookCharacter> PlayerItr(GetOuter()->GetWorld()); PlayerItr; ++PlayerItr)
		{
			UIntensityTracker* intensityTracker = (*PlayerItr)->GetIntensityTracker();
			if (intensityTracker->GetIntensity() >= 1.0f)
			{
				return State::NONE; // If the player is still at max intensity, stay in this state
			}
		}

		// No player has max intensity
		return State::BUILD_UP;
	}

	return State::NONE;
}

void UFSMState_Relax::OnTimerElapse()
{
	isTimePassed = true;
}