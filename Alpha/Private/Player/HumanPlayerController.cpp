// Fill out your copyright notice in the Description page of Project Settings.

#include "HumanPlayerController.h"
#include "PlayableCharacter.h"
#include "Camera/CameraComponent.h"
#include "PlayableGeneralPawn.h"
#include "NavigationSystem.h"
#include "GeneralHUD.h"
#include "Logger.h"

AHumanPlayerController::AHumanPlayerController() {
	ULogger::ScreenMessage(FColor::Red, "Spawning Controller");
	bHeroChosen = false;
	bGeneralChosen = false;
}

void AHumanPlayerController::HeroSelect(FString& InKey) {
	if (bGeneralChosen)
		UnPossess();
	PlayerType = EPlayerType::HERO;
	FVector Location(0.0f, 0.0f, 0.0f);
	FRotator Rotation(0.0f, 0.0f, 0.0f);
	FActorSpawnParameters SpawnInfo;
	HeroChar = GetWorld()->SpawnActor<APlayableCharacter>(Location, Rotation, SpawnInfo);
	Possess(HeroChar);
	HeroChar->SetupPlayerInputComponent(InputComponent);
	ULogger::ScreenMessage(FColor::Red, "Spawning Hero");
	bHeroChosen = true;
	bGeneralChosen = false;
}

void AHumanPlayerController::GeneralSelect() {
	if (bHeroChosen)
		UnPossess();
	ULogger::ScreenMessage(FColor::Red, "Spawning General");
	DefaultMouseCursor = EMouseCursor::Crosshairs;
	FVector Location(100.0f, 0.0f, 0.0f);
	FRotator Rotation(0.0f, 0.f, 0.0f);
	FActorSpawnParameters SpawnInfo;
	GeneralChar = GetWorld()->SpawnActor<APlayableGeneralPawn>(Location, Rotation, SpawnInfo);
	Possess(GeneralChar);
	GeneralChar->SetOwner(this);
	if (GeneralChar->GeneralHUD) {
		GeneralChar->GeneralHUD->PlayerOwner = this;
	}
	else {
		ULogger::ScreenMessage(FColor::Red, "HUD NotValid");
	}
	GeneralChar->SetupPlayerInputComponent(InputComponent);
	GeneralChar->GeneralCamera->AddRelativeRotation(FRotator(-70.f, 0.f, 0.f));
	GeneralChar->GeneralCamera->AddRelativeLocation(FVector(0.f, 0.f, 600.f));
	bGeneralChosen = true;
	bHeroChosen = false;
}