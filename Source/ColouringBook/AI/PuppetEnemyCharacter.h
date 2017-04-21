// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Character.h"
#include "ColouringBookProjectile.h"
#include "PuppetEnemyCharacter.generated.h"

UCLASS()
class COLOURINGBOOK_API APuppetEnemyCharacter : public ACharacter
{
	GENERATED_BODY()

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
