// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CombatActor.h"
#include "CombatWeapon.generated.h"

UCLASS()
class ACombatWeapon : public ACombatActor
{
	GENERATED_BODY()
	
public:	
	ACombatWeapon();

	virtual void OnUse() override;

	FRotator WeaponRotation;
	FVector WeaponLocation;

protected:

};
