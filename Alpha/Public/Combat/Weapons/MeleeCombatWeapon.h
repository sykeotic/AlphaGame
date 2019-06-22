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

protected:

};
