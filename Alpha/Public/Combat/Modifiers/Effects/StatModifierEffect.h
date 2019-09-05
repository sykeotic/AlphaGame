// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Combat/Modifiers/Effects/BaseEffect.h"
#include "StatModifierEffectData.h"
#include "StatModifierEffect.generated.h"

/**
 * 
 */
UCLASS()
class ALPHA_API AStatModifierEffect : public ABaseEffect
{
	GENERATED_BODY()

public:

	virtual void ApplyEffectsToActor(AActor* AffectedActor) override;

	virtual void AssignValues(UBaseEffectData* InData) override;

	FStatModifierEffectDataStruct StatModifierData;
	
};
