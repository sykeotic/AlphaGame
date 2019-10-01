// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "AIData.generated.h"

class AAIController;
class UBehaviorTree;
class UBlackboardComponent;
class UAIPerceptionComponent;

UCLASS(BlueprintType)
class ALPHA_API UAIData : public UDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<AAIController> AIControllerClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UBehaviorTree* BehaviorTree;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UBlackboardComponent* Blackboard;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UAIPerceptionComponent* AIPerceptionComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FName BlackboardEnemyKey;
};
