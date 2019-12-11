#include "AI/OverlordAIComponent.h"
#include "BattlefieldAIController.h"
#include "AI/CaptainAIComponent.h"

UOverlordAIComponent::UOverlordAIComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UOverlordAIComponent::Init(TArray<ABattlefieldAIController*> InTroops)
{
	int NumOfCaptains = 3;
	int NumOfTroopsPerSquad = 6;
	for (int i = 0; i < NumOfCaptains; i++) {
		ACaptainAI* NewCaptain = NewObject<ACaptainAI>(this, ACaptainAI::StaticClass());
		TArray<ABattlefieldAIController*> TroopDivide;
		uint8 TroopsPerSquad = FMath::DivideAndRoundUp(InTroops.Num(), NumOfTroopsPerSquad);
		for (int j = TroopsPerSquad * i; j < (i * TroopsPerSquad) + TroopsPerSquad; j++) {
			TroopDivide.Add(InTroops[j]);
		}
		NewCaptain->InitCaptain(TroopDivide);
		AssignedCaptains.Add(NewCaptain);
	}
}

void UOverlordAIComponent::SetAssignedTeam(UTeamComponent* InTeam)
{
	AssignedTeam = InTeam;
}

void UOverlordAIComponent::BeginPlay()
{
	Super::BeginPlay();
}