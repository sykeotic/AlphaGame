// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProjectileData.h"
#include "BaseProjectile.generated.h"

class ABaseCombatActor;
class UCombatComponent;

UCLASS()
class ALPHA_API ABaseProjectile : public AActor
{
	GENERATED_BODY()
	
public:
	ABaseProjectile();

	void Tick(float DeltaSeconds) override;

	FProjectileDataStruct ProjectileData;

	UPROPERTY(Transient)
		UParticleSystemComponent* UsePSC;

	void Fire(const FVector& ShootDirection);
	void SetCombatActorOwner(ABaseCombatActor* NewOwner);
	void InitProjectileData(FProjectileDataStruct InData, FVector InLocation);

	UFUNCTION()
		void OnHit(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
protected:
	virtual void BeginPlay() override;

private:
	ABaseCombatActor* CombatActorOwner;
	UProjectileMovementComponent* ProjectileMovement;
	UStaticMeshComponent* MeshComp;
	USphereComponent* CollisionComp;
};
