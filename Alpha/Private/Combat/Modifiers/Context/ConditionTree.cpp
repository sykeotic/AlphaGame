// Fill out your copyright notice in the Description page of Project Settings.
#include "ConditionTree.h"
#include "Logger.h"
#include "BaseExpression.h"

bool UConditionTree::IsConditionTreeTrue()
{
	for (UBaseExpression* CurrExp : ExpressionTree) {
		if (CurrExp->Evaluate() == false) {
			ULogger::ScreenMessage(FColor::Red, "Condition is true");
			return false;
		}
		else {
			ULogger::ScreenMessage(FColor::Red, "Condition is true");
		}
	}
	return true;
}

AModifier* UConditionTree::GetModifierOwner()
{
	return ModifierOwner;
}

void UConditionTree::SetModifierOwner(AModifier* InModifier)
{
	ModifierOwner = InModifier;
}
