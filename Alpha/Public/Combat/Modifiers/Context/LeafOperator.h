// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseExpression.h"
#include "LeafOperator.generated.h"

UENUM(BlueprintType)
enum class ELeafOperator : uint8 {
	EQUALS UMETA(DisplayName = "Is"),
	NOT_EQUAL UMETA(DisplayName = "Is Not"),
	GREATER_THAN UMETA(DisplayName = "Greater Than"),
	GREATER_THAN_OR_EQUAL_TO UMETA(DisplayName = "Greater Than Or Equal To"),
	LESS_THAN UMETA(DisplayName = "Less Than"),
	LESS_THAN_OR_EQUAL_TO UMETA(DisplayName = "Less Than Or Equal To")
};

/**
 * 
 */
UCLASS(Abstract, BlueprintType)
class ALPHA_API ULeafOperator : public UBaseExpression
{
	GENERATED_BODY()

public:

	virtual bool Evaluate() override;
};
