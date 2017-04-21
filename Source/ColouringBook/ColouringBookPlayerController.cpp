// Fill out your copyright notice in the Description page of Project Settings.

#include "ColouringBook.h"
#include "ColouringBookPlayerController.h"

AColouringBookPlayerController::AColouringBookPlayerController(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	// Make sure that the PlayerController DOES NOT manage active camera(We are using just one camera and it is a fixed camera!)
	bAutoManageActiveCameraTarget = false;
}


