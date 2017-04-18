// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "ColouringBook.h"
#include "ColouringBookCharacter.h"
#include "ColouringBookProjectile.h"
#include "ColouringBookGameMode.h"
#include "TimerManager.h"

const FName AColouringBookCharacter::FireForwardBinding("FireForward");
const FName AColouringBookCharacter::FireRightBinding("FireRight");

AColouringBookCharacter::AColouringBookCharacter()
{	
	// Make sure that this character replicates
	bReplicates = true;

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)

	// Weapon
	GunOffset = FVector(90.f, 0.f, 0.f);
	FireRate = 0.1f;
	bCanFire = true;

	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;
}

void AColouringBookCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	check(PlayerInputComponent);

	AColouringBookGameMode* gameMode = static_cast<AColouringBookGameMode*> (GetWorld()->GetAuthGameMode());
	if (gameMode && gameMode->GetMultiplayerMode() == AColouringBookGameMode::MultiplayerMode::LOCAL)
	{
		// AutoPossessPlayer and AutoReceiveInput to the correct player according to the ControllerId
		int32 controllerId = GetNetOwningPlayer()->GetPlayerController(GetWorld())->GetLocalPlayer()->GetControllerId();
		switch (controllerId)
		{
		case 0: AutoPossessPlayer = AutoReceiveInput = EAutoReceiveInput::Player0; break;
		case 1: AutoPossessPlayer = AutoReceiveInput = EAutoReceiveInput::Player1; break;
		case 2: AutoPossessPlayer = AutoReceiveInput = EAutoReceiveInput::Player2; break;
		case 3: AutoPossessPlayer = AutoReceiveInput = EAutoReceiveInput::Player3; break;
		case 4: AutoPossessPlayer = AutoReceiveInput = EAutoReceiveInput::Player4; break;
		case 5: AutoPossessPlayer = AutoReceiveInput = EAutoReceiveInput::Player5; break;
		case 6: AutoPossessPlayer = AutoReceiveInput = EAutoReceiveInput::Player6; break;
		case 7: AutoPossessPlayer = AutoReceiveInput = EAutoReceiveInput::Player7; break;
		default: AutoPossessPlayer = AutoReceiveInput = EAutoReceiveInput::Disabled; break;
		}
	}
	
	// set up gameplay key bindings
	PlayerInputComponent->BindAxis(FireForwardBinding);
	PlayerInputComponent->BindAxis(FireRightBinding);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAxis("MoveForward", this, &AColouringBookCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AColouringBookCharacter::MoveRight);
}

void AColouringBookCharacter::Tick(float DeltaSeconds)
{
	// Create fire direction vector
	const float FireForwardValue = GetInputAxisValue(FireForwardBinding);
	const float FireRightValue = GetInputAxisValue(FireRightBinding);
	const FVector FireDirection = FVector(FireForwardValue, FireRightValue, 0.f);

	// Try and fire a shot
	FireShot(FireDirection);
}

void AColouringBookCharacter::FireShot(FVector FireDirection)
{
	AColouringBookGameMode* gameMode = static_cast<AColouringBookGameMode*> (GetWorld()->GetAuthGameMode());
	if (gameMode && gameMode->GetMultiplayerMode() == AColouringBookGameMode::MultiplayerMode::LOCAL)
	{
		LocalFireShot(FireDirection);
	}
	else
	{
		OnlineFireShot(FireDirection);
	}
}

void AColouringBookCharacter::LocalFireShot(FVector FireDirection)
{
	// If we it's ok to fire again
	if (bCanFire == true)
	{
		// If we are pressing fire stick in a direction
		if (FireDirection.SizeSquared() > 0.0f)
		{
			const FRotator FireRotation = FireDirection.Rotation();
			// Spawn projectile at an offset from this pawn
			const FVector SpawnLocation = GetActorLocation() + FireRotation.RotateVector(GunOffset);

			UWorld* const World = GetWorld();
			if (World != NULL)
			{
				// spawn the projectile
				World->SpawnActor<AColouringBookProjectile>(SpawnLocation, FireRotation);
			}

			bCanFire = false;
			World->GetTimerManager().SetTimer(TimerHandle_ShotTimerExpired, this, &AColouringBookCharacter::ShotTimerExpired, FireRate);

			// try and play the sound if specified
			if (FireSound != nullptr)
			{
				UGameplayStatics::PlaySoundAtLocation(this, FireSound, GetActorLocation());
			}

			bCanFire = false;
		}
	}
}

void AColouringBookCharacter::OnlineFireShot(FVector FireDirection)
{
	if (Role != ROLE_AutonomousProxy)
	{
		// only allow owning client to perform this action
		return;
	}

	// For the time being use LocalFireShot
	// TO-DO: Proper client-server FireShot
	LocalFireShot(FireDirection);
}

void AColouringBookCharacter::ShotTimerExpired()
{
	bCanFire = true;
}

void AColouringBookCharacter::MoveForward(float Value)
{
	if ((Controller != NULL) && (Value != 0.0f))
	{
		AddMovementInput(FVector::ForwardVector, Value);
	}
}

void AColouringBookCharacter::MoveRight(float Value)
{
	if ((Controller != NULL) && (Value != 0.0f))
	{
		AddMovementInput(FVector::RightVector, Value);
	}
}

