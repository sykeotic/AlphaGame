// Fill out your copyright notice in the Description page of Project Settings.

#include "TestGameMode.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "TestGameState.h"
#include "Logger.h"

ATestGameMode::ATestGameMode() {
	ULogger::SetIsTestRun(true);
}

void ATestGameMode::BeginPlay() {
	Super::BeginPlay();
	//MaxNumPlayers = FCString::Atoi(*(UGameplayStatics::ParseOption(OptionsString, "MaxNumPlayers
	MaxNumPlayers = 1;
}

bool ATestGameMode::MyReadyToStartMatch()
{
	return (ReadyToStartMatch() && NumPlayers == MaxNumPlayers);
}

void ATestGameMode::PostLogin(APlayerController* NewPlayer) {
	Super::PostLogin(NewPlayer);
	HumanPlayerControllerClass.Add(NewPlayer);
}