// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CombatActor.h"
#include "CombatAbility.generated.h"

UCLASS()
class ALPHA_API ACombatAbility : public ACombatActor
{
	GENERATED_BODY()
	
public:	
	ACombatAbility();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void OnUse() override;
	virtual void ExecuteUse() override;
};
