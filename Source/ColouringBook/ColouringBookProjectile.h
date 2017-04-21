// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.
#pragma once

#include "GameFramework/Actor.h"
#include "ColouringBookProjectile.generated.h"

class UProjectileMovementComponent;
class UStaticMeshComponent;

UCLASS(config=Game)
class AColouringBookProjectile : public AActor
{
	GENERATED_BODY()

	/** Sphere collision component */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Projectile, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* ProjectileMesh;

	/** Projectile movement component */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	UProjectileMovementComponent* ProjectileMovement;

	/* The amount of ink drops to be spawned from the bullet */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = InkDrops, meta = (AllowPrivateAccess = "true"))
	int InkDropsSpawnAmount;

	/* The amount of ink drops to be spawned from the bullet */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = InkDrops, meta = (AllowPrivateAccess = "true"))
	float InkDropsSpawnAngleVariance;

public:
	AColouringBookProjectile();

	/** Function to handle the projectile hitting something */
	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	void SetOwnerID(uint8 _ownerID) { ownerID = _ownerID; }

private:
	/* Spawns the ink drops from this bullet */
	void SpawnInkDrops();

	/* Sets the number ID of the actor that spawned this actor */
	uint8 ownerID;

public:
	/** Returns ProjectileMesh subobject **/
	FORCEINLINE UStaticMeshComponent* GetProjectileMesh() const { return ProjectileMesh; }
	/** Returns ProjectileMovement subobject **/
	FORCEINLINE UProjectileMovementComponent* GetProjectileMovement() const { return ProjectileMovement; }
};


