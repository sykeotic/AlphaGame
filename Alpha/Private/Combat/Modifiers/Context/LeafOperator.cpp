#include "LeafOperator.h"
#include "Logger.h"

bool ULeafOperator::Evaluate(){
	ULogger::ScreenMessage(FColor::Red, "LEAF OPERATOR: Default False");
	return false;
}