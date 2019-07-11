// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/NonPlayableActors/OverlapTriggerActor.h"
#include "TeamComponent.h"
#include "ObjectiveOverlapActor.generated.h"

/**
 * 
 */
UCLASS()
class ALPHA_API AObjectiveOverlapActor : public AOverlapTriggerActor
{
	GENERATED_BODY()

public:
	UTeamComponent* OwningTeam;	
};
