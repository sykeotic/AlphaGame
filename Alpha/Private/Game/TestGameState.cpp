#include "TestGameState.h"
#include "Logger.h"
#include "TeamComponent.h"
#include "UnrealNetwork.h"

ATestGameState::ATestGameState() {

}

void ATestGameState::BeginPlay() {
	UTeamComponent* AITeam = CreateDefaultSubobject<UTeamComponent>(TEXT("AITeam"));
	UTeamComponent* PlayerTeam = CreateDefaultSubobject<UTeamComponent>(TEXT("PlayerTeam"));
	UTeamComponent* NeutralTeam = CreateDefaultSubobject<UTeamComponent>(TEXT("NeutralTeam"));
	ActiveTeams.AddUnique(AITeam);
	ActiveTeams.AddUnique(PlayerTeam);
	ActiveTeams.AddUnique(NeutralTeam);
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