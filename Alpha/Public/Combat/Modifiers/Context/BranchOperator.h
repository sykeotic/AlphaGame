// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseExpression.h"
#include "BranchOperator.generated.h"

UENUM(BlueprintType)
enum class EBranchOperator : uint8 {
	AND UMETA(DisplayName = "AND"),
	OR UMETA(DisplayName = "OR"),
	AND_NOT UMETA(DisplayName = "AND NOT")
};

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
