// Copyright 1998-2017 Epic Games, Inc. All Rights Reserve

#include "ColouringBook.h"
#include "ColouringBookProjectile.h"
#include "ColouringBookInkDrop.h"
#include "GameFramework/ProjectileMovementComponent.h"

AColouringBookProjectile::AColouringBookProjectile() 
{
	// make sure that replicates
	bReplicates = true;

	// Static reference to the mesh to use for the projectile
	static ConstructorHelpers::FObjectFinder<UStaticMesh> ProjectileMeshAsset(TEXT("/Game/Meshes/Player/TwinStickProjectile.TwinStickProjectile"));

	// Create mesh component for the projectile sphere
	ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ProjectileMesh0"));
	ProjectileMesh->SetStaticMesh(ProjectileMeshAsset.Object);
	ProjectileMesh->SetupAttachment(RootComponent);
	ProjectileMesh->BodyInstance.SetCollisionProfileName("Projectile");
	ProjectileMesh->OnComponentHit.AddDynamic(this, &AColouringBookProjectile::OnHit);		// set up a notification for when this component hits something
	RootComponent = ProjectileMesh;

	// Use a ProjectileMovementComponent to govern this projectile's movement
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement0"));
	ProjectileMovement->UpdatedComponent = ProjectileMesh;
	ProjectileMovement->InitialSpeed = 3000.f;
	ProjectileMovement->MaxSpeed = 3000.f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bShouldBounce = false;
	ProjectileMovement->ProjectileGravityScale = 0.f; // No gravity

	// Die after 3 seconds by default
	InitialLifeSpan = 3.0f;

	// Defaults
	InkDropsSpawnAmount = 10;
	InkDropsSpawnAngleVariance = 15.0f;
}

void AColouringBookProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (Role != ROLE_Authority)
	{
		// For the moment, only allow the server to process hits
		return;
	}

	// Only add impulse and destroy projectile if we hit a physics
	if ((OtherActor != NULL) && (OtherActor != this) && (OtherComp != NULL) && OtherComp->IsSimulatingPhysics())
	{
		OtherComp->AddImpulseAtLocation(GetVelocity() * 20.0f, GetActorLocation());

		SpawnInkDrops();
	}

	Destroy();
}

void AColouringBookProjectile::SpawnInkDrops()
{
	const FRotator fireRotation = GetActorRotation();
	const FVector spawnLocation = GetActorLocation();// + FireRotation.RotateVector(GunOffset);

	UWorld* const World = GetWorld();
	if (World != NULL)
	{
		for (int i = 0; i < InkDropsSpawnAmount; i++)
		{
			// spawn the projectile
			const FRotator randomRotation = FRotator::MakeFromEuler(FVector(0.0f,
				FMath::FRandRange(-InkDropsSpawnAngleVariance, InkDropsSpawnAngleVariance),
				FMath::FRandRange(-InkDropsSpawnAngleVariance, InkDropsSpawnAngleVariance)));
			AColouringBookInkDrop *inkDrop = World->SpawnActor<AColouringBookInkDrop>(spawnLocation, fireRotation + randomRotation);
			inkDrop->SetActorScale3D(FVector::FVector(FMath::FRandRange(0.5f, 2.0f)));
			inkDrop->SetOwnerID(ownerID);
		}
	}
}