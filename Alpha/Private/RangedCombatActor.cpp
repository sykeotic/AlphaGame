#include "RangedCombatActor.h"
#include "Logger.h"

void ARangedCombatActor::OnUse()
{
	Super::OnUse();
}

void ARangedCombatActor::ExecuteUse()
{
	ULogger::ScreenMessage(FColor::Red, "Firing Ranged Weapon");
}

void ARangedCombatActor::AssignValues(URangedCombatActorData* InData)
{
	Super::AssignValues(InData);
	RangedDataStruct = InData->RangedCombatActorDataStruct;
}
