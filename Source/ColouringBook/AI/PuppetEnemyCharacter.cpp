// Fill out your copyright notice in the Description page of Project Settings.

#include "ColouringBook.h"
#include "PuppetEnemyCharacter.h"
#include "ColouringBookInkDrop.h"

// Sets default values
APuppetEnemyCharacter::APuppetEnemyCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	// Set up a notification for when this component is hit by something
	GetMesh()->OnComponentHit.AddDynamic(this, &APuppetEnemyCharacter::OnHit);

	// Defaults
	InkDropsSpawnAmount = 10;
	InkDropsSpawnAngleVariance = 15.0f;
}

void APuppetEnemyCharacter::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	AColouringBookProjectile* bullet = nullptr;
	if ((OtherActor != NULL) && (OtherActor != this) && ((bullet = Cast<AColouringBookProjectile>(OtherActor)) != nullptr))
	{
		SpawnInkDrops(bullet);

		// Destroy the bullet actor
		OtherActor->Destroy();

		// Kill the puppet character
		OnPreDeath();
		if (DeathDestructionDelay > 0.0f)
		{
			FTimerHandle DeathHandle;
			GetWorldTimerManager().SetTimer(DeathHandle, this, &APuppetEnemyCharacter::OnDeath, DeathDestructionDelay, false);
		}
		else
		{
			OnDeath();
		}
	}
}

void APuppetEnemyCharacter::SpawnInkDrops(AColouringBookProjectile* bullet)
{
	const FRotator fireRotation = bullet->GetActorRotation();
	const FVector spawnLocation = bullet->GetActorLocation();

	UWorld* const World = GetWorld();
	if (World != NULL)
	{
		for (int i = 0; i < InkDropsSpawnAmount; i++)
		{
			// spawn the actor
			const FRotator randomRotation = FRotator::MakeFromEuler(FVector(0.0f,
				FMath::FRandRange(-InkDropsSpawnAngleVariance, InkDropsSpawnAngleVariance),
				FMath::FRandRange(-InkDropsSpawnAngleVariance, InkDropsSpawnAngleVariance)));
			AColouringBookInkDrop *inkDrop = World->SpawnActor<AColouringBookInkDrop>(spawnLocation, fireRotation + randomRotation);
			inkDrop->SetActorScale3D(FVector::FVector(FMath::FRandRange(0.5f, 2.0f)));
			inkDrop->SetOwnerID(bullet->GetOwnerID());
		}
	}
}

void APuppetEnemyCharacter::OnPreDeath()
{
	SetActorEnableCollision(false); // So an enemy can not be shot after dead and spawn ink drops more that once
}

void APuppetEnemyCharacter::OnDeath()
{
	Destroy();
}