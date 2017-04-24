// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/ActorComponent.h"
#include "IntensityTracker.generated.h"

UCLASS( ClassGroup=(AI), meta=(BlueprintSpawnableComponent) )
class COLOURINGBOOK_API UIntensityTracker : public UActorComponent
{
	GENERATED_BODY()

	// The rate at which intensity decays
	UPROPERTY(Category = Gameplay, EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float DecayRate;

	// The amount of time that the intensity is locked at its maximum value before decaying
	UPROPERTY(Category = Gameplay, EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float TimeMaxIntensityLock;

	/* The value by which increment intensity upon being hit by an enemy */
	UPROPERTY(Category = Gameplay, EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float OnHitIntensity;

	/* The value by which increment intensity upon hitting a close enemy */
	UPROPERTY(Category = Gameplay, EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float OnEnemyHitIntensity;

	/* The maximum distance at which OnEnemyHit events affect intensity */
	UPROPERTY(Category = Gameplay, EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float OnEnemyHitMaxDistance;

public:	
	// Sets default values for this component's properties
	UIntensityTracker();

	// Called when the game starts
	virtual void BeginPlay() override;

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// Called when being hit by an enemy
	void OnHit();

	// Called when hitting an enemy
	void OnEnemyHit(AActor* enemy);

private:
	// The current intensity value
	float intensity;

	// Whether the intensity is locked and cannot decay
	bool isLocked;

	// The timer handle used for decay unlocking
	FTimerHandle decayLockTimerHandle;

	// Called when the timer for decay unlocking elapses
	void UnlockDecay();

	// Increments the intensity value
	void Input(float value);

public:
	// Returns the current intensity value
	FORCEINLINE float GetIntensity() const { return intensity; }
};
