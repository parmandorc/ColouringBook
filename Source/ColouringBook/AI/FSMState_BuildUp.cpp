// Fill out your copyright notice in the Description page of Project Settings.

#include "ColouringBook.h"
#include "FSMState_BuildUp.h"

UFSMState_BuildUp::UFSMState_BuildUp()
{
	
}

void UFSMState_BuildUp::OnEnter()
{
	UE_LOG(LogTemp, Log, TEXT("BuildUp State: OnEnter"));
}

void UFSMState_BuildUp::Tick()
{
	UE_LOG(LogTemp, Log, TEXT("BuildUp State: Tick"));
}