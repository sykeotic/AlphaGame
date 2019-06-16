// Fill out your copyright notice in the Description page of Project Settings.

#include "HumanPlayerController.h"
#include "PlayableCharacter.h"
#include "Logger.h"

AHumanPlayerController::AHumanPlayerController() {
	ULogger::ScreenMessage(FColor::Red, "Spawning Controller");
}

void AHumanPlayerController::HeroSelect(FString& InKey) {
	PlayerType = EPlayerType::HERO;
	FVector Location(0.0f, 0.0f, 0.0f);
	FRotator Rotation(0.0f, 0.0f, 0.0f);
	FActorSpawnParameters SpawnInfo;
	HeroChar = GetWorld()->SpawnActor<APlayableCharacter>(Location, Rotation, SpawnInfo);
	ULogger::ScreenMessage(FColor::Red, "Spawning Hero");
}