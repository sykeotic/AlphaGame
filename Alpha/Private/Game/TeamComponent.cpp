#include "TeamComponent.h"
#include "PlayableCharacter.h"

UTeamComponent::UTeamComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UTeamComponent::BeginPlay()
{
	Super::BeginPlay();	
}

void UTeamComponent::SpawnTeamCharacter(TSubclassOf<class APlayableCharacter> CharClass, FVector SpawnLocation, FRotator SpawnRotation) {
	FActorSpawnParameters SpawnInfo;
	APlayableCharacter* SpawnChar = GetWorld()->SpawnActor<APlayableCharacter>(CharClass, SpawnLocation, SpawnRotation, SpawnInfo);
	TeamHeroes.Add(SpawnChar);
}