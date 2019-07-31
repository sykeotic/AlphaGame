// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CombatWeapon.h"
#include "RangedCombatWeapon.generated.h"

UCLASS()
class ARangedCombatWeapon : public ACombatWeapon
{
	GENERATED_BODY()
	
public:	
	ARangedCombatWeapon();

	virtual void OnUse() override;
	virtual void ExecuteUse() override;

};
