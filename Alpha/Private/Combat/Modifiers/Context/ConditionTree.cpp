// Fill out your copyright notice in the Description page of Project Settings.
#include "ConditionTree.h"
#include "Logger.h"
#include "BaseExpression.h"

void UConditionTree::InitExpressions()
{
	for (UBaseExpression* CurrExp : ExpressionTree) {
		if (CurrExp) {
			CurrExp->SetOwnerConditionTree(this);
		}
	}
}

bool UConditionTree::IsConditionTreeTrue()
{
	if (ExpressionTree.Num() > 0) {
		for (UBaseExpression* CurrExp : ExpressionTree) {
			if (CurrExp && CurrExp->Evaluate() == false) {
				return false;
			}
		}
		return true;
	}
	else {
		return true;
	}
}

AModifier* UConditionTree::GetModifierOwner()
{
	return ModifierOwner;
}

void UConditionTree::SetModifierOwner(AModifier* InModifier)
{
	ModifierOwner = InModifier;
}
