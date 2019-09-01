// Fill out your copyright notice in the Description page of Project Settings.


#include "FloatLeafOperator.h"

bool UFloatLeafOperator::Evaluate() {
	switch (Expression.Operator)
	{
	case ELeafOperator::EQUALS:
		return TargetAndPropertyToFloat(Expression.LeftTarget, Expression.LeftProperty) == DiscreteFloatOperandToFloat(Expression.RightValue);
		break;
	case ELeafOperator::NOT_EQUAL:
		return TargetAndPropertyToFloat(Expression.LeftTarget, Expression.LeftProperty) != DiscreteFloatOperandToFloat(Expression.RightValue);
		break;
	case ELeafOperator::GREATER_THAN:
		return TargetAndPropertyToFloat(Expression.LeftTarget, Expression.LeftProperty) > DiscreteFloatOperandToFloat(Expression.RightValue);
		break;
	case ELeafOperator::GREATER_THAN_OR_EQUAL_TO:
		return TargetAndPropertyToFloat(Expression.LeftTarget, Expression.LeftProperty) >= DiscreteFloatOperandToFloat(Expression.RightValue);
		break;
	case ELeafOperator::LESS_THAN:
		return TargetAndPropertyToFloat(Expression.LeftTarget, Expression.LeftProperty) < DiscreteFloatOperandToFloat(Expression.RightValue);
		break;
	case ELeafOperator::LESS_THAN_OR_EQUAL_TO:
		return TargetAndPropertyToFloat(Expression.LeftTarget, Expression.LeftProperty) <= DiscreteFloatOperandToFloat(Expression.RightValue);
		break;
	default:
		break;
	}
	return false;
}