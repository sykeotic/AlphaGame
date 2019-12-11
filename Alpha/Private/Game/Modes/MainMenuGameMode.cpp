// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/Modes/MainMenuGameMode.h"
#include "Blueprint/UserWidget.h"
#include "Logger.h"
#include "GameFramework/PlayerState.h"
#include "Runtime/Engine/Classes/Engine/StreamableManager.h"
#include "PlayableGameInstance.h"
#include "Game/Controllers/MainMenuController.h"

AMainMenuGameMode::AMainMenuGameMode()
{

}

void AMainMenuGameMode::BeginPlay()
{
	Super::BeginPlay();
}

void AMainMenuGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);
	FMainMenuTeamStruct Team_1;
	FMainMenuTeamStruct Team_2;
	Team_2.SelectedFactionData = LoadObject<UFactionData>(NULL, TEXT("FactionData'/Game/Data/DataAssets/Factions/Dwarves.Dwarves'"), NULL, LOAD_None, NULL);
	AMainMenuController* InputPlayer = Cast<AMainMenuController>(NewPlayer);
	if (InputPlayer) {
		if (GameStruct.MainMenuWidget) {
			InputPlayer->SetCurrentWidget(GameStruct.MainMenuWidget);
		}
		FMainMenuPlayerStruct PlayerStruct;
		Team_1.CurrentPlayerData.Add(InputPlayer->GetUniqueID(), PlayerStruct);
		Cast<UPlayableGameInstance>(InputPlayer->GetGameInstance())->SetPersistentID(InputPlayer->GetUniqueID());
		FString Message =  FString::FromInt(InputPlayer->GetUniqueID());
		UE_LOG(LogTemp, Warning, TEXT("MainMenuGameMode::PostLogin - Adding Unique ID "), *Message);
	}
	Teams.Add(0, Team_1);
	Teams.Add(1, Team_2);
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
	SelectedLevel = GameStruct.AvailableLevels.FindRef(InKey);
}

void AMainMenuGameMode::SetGameMode(FString InKey)
{
	SelectedGameMode = GameStruct.AvailableGameModes.FindRef(InKey);
	if (SelectedGameMode) {
		UE_LOG(LogTemp, Warning, TEXT("MainMenuGameGame::SetGameMode - SelectedGameMode OK"));
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("MainMenuGameGame::SetGameMode - SelectedGameMode NULL"));
	}
	UPlayableGameInstance* GameInstance = Cast<UPlayableGameInstance>(GetGameInstance());
	if (GameInstance) {
		UE_LOG(LogTemp, Warning, TEXT("MainMenuGameGame::SetGameMode - GameInstance OK"));
		GameInstance->SetGameModeData(SelectedGameMode);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("MainMenuGameGame::SetGameMode - GameInstance NULL"));
	}
}

void AMainMenuGameMode::SetFaction(uint8 TeamID, FString InKey)
{
	if (GameStruct.AvailableFactions.FindRef(InKey)) {
		UE_LOG(LogTemp, Warning, TEXT("MainMenuGameGame::SetFaction - Faction Data OK"));
		Teams.FindOrAdd(TeamID).SelectedFactionData = GameStruct.AvailableFactions.FindRef(InKey);
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("MainMenuGameGame::SetFaction - Faction Data NULL"));
	}
}

void AMainMenuGameMode::SetExportData()
{
	UPlayableGameInstance* GameInstance = Cast<UPlayableGameInstance>(GetGameInstance());
	if (GameInstance) {
		UE_LOG(LogTemp, Warning, TEXT("MainMenuGameGame::SetExportData - GameInstance OK"));
		if (Teams.Num() > 0) {
			UE_LOG(LogTemp, Warning, TEXT("MainMenuGameGame::SetExportData - Team Elements OK"));
			GameInstance->SetTeamData(Teams);
		}
		else {
			UE_LOG(LogTemp, Warning, TEXT("MainMenuGameGame::SetExportData - Team Elements are ZERO"));
		}

	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("MainMenuGameGame::SetExportData - GameInstance NULL"));
	}
}
