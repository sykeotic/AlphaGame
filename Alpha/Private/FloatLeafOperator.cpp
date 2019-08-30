// Fill out your copyright notice in the Description page of Project Settings.


#include "FloatLeafOperator.h"

bool UFloatLeafOperator::Evaluate() {
	switch (Expression.Operator)
	{
	case ELeafOperator::EQUALS:
		return Expression.LeftOperand == Expression.RightOperand;
		break;
	case ELeafOperator::NOT_EQUAL:
		return Expression.LeftOperand != Expression.RightOperand;
		break;
	case ELeafOperator::GREATER_THAN:
		return Expression.LeftOperand > Expression.RightOperand;
		break;
	case ELeafOperator::GREATER_THAN_OR_EQUAL_TO:
		return Expression.LeftOperand >= Expression.RightOperand;
		break;
	case ELeafOperator::LESS_THAN:
		return Expression.LeftOperand < Expression.RightOperand;
		break;
	case ELeafOperator::LESS_THAN_OR_EQUAL_TO:
		return Expression.LeftOperand <= Expression.RightOperand;
		break;
	default:
		break;
	}
	return false;
}