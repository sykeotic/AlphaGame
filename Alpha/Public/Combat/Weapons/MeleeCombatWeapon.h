// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CombatWeapon.h"
#include "MeleeCombatWeapon.generated.h"

UCLASS()
class ALPHA_API AMeleeCombatWeapon : public ACombatWeapon
{
	GENERATED_BODY()
	
public:	
	AMeleeCombatWeapon();
	virtual void OnUse() override;

	virtual void BeginPlay() override;

	UFUNCTION()
	void WeaponBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	virtual void AttachMeshToOwner(FName AttachPoint) override;

protected:

	UPROPERTY(EditDefaultsOnly)
	UBoxComponent* MeleeBoxComponent;
};
