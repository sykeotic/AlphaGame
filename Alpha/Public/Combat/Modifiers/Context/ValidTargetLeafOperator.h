// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Combat/Modifiers/Context/LeafOperator.h"
#include "ValidTargetLeafOperator.generated.h"

USTRUCT(BlueprintType)
struct FValidTargetExpression
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		ELeafOperator Operator;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		EValidTargetsOperand RightValue;
};

/**
 * 
 */
UCLASS(BlueprintType, DefaultToInstanced, EditInlineNew)
class ALPHA_API UValidTargetLeafOperator : public ULeafOperator
{
	GENERATED_BODY()

public:
	virtual bool Evaluate() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FValidTargetExpression Expression;
	
};
