// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "Director.generated.h"

UCLASS(Blueprintable)
class COLOURINGBOOK_API ADirector : public AActor
{
	GENERATED_BODY()
	
	// The class to use for spawning enemies in the scene
	UPROPERTY(Category = Gameplay, EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<ACharacter> EnemyBP;
	
public:	
	// Sets default values for this actor's properties
	ADirector();
	FTimerHandle TimerHandle;
	int32 EnemiesSpawned = 0;
	bool EnemyLimitReached = false;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	//helper to generate random position for enemies to spawn
	FVector GetRandomCirclePosition(FVector center, float radius);
	
	//allows custom timing for spawning enemies
	void SpawnPuppets();

	
};
