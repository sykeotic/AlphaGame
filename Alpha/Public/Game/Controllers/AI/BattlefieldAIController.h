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

/**
 * 
 */
UCLASS()
class ALPHA_API ABattlefieldAIController : public AAIController
{
	GENERATED_BODY()
	
public:

	ABattlefieldAIController();

	void AssignData(UAIData* InData);
	AActor* GetSeeingPawn();

	UFUNCTION(BlueprintCallable)
		void OnPerceptionUpdated(TArray<AActor*> UpdatedActors);

protected:

	virtual void BeginPlay() override;

private:
	FName BlackboardEnemyKey;

	UBehaviorTree* BehaviorTree;
	UBehaviorTreeComponent* BehaviorTreeComponent;
	UBlackboardComponent* Blackboard;
	UAIPerceptionComponent* AIPerceptionComponent;


	UAIData* AIData;
};
