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
	DebugData.currentFSMState = FName(TEXT(""));
	DebugData.playerIntensities.Empty();

	UWorld* world = OwnerPC->GetPawn()->GetWorld();
	if (world != nullptr)
	{
		TActorIterator<ADirector> DirectorItr(world);
		if (DirectorItr)
		{
			DebugData.currentFSMState = (*DirectorItr)->GetCurrentFSMStateName();
		}

		for (TActorIterator<AColouringBookCharacter> PlayerItr(world); PlayerItr; ++PlayerItr)
		{
			DebugData.playerIntensities.Add((*PlayerItr)->GetIntensityTracker()->GetIntensity());
		}
	}
}

void FDirectorDebugger::DrawData(APlayerController* OwnerPC, FGameplayDebuggerCanvasContext& CanvasContext)
{
	CanvasContext.Print(FString(TEXT("Current State: ")) + DebugData.currentFSMState.ToString());

	for (int i = 0; i < DebugData.playerIntensities.Num(); i++)
	{
		FString valueColor = DebugData.playerIntensities[i] >= 1.0f ? "yellow" : "white";
		CanvasContext.Print(FString::Printf(TEXT("Player %d intensity: {%s}%f"), i, *valueColor, DebugData.playerIntensities[i]));
	}
}

#endif