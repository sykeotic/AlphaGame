// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Combat/Actors/BaseCombatActor.h"
#include "Data/Combat/ArcCombatActorData.h"
#include "ArcCombatActor.generated.h"

/**
 * 
 */
UCLASS()
class ALPHA_API AArcCombatActor : public ABaseCombatActor
{
	GENERATED_BODY()

public:

	AArcCombatActor();

	FArcCombatActorDataStruct ArcCombatActorDataStruct;

	virtual void OnUse() override;
	virtual void ExecuteUse() override;
	virtual void AssignValues(UBaseCombatActorData* InData) override;

private:

	void ArcSweep();
	
};
