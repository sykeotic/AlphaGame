// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RangedCombatWeapon.generated.h"

UCLASS()
class ALPHA_API ARangedCombatWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this Weapon's properties
	ARangedCombatWeapon();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
