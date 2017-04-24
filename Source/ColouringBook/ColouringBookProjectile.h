// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.
#pragma once

#include "GameFramework/Actor.h"
#include "ColouringBookCharacter.h"
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

public:
	AColouringBookProjectile();

	/** Function to handle the projectile hitting something */
	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	void SetOwnerID(int32 _ownerID) { ownerID = _ownerID; }
	void SetPlayerOwner(AColouringBookCharacter* _owner) { owner = _owner; }

private:
	/* Spawns the ink drops from this bullet */
	void SpawnInkDrops();

private:

	/* Sets the number ID of the actor that spawned this actor */
	UPROPERTY(Replicated)
		int32 ownerID;

	/* Sets the player that owns this actor */
	AColouringBookCharacter* owner;

public:
	/** Returns ProjectileMesh subobject **/
	FORCEINLINE UStaticMeshComponent* GetProjectileMesh() const { return ProjectileMesh; }
	/** Returns ProjectileMovement subobject **/
	FORCEINLINE UProjectileMovementComponent* GetProjectileMovement() const { return ProjectileMovement; }
	/* Returns the player that owns this actor */
	FORCEINLINE AColouringBookCharacter* GetPlayerOwner() const { return owner; }
};



