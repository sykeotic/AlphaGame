// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/Modes/MainMenuGameMode.h"
#include "Blueprint/UserWidget.h"
#include "Logger.h"
#include "GameFramework/PlayerState.h"
#include "PlayableGameInstance.h"
#include "Game/Controllers/MainMenuController.h"

AMainMenuGameMode::AMainMenuGameMode()
{
	FMainMenuTeamStruct Neutral_Team;
	Teams.Add(0, Neutral_Team);

	FMainMenuTeamStruct Team_1;
	Teams.Add(1, Team_1);

	FMainMenuTeamStruct Team_2;
	Teams.Add(2, Team_2);
}

void AMainMenuGameMode::BeginPlay()
{
	Super::BeginPlay();
}

void AMainMenuGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);
	AMainMenuController* InputPlayer = Cast<AMainMenuController>(NewPlayer);
	if (InputPlayer) {
		if (GameStruct.MainMenuWidget) {
			InputPlayer->SetCurrentWidget(GameStruct.MainMenuWidget);
		}
		else {
			ULogger::ScreenMessage(FColor::Red, "Game Struct Null");
		}
		FMainMenuPlayerStruct PlayerStruct;
		Teams.Find(0)->CurrentPlayerData.Add(InputPlayer->GetUniqueID(), PlayerStruct);
	}
}

void AMainMenuGameMode::SetPlayerRole(int32 PlayerID, FString InKey)
{
	for (auto& Elem : Teams) {
		if(Teams.Find(Elem.Key)->CurrentPlayerData.Contains(PlayerID)){
			FMainMenuPlayerStruct* TempPlayerStruct = Teams.Find(Elem.Key)->CurrentPlayerData.Find(PlayerID);
			TempPlayerStruct->SelectedRole = (*(GameStruct.AvailableRoles.Find(InKey)));
		}
	}
}

void AMainMenuGameMode::SetMatchLevel(FString InKey)
{
	SelectedLevel = GameStruct.AvailableLevels.Find(InKey)->Get();
}

void AMainMenuGameMode::SetGameMode(FString InKey)
{
	SelectedGameMode = GameStruct.AvailableGameModes.Find(InKey)->Get();
	UPlayableGameInstance* GameInstance = Cast<UPlayableGameInstance>(GetGameInstance());
	if (GameInstance) {
		GameInstance->SetGameModeData(SelectedGameMode);
	}
	else
	{
		ULogger::ScreenMessage(FColor::Red, "MainMenuGameGame::SetGameMode - GameInstance NULL");
	}
}

void AMainMenuGameMode::SetFaction(uint8 TeamID, FString InKey)
{
	Teams.Find(TeamID)->SelectedFactionData = GameStruct.AvailableFactions.Find(InKey)->Get();
}

void AMainMenuGameMode::SetExportData()
{
	UPlayableGameInstance* GameInstance = Cast<UPlayableGameInstance>(GetGameInstance());
	if (GameInstance) {
		GameInstance->SetTeamData(Teams);
	}
	else
	{
		ULogger::ScreenMessage(FColor::Red, "MainMenuGameGame::SetExportData - GameInstance NULL");
	}
}
