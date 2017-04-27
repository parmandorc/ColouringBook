// Fill out your copyright notice in the Description page of Project Settings.

#include "ColouringBook.h"
#include "DirectorDebugger.h"
#include "ColouringBookCharacter.h"
#include "AI/Director.h"
#include "AI/IntensityTracker.h"

#if WITH_GAMEPLAY_DEBUGGER

FDirectorDebugger::FDirectorDebugger()
{
	SetDataPackReplication<FDebugData>(&DebugData);
}

TSharedRef<FGameplayDebuggerCategory> FDirectorDebugger::MakeInstance()
{
	return MakeShareable(new FDirectorDebugger());
}

void FDirectorDebugger::FDebugData::Serialize(FArchive& archive)
{
	archive << currentFSMState;
	for (float value : playerIntensities)
	{
		archive << value;
	}
}

void FDirectorDebugger::CollectData(APlayerController* OwnerPC, AActor* DebugActor)
{
	DebugData.currentFSMState = TEXT("-");

	ADirector* director = Cast<ADirector>(DebugActor);
	if (director != nullptr)
	{
		// Set director state string
	}

	DebugData.playerIntensities.Empty();
	for (TActorIterator<AColouringBookCharacter> PlayerItr(OwnerPC->GetControlledPawn()->GetWorld()); PlayerItr; ++PlayerItr)
	{
		DebugData.playerIntensities.Add((*PlayerItr)->GetIntensityTracker()->GetIntensity());
	}
}

void FDirectorDebugger::DrawData(APlayerController* OwnerPC, FGameplayDebuggerCanvasContext& CanvasContext)
{
	CanvasContext.Print(FString(TEXT("Current State: ")) + DebugData.currentFSMState);

	for (int i = 0; i < DebugData.playerIntensities.Num(); i++)
	{
		CanvasContext.Print(FString::Printf(TEXT("Player %d intensity: %f"), i, DebugData.playerIntensities[i]));
	}
}

#endif