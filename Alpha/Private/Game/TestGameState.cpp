#include "TestGameState.h"
#include "Logger.h"
#include "TestGameMode.h"
#include "PlayableCharacter.h"
#include "AIController.h"
#include "EngineUtils.h"
#include "ObjectiveOverlapActor.h"
#include "TeamComponent.h"
#include "UnrealNetwork.h"

ATestGameState::ATestGameState() {

}

void ATestGameState::BeginPlay() {
	UTeamComponent* AITeam = NewObject<UTeamComponent>(this, DefaultAITeam);
	AITeam->TeamName = "AI Team";
	AITeam->TeamIndex = 0;
	UTeamComponent* PlayerTeam = NewObject<UTeamComponent>(this, DefaultPlayerTeam);
	PlayerTeam->TeamName = "Player Team";
	PlayerTeam->TeamIndex = 1;
	UTeamComponent* NeutralTeam = NewObject<UTeamComponent>(this, DefaultNeutralTeam);
	NeutralTeam->TeamName = "Neutral Team";
	NeutralTeam->TeamIndex = 2;
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
			else {
				ULogger::ScreenMessage(FColor::Blue, "Player Controller Not Valid");
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
			CurrObj->SetOwnerTeam(AITeam);
			ActiveTeams[0]->TeamHeroes.Add(CurrObj);
		}
	}
	else {
		ULogger::ScreenMessage(FColor::Red, "At least one Team isnt valid");
	}
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