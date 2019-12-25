// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Data/Combat/BaseCombatActorData.h"
#include "ArcCombatActorData.generated.h"

class UFeedback;

USTRUCT(BlueprintType)
struct FArcCombatActorDataStruct {

	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float ArcRange;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float ConeSweepRadius;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 NumberOfCones;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float ConeTraceArcWidthInDegrees;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FName ArcSpawnFromSocket;
};

/**
 * 
 */
UCLASS(BlueprintType, DefaultToInstanced, EditInlineNew)
class ALPHA_API UArcCombatActorData : public UBaseCombatActorData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FArcCombatActorDataStruct ArcCombatActorDataStruct;
	
};
