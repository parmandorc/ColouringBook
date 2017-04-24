// Fill out your copyright notice in the Description page of Project Settings.

#include "ColouringBook.h"
#include "IntensityTracker.h"

// Sets default values for this component's properties
UIntensityTracker::UIntensityTracker()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// Default values
	DecayRate = 0.1f;
	TimeMaxIntensityLock = 3.0f;
	OnHitIntensity = 0.1f;
	OnEnemyHitIntensity = 0.05f;
	OnEnemyHitMaxDistance = 1000.0f;
}

// Called when the game starts
void UIntensityTracker::BeginPlay()
{
	Super::BeginPlay();

	intensity = 0.0f;
}

// Called every frame
void UIntensityTracker::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!isLocked)
	{
		intensity = FMath::Max(intensity - DecayRate * DeltaTime, 0.0f);
	}

	UE_LOG(LogTemp, Log, TEXT("Intensity (%s): %f"), *(GetOwner()->GetName()), intensity);
}

void UIntensityTracker::Input(float value)
{
	intensity = FMath::Min(intensity + value, 1.0f);

	if (intensity >= 1.0f)
	{
		isLocked = true;

		UWorld* world = GetWorld();
		if (world != nullptr)
		{
			world->GetTimerManager().SetTimer(decayLockTimerHandle, this, &UIntensityTracker::UnlockDecay, TimeMaxIntensityLock, false);
		}
	}
}

void UIntensityTracker::UnlockDecay()
{
	isLocked = false;
}

void UIntensityTracker::OnHit()
{
	Input(OnHitIntensity);
}

void UIntensityTracker::OnEnemyHit(AActor* enemy)
{
	if (GetOwner()->GetDistanceTo(enemy) <= OnEnemyHitMaxDistance)
	{
		Input(OnEnemyHitIntensity);
	}
}