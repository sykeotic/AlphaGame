// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseCombatActorData.h"
#include "RangedCombatActorData.generated.h"

class ABaseProjectile;
class UProjectileData;

USTRUCT(BlueprintType)
struct FRangedCombatActorDataStruct {
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float ExecutionDelay;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FName ProjectileSpawnAtSocket;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<ABaseProjectile> ProjectileClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UProjectileData* ProjectileData;
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
