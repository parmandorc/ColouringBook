// Fill out your copyright notice in the Description page of Project Settings.

#include "ColouringBook.h"
#include "Director.h"


// Sets default values
ADirector::ADirector()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ADirector::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ADirector::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//Handle spawning of enemies
	/* UWorld* const World = GetWorld();
	if (World != NULL)
	{
		World->SpawnActor<ACharacter>(EnemyBP, GetActorLocation(), GetActorRotation());
	}
	*/
}

