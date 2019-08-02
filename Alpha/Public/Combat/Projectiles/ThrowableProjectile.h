#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Runtime/Engine/Classes/GameFramework/ProjectileMovementComponent.h"
#include "Runtime/Engine/Classes/Components/SphereComponent.h"
#include "ThrowableProjectile.generated.h"

class ACombatActor;
class UCombatComponent;

UCLASS()
class ALPHA_API AThrowableProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	AThrowableProjectile();

	virtual void BeginPlay() override;

	UFUNCTION()
		void Fire(const FVector& ShootDirection);

	void SetCombatActorOwner(ACombatActor* NewOwner);

	void Detonate();

	USoundCue* PickRandomSound(TArray<USoundCue*> InSoundArray);

	UFUNCTION()
		void OnHit(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	ACombatActor* CombatActorOwner;

	UPROPERTY(EditDefaultsOnly)
		FVector FXScaleTransform;

	UPROPERTY(EditDefaultsOnly)
		FVector FXLocationTransform;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement)
		UProjectileMovementComponent* ProjectileMovement;

	UPROPERTY(VisibleDefaultsOnly, Category = Projectile)
		USphereComponent* CollisionComp;

	UPROPERTY(EditDefaultsOnly, Category = "Sounds")
		TArray<USoundCue*> ImpactSound;

	UPROPERTY(EditDefaultsOnly, Category = "Sounds")
		TArray<USoundCue*> DetonationSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SFX")
		UParticleSystem* ImpactFX;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SFX")
		UParticleSystem* DetonationFX;

	UPROPERTY(Transient)
		UParticleSystemComponent* DetonationPSC;

	UPROPERTY(EditDefaultsOnly)
		UStaticMeshComponent* MeshComp;

	UPROPERTY(EditDefaultsOnly)
		float MaxSpeed;

	UPROPERTY(EditDefaultsOnly)
		float InitialSpeed;

	UPROPERTY(EditDefaultsOnly)
		float Bounciness;

	UPROPERTY(EditDefaultsOnly)
		float BlastRadius;

	UPROPERTY(EditDefaultsOnly)
		bool bShouldBounce;

	UPROPERTY(EditDefaultsOnly)
		bool bRotationFollowsVelocity;

	UPROPERTY(EditDefaultsOnly)
		bool bDiesUponCollision;

	FTimerHandle DeathTimer;

};
