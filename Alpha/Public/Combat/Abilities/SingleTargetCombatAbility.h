// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CombatAbility.h"
#include "SingleTargetCombatAbility.generated.h"

UCLASS()
class ALPHA_API ASingleTargetCombatAbility : public ACombatAbility
{
	GENERATED_BODY()
	
public:	
	ASingleTargetCombatAbility();

	virtual void OnUse() override;

	virtual void ExecuteUse() override;


	//UFUNCTION(BlueprintImplementableEvent)
		//void FireObjectInDirection(FVector InLocation, FRotator InRotation, FVector TraceEnd);
};
