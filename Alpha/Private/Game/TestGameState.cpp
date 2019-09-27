#include "TestGameState.h"
#include "Logger.h"
#include "TestGameMode.h"
#include "PlayableCharacter.h"
#include "AIController.h"
#include "EngineUtils.h"
#include "ObjectiveOverlapActor.h"
#include "TeamComponent.h"
#include "UnrealNetwork.h"
#include "PlayableGameInstance.h"

ATestGameState::ATestGameState() {

}

void ATestGameState::BeginPlay() {

}

void ATestGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const {
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ATestGameState, ActiveObjectives);
	DOREPLIFETIME(ATestGameState, ActiveTeams);
}

void ATestGameState::ObjectiveCaptured(UTeamComponent* InTeam, AObjectiveOverlapActor* InObjective) {
	if (!InObjective)
		ULogger::ScreenMessage(FColor::Red, "Obj Not Valid");
	if (InTeam == NULL || InTeam == nullptr || !InTeam->IsValidLowLevel())
		ULogger::ScreenMessage(FColor::Red, "Team Not Valid");
	else {
		InTeam->OwnedObjectives.AddUnique(InObjective);
		ULogger::ScreenMessage(FColor::Red, InTeam->TeamName + " Objectives: " + FString::FromInt(ActiveObjectives.Num()));
		ULogger::ScreenMessage(FColor::Red, InTeam->TeamName + " Objectives: " + FString::FromInt(InTeam->OwnedObjectives.Num()));
		if (InTeam->OwnedObjectives.Num() >= ActiveObjectives.Num()) {
			DisplayEndGameWidget(InTeam->TeamName);
		}
	}
}