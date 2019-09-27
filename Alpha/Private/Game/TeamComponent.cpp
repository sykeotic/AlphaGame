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

void UTeamComponent::SpawnTeamPawn()
{

}

APlayableCharacter* UTeamComponent::SpawnTeamCharacter(FVector SpawnLocation, FRotator SpawnRotation, uint8 HeroArrayIndex) {
	FActorSpawnParameters SpawnInfo;
	APlayableCharacter* SpawnChar;
	SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	if (GetWorld()) {
		if (OwnedObjectives.Num() > 0) {
			// ULogger::ScreenMessage(FColor::Red, "TeamComponent::SpawnTeamCharacter() - Objectives > 0");
			SpawnChar = GetWorld()->SpawnActor<APlayableCharacter>(APlayableCharacter::StaticClass(), OwnedObjectives.Last()->GetActorLocation(), OwnedObjectives.Last()->GetActorRotation(), SpawnInfo);
			SpawnChar->InitCharacterData(FactionData->AvailableHeroes[HeroArrayIndex]);
		}
		else {
			SpawnChar = GetWorld()->SpawnActor<APlayableCharacter>(APlayableCharacter::StaticClass(), SpawnLocation, SpawnRotation, SpawnInfo);
			// ULogger::ScreenMessage(FColor::Red, "TeamComponent::SpawnTeamCharacter() - Objectives == 0");
			if (FactionData && FactionData->AvailableHeroes.Num() > 0) {
				// ULogger::ScreenMessage(FColor::Red, "TeamComponent::SpawnTeamCharacter() - Avail Heroes > 0");
				SpawnChar->InitCharacterData(FactionData->AvailableHeroes[HeroArrayIndex]);
			}
			else {
				// ULogger::ScreenMessage(FColor::Red, "TeamComponent::SpawnTeamCharacter() - Avail Heroes == 0");
				SpawnChar->InitCharacterData(nullptr);
			}
		}
		if (SpawnChar) {
			SpawnChar->SetOwnerTeam(this);
			TeamHeroes.Add(SpawnChar);
			// ULogger::ScreenMessage(FColor::Red, "TeamComponent::SpawnTeamCharacter() - Spawn Char valid");
		}
		else {
			// ULogger::ScreenMessage(FColor::Red, "TeamComponent::SpawnTeamCharacter() - Spawn Char not valid");
		}
		return SpawnChar;
	}
	else {
		// ULogger::ScreenMessage(FColor::Red, "TeamComponent::SpawnTeamCharacter() - World not valid");
	}
	return nullptr;
}

void UTeamComponent::SetFactionData(UFactionData* InData)
{
	FactionData = InData;
}
