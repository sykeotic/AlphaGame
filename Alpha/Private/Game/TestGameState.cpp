#include "TestGameState.h"
#include "Logger.h"
#include "PlayableCharacter.h"
#include "EngineUtils.h"
#include "ObjectiveOverlapActor.h"
#include "TeamComponent.h"
#include "UnrealNetwork.h"

ATestGameState::ATestGameState() {

}

void ATestGameState::BeginPlay() {
	UTeamComponent* AITeam = NewObject<UTeamComponent>(this);
	UTeamComponent* PlayerTeam = NewObject<UTeamComponent>(this);
	UTeamComponent* NeutralTeam = NewObject<UTeamComponent>(this);
	ActiveTeams.AddUnique(AITeam);
	ActiveTeams.AddUnique(PlayerTeam);
	ActiveTeams.AddUnique(NeutralTeam);

	for (TActorIterator<AObjectiveOverlapActor> ObjectiveActorIter(GetWorld()); ObjectiveActorIter; ++ObjectiveActorIter)
	{
		AObjectiveOverlapActor* CurrObj = *ObjectiveActorIter;
		CurrObj->OwningTeam = ActiveTeams[2];
	}

	for (TActorIterator<APlayableCharacter> ObjectiveActorIter(GetWorld()); ObjectiveActorIter; ++ObjectiveActorIter)
	{
		APlayableCharacter* CurrObj = *ObjectiveActorIter;
		ActiveTeams[0]->TeamHeroes.Add(CurrObj);
	}
}

void ATestGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const {
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ATestGameState, ActiveObjectives);
	DOREPLIFETIME(ATestGameState, ActiveTeams);
}

void ATestGameState::ObjectiveCaptured(UTeamComponent* InTeam, AObjectiveOverlapActor* InObjective) {
	InTeam->OwnedObjectives.AddUnique(InObjective);
	InObjective->OwningTeam = InTeam;
	if (InTeam->OwnedObjectives.Num() >= ActiveObjectives.Num()) {
		ULogger::ScreenMessage(FColor::Magenta, "Some Team Won!");
	}
}