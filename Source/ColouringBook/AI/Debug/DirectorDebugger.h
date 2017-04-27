// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#if WITH_GAMEPLAY_DEBUGGER

#include "GameplayDebuggerCategory.h"

class COLOURINGBOOK_API FDirectorDebugger : public FGameplayDebuggerCategory
{
public:
	FDirectorDebugger();
	
	virtual void CollectData(APlayerController* OwnerPC, AActor* DebugActor) override;
	virtual void DrawData(APlayerController* OwnerPC, FGameplayDebuggerCanvasContext& CanvasContext) override;

	static TSharedRef<FGameplayDebuggerCategory> MakeInstance();

protected:
	struct FDebugData
	{
		FName currentFSMState;
		TArray<float> playerIntensities;

		void Serialize(FArchive& archive);
	}
	DebugData;
};

#endif