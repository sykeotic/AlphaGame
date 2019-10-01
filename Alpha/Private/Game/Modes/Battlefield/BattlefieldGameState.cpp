#include "Game/Modes/Battlefield/BattlefieldGameState.h"
#include "UnrealNetwork.h"
#include "Logger.h"
#include "TeamComponent.h"
#include "EngineUtils.h"


ABattlefieldGameState::ABattlefieldGameState()
{

}

void ABattlefieldGameState::BeginPlay()
{

}

void ABattlefieldGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const {
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ABattlefieldGameState, ActiveObjectives);
	DOREPLIFETIME(ABattlefieldGameState, ActiveTeams);
}

void ABattlefieldGameState::AddTeam(UTeamComponent* NewTeam)
{
	ActiveTeams.Add(NewTeam);
}

UTeamComponent* ABattlefieldGameState::FindTeamForPlayer(int32 PlayerID)
{
	for (UTeamComponent* CurrTeam : ActiveTeams) {
		if (CurrTeam->CurrentPlayerData.Contains(PlayerID)) {
			return CurrTeam;
		}
		else {

		}
	}
	return nullptr;
}

void ABattlefieldGameState::ObjectiveCaptured(UTeamComponent* InTeam, AObjectiveOverlapActor* InObjective) {
	if (!InObjective)
		ULogger::ScreenMessage(FColor::Red, "Obj Not Valid");
	if (InTeam == NULL || InTeam == nullptr || !InTeam->IsValidLowLevel())
		ULogger::ScreenMessage(FColor::Red, "Team Not Valid");
	else {
		InTeam->OwnedObjectives.AddUnique(InObjective);
		ULogger::ScreenMessage(FColor::Red, InTeam->TeamName + " Objectives: " + FString::FromInt(ActiveObjectives.Num()));
		ULogger::ScreenMessage(FColor::Red, InTeam->TeamName + " Objectives: " + FString::FromInt(InTeam->OwnedObjectives.Num()));
		if (InTeam->OwnedObjectives.Num() >= ActiveObjectives.Num()) {
			ULogger::ScreenMessage(FColor::Red, "YOUR TEAM WON WOOHOO");
		}
	}
}