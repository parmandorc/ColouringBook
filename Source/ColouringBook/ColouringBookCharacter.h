// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.
#pragma once

#include "GameFramework/Character.h"
#include "ColouringBookCharacter.generated.h"

UCLASS(Blueprintable)
class AColouringBookCharacter : public ACharacter
{
	GENERATED_BODY()

	/* The number to identify this player */
	UPROPERTY(Category = Gameplay, EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	uint8 PlayerID;

	/** Offset from the ships location to spawn projectiles */
	UPROPERTY(Category = Gameplay, EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	FVector GunOffset;

	/* How fast the weapon will fire */
	UPROPERTY(Category = Gameplay, EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float FireRate;

	/** Sound to play each time we fire */
	UPROPERTY(Category = Audio, EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class USoundBase* FireSound;

	/* Flag to control firing  */
	uint32 bCanFire : 1;

	/** Handle for efficient management of ShotTimerExpired timer */
	FTimerHandle TimerHandle_ShotTimerExpired;

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

	/* Returns the number that identifies this player */
	FORCEINLINE uint8 GetPlayerID() { return PlayerID; }

protected:
	/** Called for forwards/backward input */
	void MoveForward(float Value);

	/** Called for side to side input */
	void MoveRight(float Value);

	bool LocalFireShot(FVector fireLocation, FRotator fireRotator);
	bool OnlineFireShot(FVector fireLocation, FRotator fireRotator);

private:

	// FireShot done by the server
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerFireShot(FVector fireLocation, FRotator fireRotator);

	// Called by the server to inform all the clients that a player fired
	UFUNCTION(NetMulticast, unreliable)
	void MulticastPlayerFired();

	class AColouringBookProjectile* SpawnProjectile(FVector fireLocation, FRotator fireRotator);
};

