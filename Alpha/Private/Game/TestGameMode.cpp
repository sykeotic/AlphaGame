// Fill out your copyright notice in the Description page of Project Settings.

#include "TestGameMode.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "GameplayUtils.h"
#include "TestGameState.h"
#include "PlayableGameInstance.h"
#include "TimerManager.h"
#include "PlayableCharacter.h"
#include "GameModeData.h"
#include "Logger.h"

ATestGameMode::ATestGameMode() {

}

void ATestGameMode::BeginPlay() {
	Super::BeginPlay();
	ULogger::SetIsTestRun(bDisplayLogs);
	CurrentGameState = Cast<ATestGameState>(GetWorld()->GetGameState());
}

void ATestGameMode::PostLogin(APlayerController* NewPlayer) {
	Super::PostLogin(NewPlayer);
	HumanPlayers.Add(NewPlayer);
}

void ATestGameMode::StartMatch()
{
	Super::StartMatch();
	InitTeams();
}

void ATestGameMode::InitTeams()
{
	UTeamComponent* NeutralTeam = NewObject<UTeamComponent>(this, UTeamComponent::StaticClass());
	NeutralTeam->TeamName = "Neutral Team";
	NeutralTeam->TeamIndex = 0;
	NeutralTeam->FactionData = LoadObject<UFactionData>(nullptr, TEXT("FactionData'/Game/Data/DataAssets/Factions/Dwarves.Dwarves'"));
	CurrentGameState->ActiveTeams.Add(NeutralTeam);
	UTeamComponent* Team_1 = NewObject<UTeamComponent>(this, UTeamComponent::StaticClass());
	Team_1->TeamName = "Team 1";
	Team_1->TeamIndex = 1;
	Team_1->FactionData = LoadObject<UFactionData>(nullptr, TEXT("FactionData'/Game/Data/DataAssets/Factions/Dwarves.Dwarves'"));
	CurrentGameState->ActiveTeams.Add(Team_1);
	UTeamComponent* Team_2 = NewObject<UTeamComponent>(this, UTeamComponent::StaticClass());
	Team_2->TeamName = "Team 2";
	Team_2->TeamIndex = 2;
	Team_2->FactionData = LoadObject<UFactionData>(nullptr, TEXT("FactionData'/Game/Data/DataAssets/Factions/Humans.Humans'"));
	CurrentGameState->ActiveTeams.Add(Team_2);

	if (CurrentGameState->ActiveTeams.Num() == 3 && CurrentGameState->ActiveTeams[0] && CurrentGameState->ActiveTeams[1] && CurrentGameState->ActiveTeams[2]) {
		for (FConstPlayerControllerIterator Iterator = GetWorld()->GetPlayerControllerIterator(); Iterator; ++Iterator)
		{
			AHumanPlayerController* PlayerController = Cast<AHumanPlayerController>(*Iterator);
			UPlayableGameInstance* GameInstance = Cast<UPlayableGameInstance>(PlayerController->GetGameInstance());
			if (PlayerController && GameInstance)
			{
				PlayerController->ControllerTeam = CurrentGameState->ActiveTeams[GameInstance->CurrentTeamIndex];
				PlayerController->PlayerControllerData = GameInstance->GameData->GameInstanceData.PlayerControllerData;
				CurrentGameState->ActiveTeams[GameInstance->CurrentTeamIndex]->PlayerControllerArray.Add(PlayerController);
				if (PlayerController->PlayerControllerData) {
					PlayerController->ControllerTeam->FactionData = GameInstance->SelectedFaction;
				}
				if (GameInstance->Role == EPlayerType::GENERAL) {
					PlayerController->GeneralSelect();
				}
				else if (GameInstance->Role == EPlayerType::HERO) {
					PlayerController->ShowHeroSelectWidget();
				}
			}
		}

		for (uint8 i = 1; i < CurrentGameState->ActiveTeams.Num(); i++) {
			UTeamComponent* CurrTeam = CurrentGameState->ActiveTeams[i];
			float ChangeFloat = 0.f;
			uint8 Sentinel = 0;
			while (CurrTeam->PlayerControllerArray.Num() <= 8) {
				if (Sentinel > 50) break;
				FVector Vec = { FMath::FRandRange(0.f + ChangeFloat, 2000.f + ChangeFloat), FMath::FRandRange(0.f + ChangeFloat, 2000.f + ChangeFloat), 250.0f };
				FRotator Rot = { 0, 0, 0 };
				APlayableCharacter* AIChar = CurrTeam->SpawnTeamCharacter(Vec, Rot, 0);
				Sentinel++;
			}
			ChangeFloat += 5000.f;
		}
	}
}
