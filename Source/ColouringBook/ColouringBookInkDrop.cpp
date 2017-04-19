#include "ColouringBook.h"
#include "ColouringBookInkDrop.h"
#include "GameFramework/ProjectileMovementComponent.h"

AColouringBookInkDrop::AColouringBookInkDrop()
{
	// Static reference to the mesh to use for the projectile
	static ConstructorHelpers::FObjectFinder<UStaticMesh> InkDropMeshAsset(TEXT("/Game/Meshes/InkDrop.InkDrop"));

	// Create mesh component for the projectile sphere
	InkDropMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("InkDropMesh0"));
	InkDropMesh->SetStaticMesh(InkDropMeshAsset.Object);
	InkDropMesh->SetupAttachment(RootComponent);
	InkDropMesh->BodyInstance.SetCollisionProfileName("InkDrop");
	RootComponent = InkDropMesh;

	// Use a ProjectileMovementComponent to govern this projectile's movement
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement0"));
	ProjectileMovement->UpdatedComponent = InkDropMesh;
	ProjectileMovement->InitialSpeed = 650.0f;
	ProjectileMovement->MaxSpeed = 3000.0f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bShouldBounce = false;
	ProjectileMovement->ProjectileGravityScale = 1.0f;

	// Die after 3 seconds by default
	InitialLifeSpan = 3.0f;
}