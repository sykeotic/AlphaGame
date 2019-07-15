#include "TestGameState.h"
#include "Logger.h"
#include "TestGameMode.h"
#include "PlayableCharacter.h"
#include "EngineUtils.h"
#include "ObjectiveOverlapActor.h"
#include "TeamComponent.h"
#include "UnrealNetwork.h"

ATestGameState::ATestGameState() {

}

void ATestGameState::BeginPlay() {
	UTeamComponent* AITeam = NewObject<UTeamComponent>(this, DefaultTeam);
	UTeamComponent* PlayerTeam = NewObject<UTeamComponent>(this, DefaultTeam);
	UTeamComponent* NeutralTeam = NewObject<UTeamComponent>(this, DefaultTeam);
	ActiveTeams.AddUnique(AITeam);
	ActiveTeams.AddUnique(PlayerTeam);
	ActiveTeams.AddUnique(NeutralTeam);

	if (ActiveTeams.Num() > 2 && ActiveTeams[0] && ActiveTeams[1] && ActiveTeams[2]) {
		for (FConstPlayerControllerIterator Iterator = GetWorld()->GetPlayerControllerIterator(); Iterator; ++Iterator)
		{
			AHumanPlayerController* PlayerController = Cast<AHumanPlayerController>(*Iterator);
			if (PlayerController)
			{
				PlayerController->ControllerTeam = ActiveTeams[1];
			}
		}

		for (TActorIterator<AObjectiveOverlapActor> ObjectiveActorIter(GetWorld()); ObjectiveActorIter; ++ObjectiveActorIter)
		{
			AObjectiveOverlapActor* CurrObj = *ObjectiveActorIter;
			ActiveObjectives.AddUnique(CurrObj);
			CurrObj->OwningTeam = ActiveTeams[2];
		}

		for (TActorIterator<APlayableCharacter> ObjectiveActorIter(GetWorld()); ObjectiveActorIter; ++ObjectiveActorIter)
		{
			APlayableCharacter* CurrObj = *ObjectiveActorIter;
			ActiveTeams[0]->TeamHeroes.Add(CurrObj);
		}
	}
}

void ATestGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const {
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ATestGameState, ActiveObjectives);
	DOREPLIFETIME(ATestGameState, ActiveTeams);
}

void ATestGameState::ObjectiveCaptured(UTeamComponent* InTeam, AObjectiveOverlapActor* InObjective) {
	if (InTeam == NULL || InTeam == nullptr || !InTeam->IsValidLowLevel())
		ULogger::ScreenMessage(FColor::Red, "Team Not Valid");
	if (!InObjective)
		ULogger::ScreenMessage(FColor::Red, "Obj Not Valid");
	InTeam->OwnedObjectives.AddUnique(InObjective);
	ULogger::ScreenMessage(FColor::Red, "Num Objectives: " + FString::FromInt(ActiveObjectives.Num()));
	ULogger::ScreenMessage(FColor::Red, "Team Objectives: " + FString::FromInt(InTeam->OwnedObjectives.Num()));
	if (InTeam->OwnedObjectives.Num() >= ActiveObjectives.Num()) {
		DisplayEndGameWidget();
	}
}