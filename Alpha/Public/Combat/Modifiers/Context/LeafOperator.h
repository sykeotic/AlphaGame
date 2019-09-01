// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseExpression.h"
#include "LeafOperator.generated.h"

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
