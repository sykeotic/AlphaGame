// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "BaseEffectData.generated.h"

USTRUCT(BlueprintType)
struct FBaseEffectDataStruct {
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<class ABaseEffect> EffectClass;
};

/**
 * 
 */
UCLASS(Abstract)
class ALPHA_API UBaseEffectData : public UDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FBaseEffectDataStruct BaseEffectDataStruct;
};
