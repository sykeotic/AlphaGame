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
	UTeamComponent* NeutralTeam = NewObject<UTeamComponent>(this, UTeamComponent::StaticClass());
	NeutralTeam->TeamName = "Neutral Team";
	NeutralTeam->TeamIndex = 0;
	UTeamComponent* DwarfTeam = NewObject<UTeamComponent>(this, UTeamComponent::StaticClass());
	DwarfTeam->TeamName = "Dwarf Team";
	DwarfTeam->TeamIndex = 1;
	UTeamComponent* HumanTeam = NewObject<UTeamComponent>(this, UTeamComponent::StaticClass());
	HumanTeam->TeamName = "Human Team";
	HumanTeam->TeamIndex = 2;
	ActiveTeams.AddUnique(NeutralTeam);
	ActiveTeams.AddUnique(DwarfTeam);
	ActiveTeams.AddUnique(HumanTeam);

	if (ActiveTeams.Num() == 3 && ActiveTeams[0] && ActiveTeams[1] && ActiveTeams[2]) {
		for (FConstPlayerControllerIterator Iterator = GetWorld()->GetPlayerControllerIterator(); Iterator; ++Iterator)
		{
			AHumanPlayerController* PlayerController = Cast<AHumanPlayerController>(*Iterator);
			UPlayableGameInstance* GameInstance = Cast<UPlayableGameInstance>(PlayerController->GetGameInstance());
			if (PlayerController && GameInstance)
			{
				PlayerController->ControllerTeam = ActiveTeams[GameInstance->CurrentTeamIndex];
				if (GameInstance->Role == EPlayerType::GENERAL) {
					PlayerController->GeneralSelect();
				}
				else if (GameInstance->Role == EPlayerType::HERO) {
					PlayerController->ShowHeroSelectWidget();
				}
			}
		}
		for (TActorIterator<AObjectiveOverlapActor> ObjectiveActorIter(GetWorld()); ObjectiveActorIter; ++ObjectiveActorIter)
		{
			AObjectiveOverlapActor* CurrObj = *ObjectiveActorIter;
			ActiveObjectives.AddUnique(CurrObj);
			CurrObj->OwningTeam = ActiveTeams[0];
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