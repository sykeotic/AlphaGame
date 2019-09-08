// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Info.h"
#include "Engine/DataTable.h"
#include "BaseExpression.generated.h"

UENUM(BlueprintType)
enum class ETargetOperand : uint8 {
	SELF UMETA(DisplayName = "Self")
};

UENUM(BlueprintType)
enum class EFloatProperty : uint8 {
	CURRENT_HEALTH_PERCENT UMETA(DisplayName = "Current Health %"),
	CURRENT_MOVESPEED UMETA(DisplayName = "Current Movespeed")
};

UENUM(BlueprintType)
enum class EBranchOperator : uint8 {
	AND UMETA(DisplayName = "AND"),
	OR UMETA(DisplayName = "OR"),
	AND_NOT UMETA(DisplayName = "AND NOT")
};

UENUM(BlueprintType)
enum class ELeafOperator : uint8 {
	EQUALS UMETA(DisplayName = "Is"),
	NOT_EQUAL UMETA(DisplayName = "Is Not"),
	GREATER_THAN UMETA(DisplayName = "Greater Than"),
	GREATER_THAN_OR_EQUAL_TO UMETA(DisplayName = "Greater Than Or Equal To"),
	LESS_THAN UMETA(DisplayName = "Less Than"),
	LESS_THAN_OR_EQUAL_TO UMETA(DisplayName = "Less Than Or Equal To")
};

UENUM(BlueprintType)
enum class EDiscreteFloatValue : uint8 {
	FIVE_PERCENT UMETA(DisplayName = "5%"),
	TEN_PERCENT UMETA(DisplayName = "10%"),
	TWENTY_FIVE_PERCENT UMETA(DisplayName = "25%"),
	FIFTY_PERCENT UMETA(DisplayName = "50%"),
	SEVENTY_FIVE_PERCENT UMETA(DisplayName = "75%"),
	ONE_HUNDRED_PERCENT UMETA(DisplayName = "100%")
};

class UConditionTree;



/**
 * 
 */
UCLASS(Abstract)
class ALPHA_API UBaseExpression : public UObject
{
	GENERATED_BODY()

public:
	virtual bool Evaluate();

	UConditionTree* GetOwnerConditionTree();
	void SetOwnerConditionTree(UConditionTree* InTree);

protected:

	float DiscreteFloatOperandToFloat(EDiscreteFloatValue InValue);
	float TargetAndPropertyToFloat(ETargetOperand Target, EFloatProperty InProperty);

private:

	UConditionTree* OwnerConditionTree;
};
