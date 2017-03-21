// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "ColouringBook.h"
#include "ColouringBookGameMode.h"
#include "ColouringBookCharacter.h"

AColouringBookGameMode::AColouringBookGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/Blueprints/ColouringBookCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}

