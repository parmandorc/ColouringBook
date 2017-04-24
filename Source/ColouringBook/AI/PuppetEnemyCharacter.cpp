// Fill out your copyright notice in the Description page of Project Settings.

#include "ColouringBook.h"
#include "PuppetEnemyCharacter.h"
#include "ColouringBookInkDrop.h"

// Sets default values
APuppetEnemyCharacter::APuppetEnemyCharacter()
{
	// Make sure that this character replicates
	bReplicates = true;

 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	// Set up a notification for when this component is hit by something
	GetCapsuleComponent()->OnComponentHit.AddDynamic(this, &APuppetEnemyCharacter::OnHit);

	// Defaults
	GunOffset = FVector(90.0f, 0.0f, 0.0f);
	InkDropsSpawnAmount = 10;
	InkDropsSpawnAngleVariance = 15.0f;
}

void APuppetEnemyCharacter::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (Role != ROLE_Authority)
	{
		// for the moment only allow the server to proccess hits
		return;
	}

	AColouringBookProjectile* bullet = nullptr;
	if ((OtherActor != NULL) && (OtherActor != this) && ((bullet = Cast<AColouringBookProjectile>(OtherActor)) != nullptr))
	{
		SpawnInkDrops(bullet);

		// Destroy the bullet actor
		OtherActor->Destroy();

		// Kill the puppet character
		OnPreDeath(bullet->GetPlayerOwner());
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

			int32 ownerId = bullet->GetOwnerID();
			inkDrop->SetOwnerID(ownerId);
		}
	}
}

void APuppetEnemyCharacter::OnPreDeath(AColouringBookCharacter* player)
{
	SetActorEnableCollision(false); // So an enemy can not be shot after dead and spawn ink drops more that once

	if (player)
	{
		player->OnEnemyHit(this); // Callback event to the player that killed this enemy
	}

	if (director)
	{
		director->OnEnemyDeath(this); // Callback event to the director
	}
}

void APuppetEnemyCharacter::OnDeath()
{
	Destroy();
}

void APuppetEnemyCharacter::Fire()
{
	if (Role != ROLE_Authority)
	{
		// Only allow the server to fire
		return;
	}

	const FVector FireDirection = GetActorForwardVector();
	const FRotator FireRotation = FireDirection.Rotation();
	// Spawn projectile at an offset from this pawn
	const FVector SpawnLocation = GetActorLocation() + FireRotation.RotateVector(GunOffset);

	UWorld* const World = GetWorld();
	if (World != NULL && ProjectileBP != nullptr)
	{
		// spawn the projectile
		World->SpawnActor<AColouringBookProjectile>(ProjectileBP, SpawnLocation, FireRotation);
	}

	// try and play the sound if specified
	if (FireSound != nullptr)
	{
		UGameplayStatics::PlaySoundAtLocation(this, FireSound, GetActorLocation());
	}
}