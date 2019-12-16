// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "BattlefieldGameState.generated.h"

class UTeamComponent;
class AObjectiveOverlapActor;

UCLASS(Blueprintable)
class ALPHA_API ABattlefieldGameState : public AGameState
{
	GENERATED_BODY()

public:

	ABattlefieldGameState();

	void AddTeam(UTeamComponent* NewTeam);
	UTeamComponent* GetTeamFromID(int32 InIndex);

	TArray<AObjectiveOverlapActor*> GetAllObjectives();

	UTeamComponent* FindTeamForPlayer(int32 PlayerID);
	void ObjectiveCaptured(UTeamComponent* InTeam, AObjectiveOverlapActor* InObjective);

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(Replicated)
		TArray<UTeamComponent*> ActiveTeams;

	UPROPERTY(Replicated)
		TArray<AObjectiveOverlapActor*> ActiveObjectives;
	
};
