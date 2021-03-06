#pragma once

#include "GameFramework/Actor.h"
#include "ColouringBookInkDrop.generated.h"

class UProjectileMovementComponent;
class UStaticMeshComponent;

UCLASS(config = Game)
class AColouringBookInkDrop : public AActor
{
	GENERATED_BODY()

	/** Sphere collision component */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Projectile, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* InkDropMesh;

	/** Projectile movement component */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	UProjectileMovementComponent* ProjectileMovement;

public:
	AColouringBookInkDrop();

	// Sets the owner
	void SetOwnerID(uint8 _ownerID);

private:
	/* The number ID of the player that owns this actor */
	uint8 ownerID;

	/* The dynamic material instance of the ink drop */
	class UMaterialInstanceDynamic* dynamicMaterial;

public:
	/** Returns InkDropMesh subobject **/
	FORCEINLINE UStaticMeshComponent* GetInkDropMesh() const { return InkDropMesh; }
	/** Returns ProjectileMovement subobject **/
	FORCEINLINE UProjectileMovementComponent* GetProjectileMovement() const { return ProjectileMovement; }
	/* Returns the number ID of the player that owns this actor */
	FORCEINLINE uint8 GetOwnerID() const { return ownerID; }
};
