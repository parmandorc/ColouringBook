// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "ColouringBook.h"
#include "ColouringBookPawn.h"
#include "ColouringBookProjectile.h"
#include "TimerManager.h"

const FName AColouringBookPawn::MoveForwardBinding("MoveForward");
const FName AColouringBookPawn::MoveRightBinding("MoveRight");
const FName AColouringBookPawn::FireForwardBinding("FireForward");
const FName AColouringBookPawn::FireRightBinding("FireRight");

AColouringBookPawn::AColouringBookPawn()
{	
	// Create the mesh component
	static ConstructorHelpers::FObjectFinder<UStaticMesh> ShipMesh(TEXT("/Game/Meshes/Player/TwinStickUFO.TwinStickUFO"));
	ShipMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ShipMesh"));
	RootComponent = ShipMeshComponent;
	ShipMeshComponent->SetCollisionProfileName(UCollisionProfile::Pawn_ProfileName);
	ShipMeshComponent->SetStaticMesh(ShipMesh.Object);
	
	// Cache our sound effect
	static ConstructorHelpers::FObjectFinder<USoundBase> FireAudio(TEXT("/Game/Audio/TwinStickFire.TwinStickFire"));
	FireSound = FireAudio.Object;

	// Movement
	MoveSpeed = 1000.0f;

	// Weapon
	GunOffset = FVector(90.f, 0.f, 0.f);
	FireRate = 0.1f;
	bCanFire = true;
}

void AColouringBookPawn::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	check(PlayerInputComponent);

	// AutoPossessPlayer and AutoReceiveInput to the correct player according to the ControllerId
	int32 controllerId = GetNetOwningPlayer()->GetPlayerController(GetWorld())->GetLocalPlayer()->GetControllerId();
	switch (controllerId)
	{
	case 0: AutoPossessPlayer = AutoReceiveInput = EAutoReceiveInput::Player0;  break;
	case 1: AutoPossessPlayer = AutoReceiveInput = EAutoReceiveInput::Player1; break;
	case 2: AutoPossessPlayer = AutoReceiveInput = EAutoReceiveInput::Player2; break;
	case 3: AutoPossessPlayer = AutoReceiveInput = EAutoReceiveInput::Player3; break;
	case 4: AutoPossessPlayer = AutoReceiveInput = EAutoReceiveInput::Player4; break;
	case 5: AutoPossessPlayer = AutoReceiveInput = EAutoReceiveInput::Player5; break;
	case 6: AutoPossessPlayer = AutoReceiveInput = EAutoReceiveInput::Player6; break;
	case 7: AutoPossessPlayer = AutoReceiveInput = EAutoReceiveInput::Player7; break;
	default: AutoPossessPlayer = AutoReceiveInput = EAutoReceiveInput::Disabled; break;
	}

	// set up gameplay key bindinyygs
	PlayerInputComponent->BindAxis(MoveForwardBinding);
	PlayerInputComponent->BindAxis(MoveRightBinding);
	PlayerInputComponent->BindAxis(FireForwardBinding);
	PlayerInputComponent->BindAxis(FireRightBinding);
}

void AColouringBookPawn::Tick(float DeltaSeconds)
{
	// Find movement direction
	const float ForwardValue = GetInputAxisValue(MoveForwardBinding);
	const float RightValue = GetInputAxisValue(MoveRightBinding);

	// Clamp max size so that (X=1, Y=1) doesn't cause faster movement in diagonal directions
	const FVector MoveDirection = FVector(ForwardValue, RightValue, 0.f).GetClampedToMaxSize(1.0f);

	// Calculate  movement
	const FVector Movement = MoveDirection * MoveSpeed * DeltaSeconds;

	// If non-zero size, move this actor
	if (Movement.SizeSquared() > 0.0f)
	{
		const FRotator NewRotation = Movement.Rotation();
		FHitResult Hit(1.f);
		RootComponent->MoveComponent(Movement, NewRotation, true, &Hit);
		
		if (Hit.IsValidBlockingHit())
		{
			const FVector Normal2D = Hit.Normal.GetSafeNormal2D();
			const FVector Deflection = FVector::VectorPlaneProject(Movement, Normal2D) * (1.f - Hit.Time);
			RootComponent->MoveComponent(Deflection, NewRotation, true);
		}
	}
	
	// Create fire direction vector
	const float FireForwardValue = GetInputAxisValue(FireForwardBinding);
	const float FireRightValue = GetInputAxisValue(FireRightBinding);
	const FVector FireDirection = FVector(FireForwardValue, FireRightValue, 0.f);

	// Try and fire a shot
	FireShot(FireDirection);
}

void AColouringBookPawn::FireShot(FVector FireDirection)
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
			World->GetTimerManager().SetTimer(TimerHandle_ShotTimerExpired, this, &AColouringBookPawn::ShotTimerExpired, FireRate);

			// try and play the sound if specified
			if (FireSound != nullptr)
			{
				UGameplayStatics::PlaySoundAtLocation(this, FireSound, GetActorLocation());
			}

			bCanFire = false;
		}
	}
}

void AColouringBookPawn::ShotTimerExpired()
{
	bCanFire = true;
}

