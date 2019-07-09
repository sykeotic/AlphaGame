#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CombatActor.h"
#include "Runtime/Engine/Classes/GameFramework/ProjectileMovementComponent.h"
#include "Runtime/Engine/Classes/Components/SphereComponent.h"
#include "CombatProjectile.generated.h"

UCLASS()
class ALPHA_API ACombatProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	ACombatProjectile();

	virtual void BeginPlay() override;

	UFUNCTION()
	void Fire(const FVector& ShootDirection);

	void SetCombatActorOwner(ACombatActor* NewOwner);

	UFUNCTION()
	void OnHit(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	ACombatActor* CombatActorOwner;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement)
		UProjectileMovementComponent* ProjectileMovement;

	UPROPERTY(VisibleDefaultsOnly, Category = Projectile)
		USphereComponent* CollisionComp;

	UPROPERTY(EditDefaultsOnly)
		float MaxSpeed;

	UPROPERTY(EditDefaultsOnly)
		float InitialSpeed;

	UPROPERTY(EditDefaultsOnly)
		float Bounciness;

	UPROPERTY(EditDefaultsOnly)
		bool bShouldBounce;

	UPROPERTY(EditDefaultsOnly)
		bool bRotationFollowsVelocity;

	UPROPERTY(EditDefaultsOnly)
		bool bDiesUponCollision;

protected:


};
