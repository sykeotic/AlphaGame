// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/Modes/MainMenuGameMode.h"
#include "Blueprint/UserWidget.h"
#include "Logger.h"
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
	AMainMenuController* InputPlayer = Cast<AMainMenuController>(NewPlayer);
	if (InputPlayer) {
		if (GameStruct.MainMenuWidget) {
			InputPlayer->SetCurrentWidget(GameStruct.MainMenuWidget);
		}
		else {
			ULogger::ScreenMessage(FColor::Red, "Game Struct Null");
		}
	}
	else {
		ULogger::ScreenMessage(FColor::Red, "Input Player Cast Null");
	}
}