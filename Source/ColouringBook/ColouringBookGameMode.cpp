// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "ColouringBook.h"
#include "ColouringBookGameMode.h"
#include "ColouringBookPawn.h"

AColouringBookGameMode::AColouringBookGameMode()
{
	// set default pawn class to our character class
	DefaultPawnClass = AColouringBookPawn::StaticClass();
}

