// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseExpression.h"
#include "BranchOperator.generated.h"

USTRUCT(BlueprintType)
struct FBranchExpression
{
	GENERATED_BODY()

public:
	UPROPERTY(Instanced, EditAnywhere, BlueprintReadWrite)
		UBaseExpression* LeftNode;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		EBranchOperator Operator;
	UPROPERTY(Instanced, EditAnywhere, BlueprintReadWrite)
		UBaseExpression* RightNode;
};

/**
 * 
 */
UCLASS(BlueprintType, DefaultToInstanced, EditInlineNew)
class ALPHA_API UBranchOperator : public UBaseExpression
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FBranchExpression Operator;

	bool Evaluate() override;	
};
