#include "AI/OverlordAIComponent.h"
#include "BattlefieldAIController.h"
#include "TimerManager.h"
#include "Game/Modes/Battlefield/BattlefieldGameState.h"
#include "Actors/ObjectiveOverlapActor.h"
#include "AI/CaptainAIComponent.h"

UOverlordAIComponent::UOverlordAIComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UOverlordAIComponent::Init(TArray<ABattlefieldAIController*> InTroops)
{
	int NumOfCaptains = 1;
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
	OverlordPulse();
}

void UOverlordAIComponent::SetAssignedTeam(UTeamComponent* InTeam)
{
	AssignedTeam = InTeam;
}

void UOverlordAIComponent::OverlordPulse()
{
	for (ACaptainAI* CurrentCaptain : AssignedCaptains) {
		
	}

	GetWorld()->GetTimerManager().SetTimer(PulseHandler, this, &UOverlordAIComponent::OverlordPulse, .1f, true);
}

void UOverlordAIComponent::BeginPlay()
{
	Super::BeginPlay();
}