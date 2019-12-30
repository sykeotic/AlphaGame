#include "TeamComponent.h"
#include "Logger.h"
#include "FactionData.h"
#include "TimerManager.h"
#include "Kismet/GameplayStatics.h"
#include "Feedback.h"
#include "ObjectiveOverlapActor.h"
#include "Components/StaticMeshComponent.h"
#include "Game/Modes/Battlefield/BattlefieldGameState.h"
#include "Particles/ParticleSystemComponent.h"
#include "AI/OverlordAIComponent.h"
#include "PlayableCharacter.h"

UTeamComponent::UTeamComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	OverlordComponent = CreateDefaultSubobject<UOverlordAIComponent>(TEXT("OverlordComp"));
}

void UTeamComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UTeamComponent::InitOverlordComponent(TArray<ABattlefieldAIController*> InTroops)
{
	OverlordComponent->Init(InTroops);
}

void UTeamComponent::SpawnTeamPawn()
{

}

void UTeamComponent::FindAllObjectivesForTeam()
{
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AObjectiveOverlapActor::StaticClass(), FoundActors);
	TArray<AObjectiveOverlapActor*> FoundObjectives;
	for (AActor* CurrActor : FoundActors) {
		FoundObjectives.Add(Cast<AObjectiveOverlapActor>(CurrActor));
	}

	for (AObjectiveOverlapActor* CurrObj : FoundObjectives) {
		if (CurrObj->StartingTeam == TeamIndex) {
			CurrObj->OwningTeam = this;
			AddObjective(CurrObj);
			CurrObj->OBJECTIVE_STATE = EObjectiveState::CAPTURED;
			CurrObj->PREV_STATE = EObjectiveState::NEUTRAL;
			CurrObj->SkeletalMeshComp->SetSkeletalMesh(FactionData->FactionFlagMesh);
		}
	}
}

void UTeamComponent::AddObjective(AObjectiveOverlapActor* InObjective)
{
	if(InObjective)
		OwnedObjectives.Add(InObjective);
}

void UTeamComponent::RemoveObjective(AObjectiveOverlapActor* InObjective)
{
	if(InObjective && this)
		OwnedObjectives.Remove(InObjective);
}

APlayableCharacter* UTeamComponent::SpawnTeamCharacter(FVector SpawnLocation, FRotator SpawnRotation, uint8 HeroArrayIndex) {
	FActorSpawnParameters SpawnInfo;
	APlayableCharacter* SpawnChar;
	SpawnLocation = { 238520.0, -204070.0, 7100.0 };
	SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	if (GetWorld()) {
		if (OwnedObjectives.Num() > 0) {
			FVector Loc = OwnedObjectives.Last()->GetActorLocation();
			Loc.X += FMath::RandRange(-200, 200);
			Loc.Y += FMath::RandRange(-200, 200);
			Loc.Z += 100;
			SpawnChar = GetWorld()->SpawnActor<APlayableCharacter>(APlayableCharacter::StaticClass(), Loc, OwnedObjectives.Last()->GetActorRotation(), SpawnInfo);
			
			UE_LOG(LogTemp, Warning, TEXT("TeamComponent::SpawnTeamCharacter - Spawn at objective"));
			SpawnChar->InitCharacterData(FactionData->AvailableHeroes[HeroArrayIndex]);
		}
		else {
			SpawnChar = GetWorld()->SpawnActor<APlayableCharacter>(APlayableCharacter::StaticClass(), SpawnLocation, SpawnRotation, SpawnInfo);
			if (FactionData && FactionData->AvailableHeroes.Num() > 0) {
				UE_LOG(LogTemp, Warning, TEXT("TeamComponent::SpawnTeamCharacter - Char Data not null"));
				if (IsValid(FactionData->AvailableHeroes[HeroArrayIndex])) {
					UE_LOG(LogTemp, Warning, TEXT("TeamComponent::SpawnTeamCharacter - FactionData Hero OK"));
				}
				else {
					UE_LOG(LogTemp, Warning, TEXT("TeamComponent::SpawnTeamCharacter - FactionData Hero NULL"));
				}
				SpawnChar->InitCharacterData(FactionData->AvailableHeroes[HeroArrayIndex]);
			}
			else {
				UE_LOG(LogTemp, Warning, TEXT("TeamComponent::SpawnTeamCharacter - Char Data nullptr"));
				SpawnChar->InitCharacterData(nullptr);
			}
		}
		if (SpawnChar) {
			SpawnChar->SetOwnerTeam(this);
			TeamHeroes.Add(SpawnChar);
		}
		else {
			ULogger::ScreenMessage(FColor::Yellow, "TEAM IS NULL ASSHOLE");
		}
		if (GetWorld()->GetTimeSeconds() > 1) {
			SpawnChar->PlaySpawnFX(SpawnFX->VisualFX, SpawnFX->SoundFX[0]);
		}
		return SpawnChar;
	}
	else {
	}
	return nullptr;
}

void UTeamComponent::SetFactionData(UFactionData* InData)
{
	FactionData = InData;
}
