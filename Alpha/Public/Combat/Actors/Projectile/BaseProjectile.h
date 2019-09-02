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

	void Fire(const FVector& ShootDirection);
	void SetCombatActorOwner(ABaseCombatActor* NewOwner);
	void InitProjectileData(FProjectileDataStruct InData, FVector InLocation);

	UProjectileMovementComponent* ProjectileMovement;

	UFUNCTION()
		void OnHit(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
protected:
	virtual void BeginPlay() override;

private:
	ABaseCombatActor* CombatActorOwner;

	UStaticMeshComponent* MeshComp;
	USphereComponent* CollisionComp;
};
