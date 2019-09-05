// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Data/Combat/Effects/BaseEffectData.h"
#include "StatModifierEffectData.generated.h"

UENUM(BlueprintType)
enum class EStatModifierType : uint8 {
	HP UMETA(DisplayName = "Health")
};

USTRUCT(BlueprintType)
struct FStatModifierEffectDataStruct {
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float ModifierOperand;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		EStatModifierType Type;
};

/**
 * 
 */
UCLASS(BlueprintType, DefaultToInstanced, EditInlineNew)
class ALPHA_API UStatModifierEffectData : public UBaseEffectData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FStatModifierEffectDataStruct StatModifierEffectDataStruct;
	
};
