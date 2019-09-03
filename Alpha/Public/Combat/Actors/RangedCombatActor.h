// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Combat/Actors/BaseCombatActor.h"
#include "RangedCombatActorData.h"
#include "RangedCombatActor.generated.h"

/**
 * 
 */
UCLASS()
class ALPHA_API ARangedCombatActor : public ABaseCombatActor
{
	GENERATED_BODY()

public:
	ARangedCombatActor();
	
	FRangedCombatActorDataStruct RangedDataStruct;

	virtual void OnUse() override;
	virtual void ExecuteUse() override;
	virtual void AssignValues(UBaseCombatActorData* InData) override;

private:
	void LaunchProjectile();
	
};
