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

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void SpawnPuppets();


	void CustomSpawningTimer(); //function to allow spawning at desired frequency

	
};
