// Fill out your copyright notice in the Description page of Project Settings.

#include "ColouringBook.h"
#include "ColouringBookHUD.h"
#include "ColouringBookGameMode.h"
#include "ColouringBookCanvas.h"
#include "EngineUtils.h"

AColouringBookHUD::AColouringBookHUD()
{
	//static ConstructorHelpers::FObjectFinder<UFont>OurHudFont(TEXT("/ Game/UI/CBFont"));

	//HUDFont = OurHudFont.Object;
}

void AColouringBookHUD::DrawHUD()
{

	Super::DrawHUD(); 
	
	if (CanvasInstance != nullptr)
	{
		UWorld *world = GetWorld();

		FVector2D ScreenDimensions = FVector2D(Canvas->SizeX, Canvas->SizeY);

		AColouringBookGameMode* BookGameMode = Cast<AColouringBookGameMode>(world->GetWorld()->GetAuthGameMode());

		//FString RunTimeString = FString::Printf(TEXT("Runtime: %.4f"), BookGameMode->GetRunTime());
		FString ScorePlayer1 = FString::Printf(TEXT("Score Player 1: %d pct " ), (int)(CanvasInstance->GetScore(0) * 100.0f));
		FString ScorePlayer2 = FString::Printf(TEXT("Score Player 2: %d pct "), (int)(CanvasInstance->GetScore(1) * 100.0f));


		//FString HUDString1 = FString::Printf(TEXT("PLAYER 1 SCORE: "));
		//FString HUDString2 = FString::Printf(TEXT("PLAYER 2 SCORE: "));

		//DrawText(RunTimeString, FColor::White, 750, 150, 0, 1.2f);
		DrawText(ScorePlayer1, FColor::Green, 200, 20, 0, 1.2f);
		DrawText(ScorePlayer2, FColor::Blue, 600, 20, 0, 1.2f);
	}
}


