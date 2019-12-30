// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Data/Combat/Effects/BaseEffectData.h"
#include "BaseEffect.generated.h"

class AModifier;
/**
 * 
 */
UCLASS(Abstract)
class ALPHA_API ABaseEffect : public AActor
{
	GENERATED_BODY()
	
public:

	FBaseEffectDataStruct BaseEffectData;

	virtual void ApplyEffectsToActor(AActor* AffectedActor, bool bPlayFeedback);
	virtual void AssignValues(UBaseEffectData* InData);
	virtual void DeactivateEffect(AActor* AffectedActor);

	void ShowEffectFeedback(AActor* AffectedActor);

	void SetModifierOwner(AModifier* InMod);

protected:
	AModifier* ModifierOwner;

private:


};
