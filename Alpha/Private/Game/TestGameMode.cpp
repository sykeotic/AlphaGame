// Fill out your copyright notice in the Description page of Project Settings.

#include "TestGameMode.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Logger.h"

ATestGameMode::ATestGameMode() {
	ULogger::SetIsTestRun(true);
}

void ATestGameMode::BeginPlay() {
	Super::BeginPlay();

	//MaxNumPlayers = FCString::Atoi(*(UGameplayStatics::ParseOption(OptionsString, "MaxNumPlayers")));
	MaxNumPlayers = 1;
}

bool ATestGameMode::ReadyToStartMatch_Implementation() {
	Super::ReadyToStartMatch();

	return MaxNumPlayers == NumPlayers;
}

void ATestGameMode::PostLogin(APlayerController* NewPlayer) {
	Super::PostLogin(NewPlayer);
	HumanPlayerControllerClass.Add(NewPlayer);
}