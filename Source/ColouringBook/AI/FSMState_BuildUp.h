// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AI/DirectorFSMState.h"
#include "FSMState_BuildUp.generated.h"

/**
 * 
 */
UCLASS()
class COLOURINGBOOK_API UFSMState_BuildUp : public UDirectorFSMState
{
	GENERATED_BODY()

public:
	UFSMState_BuildUp();

	// FSMState interface
	virtual void OnEnter() override;
	virtual void Tick() override;
};
