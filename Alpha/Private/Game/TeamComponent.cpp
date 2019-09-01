#include "TeamComponent.h"
#include "Logger.h"
#include "PlayableCharacter.h"

UTeamComponent::UTeamComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UTeamComponent::BeginPlay()
{
	Super::BeginPlay();	
}

APlayableCharacter* UTeamComponent::SpawnTeamCharacter(TSubclassOf<class APlayableCharacter> CharClass, FVector SpawnLocation, FRotator SpawnRotation) {
	FActorSpawnParameters SpawnInfo;
	APlayableCharacter* SpawnChar;
	if (OwnedObjectives.Num() > 0) {
		ULogger::ScreenMessage(FColor::Red, "Spawn Char +1 Obj");
		SpawnChar = GetWorld()->SpawnActor<APlayableCharacter>(CharClass, OwnedObjectives.Last()->GetActorLocation(), OwnedObjectives.Last()->GetActorRotation(), SpawnInfo);
	}
	else {
		ULogger::ScreenMessage(FColor::Red, "Spawn Char 0 Obj");
		SpawnChar = GetWorld()->SpawnActor<APlayableCharacter>(CharClass, SpawnLocation, SpawnRotation, SpawnInfo);
	}
	SpawnChar->SetOwnerTeam(this);
	// SpawnChar->InitCharacterData();
	TeamHeroes.Add(SpawnChar);
	return SpawnChar; 
}