// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "GameFramework/GameState.h"
#include "TeamComponent.h"
#include "UnrealNetwork.h"
#include "ObjectiveOverlapActor.h"
#include "TestGameState.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class ALPHA_API ATestGameState : public AGameState
{
	GENERATED_BODY()

public:

	ATestGameState();

	virtual void BeginPlay() override;

	UPROPERTY(Replicated)
	TArray<UTeamComponent*> ActiveTeams;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class UTeamComponent> DefaultPlayerTeam;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class UTeamComponent> DefaultAITeam;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class UTeamComponent> DefaultNeutralTeam;

	UPROPERTY(Replicated)
	TArray<AObjectiveOverlapActor*> ActiveObjectives;

	bool bTeamOwnsAllObjectives;

	UFUNCTION(BlueprintImplementableEvent)
	void DisplayEndGameWidget(const FString& TeamName);

	void ObjectiveCaptured(UTeamComponent* InTeam, AObjectiveOverlapActor* InObjective);
};
