#include "BaseExpression.h"
#include "ConditionTree.h"
#include "StatsComponent.h"
#include "Logger.h"
#include "Modifier.h"

bool UBaseExpression::Evaluate()
{
	return false;
}

float UBaseExpression::DiscreteFloatOperandToFloat(EDiscreteFloatValue InValue) {
	switch (InValue)
	{
	case EDiscreteFloatValue::FIVE_PERCENT:
		return .05f;
		break;
	case EDiscreteFloatValue::TEN_PERCENT:
		return .10f;
		break;
	case EDiscreteFloatValue::TWENTY_FIVE_PERCENT:
		return .25f;
		break;
	case EDiscreteFloatValue::FIFTY_PERCENT:
		return .50f;
		break;
	case EDiscreteFloatValue::SEVENTY_FIVE_PERCENT:
		return .75f;
		break;
	case EDiscreteFloatValue::ONE_HUNDRED_PERCENT:
		return 1.0f;
		break;
	default:
		break;
	}
	return 0.0f;
}

float UBaseExpression::TargetAndPropertyToFloat(ETargetOperand Target, EFloatProperty InProperty) {
	AActor* ActorTarget = nullptr;
	switch (Target)	
	{
	case ETargetOperand::SELF:
		ActorTarget = OwnerConditionTree->GetModifierOwner()->GetActorOwner();
		break;
	default:
		break;
	}
	UStatsComponent* TempStats = nullptr;
	if (ActorTarget) {
		switch (InProperty)
		{
		case EFloatProperty::CURRENT_HEALTH_PERCENT:
			TempStats = ActorTarget->FindComponentByClass<UStatsComponent>();
			return TempStats->GetCurrentHealth() / TempStats->GetMaxHealth();
			break;
		case EFloatProperty::CURRENT_MOVESPEED:
			return FVector::DotProduct(ActorTarget->GetVelocity(), ActorTarget->GetActorRotation().Vector());
			break;
		default:
			break;
		}
	}
	return 0.0f;
}

UConditionTree* UBaseExpression::GetOwnerConditionTree()
{
	return OwnerConditionTree;
}

void UBaseExpression::SetOwnerConditionTree(UConditionTree* InTree)
{
	OwnerConditionTree = InTree;
}
