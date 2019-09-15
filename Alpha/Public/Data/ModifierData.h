// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Data/Combat/Effects/BaseEffectData.h"
#include "ModifierData.generated.h"

class UFeedback;
class AModifier;
class UConditionTree;
class UBaseEffectData;

USTRUCT(BlueprintType)
struct FModifierDataStruct {
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UFeedback* Feedback;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<AModifier> ModifierClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool bHasDuration;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float Duration;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UConditionTree* Conditions;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<UBaseEffectData*> BaseEffectData;
};

/**
 * 
 */
UCLASS(BlueprintType, DefaultToInstanced, EditInlineNew)
class ALPHA_API UModifierData : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FModifierDataStruct ModifierData;	
};