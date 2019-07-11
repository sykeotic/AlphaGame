#include "TestGameState.h"
#include "Logger.h"
#include "UnrealNetwork.h"

void ATestGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const {
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ATestGameState, ActiveObjectives);
	DOREPLIFETIME(ATestGameState, ActiveTeams);
}void ATestGameState::ObjectiveCaptured(UTeamComponent* InTeam, AObjectiveOverlapActor* InObjective) {
	InTeam->OwnedObjectives.AddUnique(InObjective);	InObjective->OwningTeam = InTeam;	if (InTeam->OwnedObjectives.Num() >= ActiveObjectives.Num()) {		ULogger::ScreenMessage(FColor::Magenta, "Some Team Won!");	}}