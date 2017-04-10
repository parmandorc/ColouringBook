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

	

	////Handle spawning of enemies
	//UWorld* const World = GetWorld();


	////TODO modify spawn every second or so, modify location spawn in circle random position
	//FVector PuppetLocation = FVector(850.0f, 1000.0f, 250.0f);


	//if (World != NULL)
	//{

	//	//ACharacter* SpawnedPuppet = World->SpawnActor<ACharacter>(EnemyBP, GetActorLocation(), GetActorRotation());

	//	ACharacter* SpawnedPuppet = World->SpawnActor<ACharacter>(EnemyBP, PuppetLocation, GetActorRotation());


	//	FString PuppetPosition = *SpawnedPuppet->GetTransform().GetLocation().ToString();

	//	UE_LOG(LogTemp, Warning, TEXT("Puppet is at %s"), *PuppetPosition);

	//}


}


void ADirector::SpawnPuppets()
{
	//Handle spawning of enemies
	UWorld* const World = GetWorld();


	//TODO modify spawn every second or so, modify location spawn in circle random position
	FVector PuppetLocation = FVector(850.0f, 1000.0f, 250.0f);


	if (World != NULL)
	{
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &ADirector::SpawnPuppets, 3.0f, true);
		//ACharacter* SpawnedPuppet = World->SpawnActor<ACharacter>(EnemyBP, GetActorLocation(), GetActorRotation());

		ACharacter* SpawnedPuppet = World->SpawnActor<ACharacter>(EnemyBP, PuppetLocation, GetActorRotation());


		FString PuppetPosition = *SpawnedPuppet->GetTransform().GetLocation().ToString();

		UE_LOG(LogTemp, Warning, TEXT("Puppet is at %s"), *PuppetPosition);

	}
}


void ADirector::CustomSpawningTimer()
{
	
	//GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &ADirector::SpawnPuppets, 5.0f, true);
}



