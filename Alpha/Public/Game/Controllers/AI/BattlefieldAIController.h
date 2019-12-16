// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "BattlefieldAIController.generated.h"

class UBehaviorTreeComponent;
class UAIPerceptionComponent;
class UBehaviorTree;
class UBlackboardComponent;
class UAISenseConfig_Sight;
class UAIData;
class ATextRenderActor;
class APlayableCharacter;
class UTeamComponent;


/**
 * 
 */
UCLASS()
class ALPHA_API ABattlefieldAIController : public AAIController
{
	GENERATED_BODY()
	
public:

	ABattlefieldAIController();

	void SetAICharacter(APlayableCharacter* InChar);
	APlayableCharacter* GetAICharacter();

	void SetControllerTeam(UTeamComponent* InTeam);
	UTeamComponent* GetControllerTeam();

	void AssignData(UAIData* InData);
	AActor* GetSeeingPawn();

	void HandlePawnDeath();

	void JumpRandomly();

	void SpawnTextActor();
	void EngageBrainPulseLoop();
	void SetPossessed(bool inPossessed);

	UFUNCTION(BlueprintCallable)
		void OnPerceptionUpdated(const TArray<AActor*>& UpdatedActors);

protected:

	virtual void BeginPlay() override;

private:

	bool bInCombat;
	bool bPossessed;

	TArray<AActor*> VisibleEnemies;

	FName BlackboardEnemyKey;
	FName BlackboardCanSeeEnemyKey;

	UTeamComponent* ControllerTeam;

	ATextRenderActor* AIStatusText;

	UBehaviorTree* BehaviorTree;
	UBehaviorTreeComponent* BehaviorTreeComponent;
	UBlackboardComponent* Blackboard;
	UAIPerceptionComponent* AIPerceptionComponent;
	UAISenseConfig_Sight* Sight;

	APlayableCharacter* AICharacter;

	FTimerHandle PulseHandler;

	UAIData* AIData;

	void ExecuteJump();
	void SpawnNewPawn();

	void BrainPulse();
	void NonCombatPulse();
	void CombatPulse();

};
