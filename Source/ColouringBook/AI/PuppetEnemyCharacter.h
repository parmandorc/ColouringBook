// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Character.h"
#include "ColouringBookProjectile.h"
#include "PuppetEnemyCharacter.generated.h"

UCLASS()
class COLOURINGBOOK_API APuppetEnemyCharacter : public ACharacter
{
	GENERATED_BODY()

	/* Offset from the character location to spawn projectiles */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Gameplay, meta = (AllowPrivateAccess = "true"))
	FVector GunOffset;

	/* The class to use for spawning bullet in the scene when firing */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Gameplay, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AColouringBookProjectile> ProjectileBP;

	/* Sound to play each time we fire */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Audio, meta = (AllowPrivateAccess = "true"))
	class USoundBase* FireSound;

	/* The amount of ink drops to be spawned when shot */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = InkDrops, meta = (AllowPrivateAccess = "true"))
	int InkDropsSpawnAmount;

	/* The angle of the at which the ink drops should be spawned when shot */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = InkDrops, meta = (AllowPrivateAccess = "true"))
	float InkDropsSpawnAngleVariance;

	/* The amount of time delay to wait after the character dies and before it is destroyed */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Gameplay, meta = (AllowPrivateAccess = "true"))
	float DeathDestructionDelay;

public:
	// Sets default values for this character's properties
	APuppetEnemyCharacter();

	// Fires in the direction the puppet character is facing
	UFUNCTION(BlueprintCallable, Category = Combat)
	void Fire();

private:
	/* Function to handle the character being hit by something */
	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);	

	/* Spawns the ink drops when shot */
	void SpawnInkDrops(AColouringBookProjectile* bullet);

	/* Called before the character dies */
	void OnPreDeath();

	/* Called when the character dies */
	void OnDeath();
};
