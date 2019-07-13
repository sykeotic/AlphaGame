// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "TeamComponent.h"
#include "UnrealNetwork.h"
#include "ObjectiveOverlapActor.h"
#include "TestGameState.generated.h"

/**
 * 
 */
UCLASS()
class ALPHA_API ATestGameState : public AGameState
{
	GENERATED_BODY()

public:

	ATestGameState();

	virtual void BeginPlay() override;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite)
	TArray<UTeamComponent*> ActiveTeams;

	UPROPERTY(Replicated)
	TArray<AObjectiveOverlapActor*> ActiveObjectives;

	bool bTeamOwnsAllObjectives;

	void ObjectiveCaptured(UTeamComponent* InTeam, AObjectiveOverlapActor* InObjective);
};
