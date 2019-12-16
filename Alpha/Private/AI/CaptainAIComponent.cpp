#include "AI/CaptainAIComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Actors/ObjectiveOverlapActor.h"
#include "BattlefieldAIController.h"

ACaptainAI::ACaptainAI()
{
	
}

void ACaptainAI::InitCaptain(TArray<ABattlefieldAIController*> InTroops)
{
	AssignedTroops = InTroops;
}

void ACaptainAI::SetAssignedTroops(TArray<ABattlefieldAIController*> InTroops)
{
	AssignedTroops = InTroops;
}

TArray<ABattlefieldAIController*> ACaptainAI::GetAssignedTroops()
{
	return AssignedTroops;
}

AObjectiveOverlapActor* ACaptainAI::GetCurrentGoal()
{
	return CurrentGoal;
}

void ACaptainAI::SetGoal(AObjectiveOverlapActor* InGoal)
{
	CurrentGoal = InGoal;
}
