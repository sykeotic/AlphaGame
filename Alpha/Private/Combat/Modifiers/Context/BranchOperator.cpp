#include "BranchOperator.h"

bool UBranchOperator::Evaluate()
{
	switch (Operator.Operator)
	{
	case EBranchOperator::AND:
		return Operator.LeftNode->Evaluate() && Operator.RightNode->Evaluate();
		break;
	case EBranchOperator::OR:
		return Operator.LeftNode->Evaluate() || Operator.RightNode->Evaluate();
		break;
	case EBranchOperator::AND_NOT:
		return Operator.LeftNode->Evaluate() && !Operator.RightNode->Evaluate();
		break;
	default:
		break;
	}
	return false;
}
