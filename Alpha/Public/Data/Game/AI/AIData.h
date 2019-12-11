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
		float SightRadius;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float LoseSightRadius;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float PeripheralVisionAngleDegrees;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool DetectEnemies;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool DetectAllies;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool DetectNeutrals;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FName BlackboardEnemyKey;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FName BlackboardCanSeeEnemyKey;
};
