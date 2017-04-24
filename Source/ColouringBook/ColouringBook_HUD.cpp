// Fill out your copyright notice in the Description page of Project Settings.

#include "ColouringBook.h"
#include "ColouringBook_HUD.h"
#include "ColouringBookGameMode.h"
#include "ColouringBookCharacter.h"

AColouringBook_HUD::AColouringBook_HUD()
{
	static ConstructorHelpers::FObjectFinder<UFont>OurHudFont(TEXT("/ Game/ColouringBook_Font.ColouringBook_Font"));

	HUDFont = OurHudFont.Object;
}


void AColouringBook_HUD::DrawHUD()
{
	Super::DrawHUD();

	FVector2D ScreenDimensions = FVector2D(Canvas->SizeX, Canvas->SizeY);

	AColouringBookCharacter* BookCharacter = Cast<AColouringBookCharacter>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
	AColouringBookGameMode* BookGameMode = Cast<AColouringBookGameMode>(GetWorld()->GetAuthGameMode());

}
