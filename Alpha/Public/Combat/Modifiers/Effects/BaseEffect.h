// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Data/Combat/Effects/BaseEffectData.h"
#include "BaseEffect.generated.h"

/**
 * 
 */
UCLASS(Abstract)
class ALPHA_API ABaseEffect : public AActor
{
	GENERATED_BODY()
	
public:

	FBaseEffectDataStruct BaseEffectData;

	virtual void ApplyEffectsToActor(AActor* AffectedActor);

	virtual void AssignValues(UBaseEffectData* InData);
};
