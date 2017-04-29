// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "ColouringBook.h"
#include "AI/Debug/DirectorDebugger.h"
#include "GameplayDebugger.h"

IMPLEMENT_PRIMARY_GAME_MODULE( FColouringBookModule, ColouringBook, "ColouringBook" );

DEFINE_LOG_CATEGORY(LogColouringBook)
 
#define LOCTEXT_NAMESPACE "ColouringBook"
void FColouringBookModule::StartupModule()
{
	UE_LOG(LogColouringBook, Log, TEXT("ColouringBook: Log Started"));

#if WITH_GAMEPLAY_DEBUGGER
	IGameplayDebugger& GameplayDebuggerModule = IGameplayDebugger::Get();
	GameplayDebuggerModule.RegisterCategory("AI Director", IGameplayDebugger::FOnGetCategory::CreateStatic(&FDirectorDebugger::MakeInstance), EGameplayDebuggerCategoryState::EnabledInGame, 7);
	GameplayDebuggerModule.NotifyCategoriesChanged();
#endif
}

void FColouringBookModule::ShutdownModule()
{
	UE_LOG(LogColouringBook, Log, TEXT("ColouringBook: Log Ended"));
}
#undef LOCTEXT_NAMESPACE