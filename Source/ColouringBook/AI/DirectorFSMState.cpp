// Fill out your copyright notice in the Description page of Project Settings.

#include "ColouringBook.h"
#include "DirectorFSMState.h"
#include "Director.h"
#include "FSMState_BuildUp.h"
#include "FSMState_Peak.h"
#include "FSMState_Relax.h"

UDirectorFSMState::UDirectorFSMState()
{

}

TArray<UDirectorFSMState*> UDirectorFSMState::CreateFSM(ADirector* director)
{
	TArray<UDirectorFSMState*> FSM;
	
	// Add all the states
	FSM.Add(NewObject<UDirectorFSMState>(director, UFSMState_BuildUp::StaticClass())->SetDirector(director));
	FSM.Add(NewObject<UDirectorFSMState>(director, UFSMState_Peak::StaticClass())->SetDirector(director));
	FSM.Add(NewObject<UDirectorFSMState>(director, UFSMState_Relax::StaticClass())->SetDirector(director));

	return FSM;
}