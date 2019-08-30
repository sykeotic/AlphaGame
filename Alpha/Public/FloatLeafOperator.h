// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Combat/Modifiers/Context/LeafOperator.h"
#include "FloatLeafOperator.generated.h"

USTRUCT(BlueprintType)
struct FFloatExpression
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float LeftOperand;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		ELeafOperator Operator;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float RightOperand;
};

/**
 * 
 */
UCLASS(BlueprintType, DefaultToInstanced, EditInlineNew)
class ALPHA_API UFloatLeafOperator : public ULeafOperator
{
	GENERATED_BODY()

public:
	virtual bool Evaluate() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FFloatExpression Expression;
};
