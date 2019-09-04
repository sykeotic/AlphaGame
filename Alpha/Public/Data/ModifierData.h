// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "ModifierData.generated.h"

class UFeedback;
class AModifier;

USTRUCT(BlueprintType)
struct FModifierDataStruct {
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UFeedback* Feedback;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<AModifier> ModifierClass;
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
