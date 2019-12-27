// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Combat/Modifiers/Effects/BaseEffect.h"
#include "ModifyTagEffectData.h"
#include "ModifyTagEffect.generated.h"

/**
 * 
 */
UCLASS()
class ALPHA_API AModifyTagEffect : public ABaseEffect
{
	GENERATED_BODY()

public:

	virtual void ApplyEffectsToActor(AActor* AffectedActor) override;
	virtual void AssignValues(UBaseEffectData* InData) override;
	virtual void DeactivateEffect(AActor* AffectedActor) override;

	FModifyTagEffectDataStruct ModifyTagData;

private:

	void TauntActor(AActor* AffectedActor);
};
