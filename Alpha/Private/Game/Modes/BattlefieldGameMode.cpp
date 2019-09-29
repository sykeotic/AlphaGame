#include "BattlefieldGameMode.h"
#include "Engine/World.h"
#include "Logger.h"
#include "Game/PlayableGameInstance.h"

void ABattlefieldGameMode::InitTeams()
{

}

void ABattlefieldGameMode::BeginPlay()
{
	Super::BeginPlay();
	UPlayableGameInstance* GameInstance = Cast<UPlayableGameInstance>(GetGameInstance());
	CurrentGameModeData = GameInstance->GetGameModeData();
	if (CurrentGameModeData) {
		ULogger::ScreenMessage(FColor::Red, "BattlefieldGameMode::BeginPlay - Game Mode Data OK");
	}
	else {
		ULogger::ScreenMessage(FColor::Red, "BattlefieldGameMode::BeginPlay - Game Mode Data NULL");
	}
}

void ABattlefieldGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);
	ULogger::ScreenMessage(FColor::Red, "PostLogin");
	NewPlayer->ClientMessage("Loaded");
}

void ABattlefieldGameMode::StartMatch()
{
	Super::StartMatch();
	ULogger::ScreenMessage(FColor::Red, "StartMatch");
}

void ABattlefieldGameMode::InitTeam(FMainMenuTeamStruct InTeamStruct)
{

}
