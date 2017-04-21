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

	//ADirector::CustomSpawningTimer();
	
	SpawnPuppets();

}

// Called every frame
void ADirector::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

FVector ADirector::GetRandomCirclePosition(FVector center, float radius)
{
	
	float angle = FMath::RandRange(0.0f, 270.0f);

	FVector position;

	position.X = center.X + radius*FMath::Sin(angle*FMath::DegreesToRadians(angle));
	position.Y = center.Y + radius*FMath::Cos(angle*FMath::DegreesToRadians(angle));
	position.Z = center.Z;

	return position;

}

//TODO make enemies not spawn near player

//Called every x seconds with FTimerManager::SetTimer()
void ADirector::SpawnPuppets()
{
	//Handle spawning of enemies
	UWorld* const World = GetWorld();

	FVector center(0.0f, 0.0f, 250.0f);
	float radius = 1200.0f;
	FVector PuppetLocation = GetRandomCirclePosition(center, radius);

	if (World != NULL && !EnemyLimitReached)
	{
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &ADirector::SpawnPuppets, 1.5f, true);

		EnemiesSpawned++;

		//setting limit to spawn enemies. TODO change later or rewrite
		if (EnemiesSpawned > 15) 
		{
			GetWorld()->GetTimerManager().ClearAllTimersForObject(this);
		}
		
		//ACharacter* SpawnedPuppet = World->SpawnActor<ACharacter>(EnemyBP, GetActorLocation(), GetActorRotation());

		ACharacter* SpawnedPuppet = World->SpawnActor<ACharacter>(EnemyBP, PuppetLocation, GetActorRotation());
		
		//Debug
		FString PuppetPosition = *SpawnedPuppet->GetTransform().GetLocation().ToString();
		UE_LOG(LogTemp, Warning, TEXT("Puppet is at %s"), *PuppetPosition);

	}
}




