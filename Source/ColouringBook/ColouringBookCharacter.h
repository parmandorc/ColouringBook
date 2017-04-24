// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.
#pragma once

#include "GameFramework/Character.h"
#include "AI/IntensityTracker.h"
#include "ColouringBookCharacter.generated.h"

UCLASS(Blueprintable)
class AColouringBookCharacter : public ACharacter
{
	GENERATED_BODY()

	/* The number to identify this player */
	UPROPERTY(Category = Gameplay, EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	uint8 PlayerID;

	/** Offset from the character location to spawn projectiles */
	UPROPERTY(Category = Gameplay, EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	FVector GunOffset;

	/* How fast the weapon will fire */
	UPROPERTY(Category = Gameplay, EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float FireRate;

	/** Sound to play each time we fire */
	UPROPERTY(Category = Audio, EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class USoundBase* FireSound;

	/* The intensity tracker component attached to this player */
	UPROPERTY(Category = AI, EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UIntensityTracker* IntensityTrackerComponent;

public:
	AColouringBookCharacter();

	// Begin Actor Interface
	virtual void Tick(float DeltaSeconds) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;
	// End Actor Interface

	/* Fire a shot in the specified direction */
	void FireShot(FVector FireDirection);

	/* Handler for the fire timer expiry */
	void ShotTimerExpired();

	// Static names for axis bindings
	static const FName FireForwardBinding;
	static const FName FireRightBinding;

	/* Called when this player hit an enemy */
	void OnEnemyHit(AActor* enemy);

protected:
	/** Called for forwards/backward input */
	void MoveForward(float Value);

	/** Called for side to side input */
	void MoveRight(float Value);

	/* Function to handle the character being hit by something */
	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

private:
	/* Flag to control firing  */
	uint32 bCanFire : 1;

	/** Handle for efficient management of ShotTimerExpired timer */
	FTimerHandle TimerHandle_ShotTimerExpired;

public:
	/* Returns the number that identifies this player */
	FORCEINLINE uint8 GetPlayerID() const { return PlayerID; }
	/* Returns the intensity tracker component attached to this player */
	FORCEINLINE UIntensityTracker* GetIntensityTracker() const { return IntensityTrackerComponent; }
};

