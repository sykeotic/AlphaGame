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

APlayableCharacter* UTeamComponent::SpawnTeamCharacter(FVector SpawnLocation, FRotator SpawnRotation) {
	FActorSpawnParameters SpawnInfo;
	APlayableCharacter* SpawnChar;
	if (OwnedObjectives.Num() > 0) {
		ULogger::ScreenMessage(FColor::Red, "Spawn Char +1 Obj");
		SpawnChar = GetWorld()->SpawnActor<APlayableCharacter>(APlayableCharacter::StaticClass(), OwnedObjectives.Last()->GetActorLocation(), OwnedObjectives.Last()->GetActorRotation(), SpawnInfo);
		SpawnChar->InitCharacterData(FactionData->AvailableHeroes[0]);
	}
	else {
		ULogger::ScreenMessage(FColor::Red, "Spawn Char 0 Obj");
		SpawnChar = GetWorld()->SpawnActor<APlayableCharacter>(APlayableCharacter::StaticClass(), SpawnLocation, SpawnRotation, SpawnInfo);
		if (FactionData && FactionData->AvailableHeroes.Num() > 0) {
			SpawnChar->InitCharacterData(FactionData->AvailableHeroes[0]);
		}
		else {
			SpawnChar->InitCharacterData(nullptr);
		}
	}
	if (SpawnChar) {
		SpawnChar->SetOwnerTeam(this);
		TeamHeroes.Add(SpawnChar);
	}
	return SpawnChar; 
}