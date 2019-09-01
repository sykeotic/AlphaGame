// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseCombatActorData.h"
#include "RangedCombatActorData.generated.h"

USTRUCT(BlueprintType)
struct FRangedCombatActorDataStruct {
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float ExecutionDelay;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FName ProjectileSpawnSocketLocation;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite)
		//TSubclassOf<Projectile>
};

/**
 * 
 */
UCLASS(BlueprintType, DefaultToInstanced, EditInlineNew)
class ALPHA_API URangedCombatActorData : public UBaseCombatActorData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FRangedCombatActorDataStruct RangedCombatActorDataStruct;

};
