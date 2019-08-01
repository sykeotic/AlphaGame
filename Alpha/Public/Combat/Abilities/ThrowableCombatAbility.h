// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Combat/Abilities/CombatAbility.h"
#include "ThrowableCombatAbility.generated.h"

/**
 * 
 */
UCLASS()
class ALPHA_API AThrowableCombatAbility : public ACombatAbility
{
	GENERATED_BODY()

public:

	AThrowableCombatAbility();
	
	virtual void OnUse() override;

	virtual void ExecuteUse() override;


};
