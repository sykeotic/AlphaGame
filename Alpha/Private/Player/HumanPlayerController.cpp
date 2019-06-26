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

void AHumanPlayerController::HeroSelect(FString& InKey, FVector InLoc, FRotator InRot) {
	if (bGeneralChosen)
		UnPossess();
	PlayerType = EPlayerType::HERO;
	FActorSpawnParameters SpawnInfo;
	HeroChar = GetWorld()->SpawnActor<APlayableCharacter>(CharacterBlueprint, InLoc, InRot, SpawnInfo);
	Possess(HeroChar);
	Unbind();
	HeroChar->SetupPlayerInputComponent(InputComponent);
	ULogger::ScreenMessage(FColor::Red, "Spawning Hero");
	bHeroChosen = true;
	bGeneralChosen = false;
}

void AHumanPlayerController::GeneralSelect(FVector InLoc, FRotator InRot) {
	if (bHeroChosen)
		UnPossess();
	ULogger::ScreenMessage(FColor::Red, "Spawning General");
	DefaultMouseCursor = EMouseCursor::Crosshairs;
	FActorSpawnParameters SpawnInfo;
	GeneralChar = GetWorld()->SpawnActor<APlayableGeneralPawn>(PawnBlueprint, InLoc, InRot, SpawnInfo);
	Possess(GeneralChar);
	Unbind();
	GeneralChar->SetOwner(this);
	SetupGeneralHUD();
	GeneralChar->SetupPlayerInputComponent(InputComponent);
	GeneralChar->GeneralCamera->AddRelativeRotation(FRotator(-70.f, 0.f, 0.f));
	GeneralChar->GeneralCamera->AddRelativeLocation(FVector(0.f, 0.f, 600.f));
	bGeneralChosen = true;
	bHeroChosen = false;
}

void AHumanPlayerController::Unbind() {
	InputComponent->AxisBindings.Empty();
	InputComponent->ClearActionBindings();
}

void AHumanPlayerController::SetupGeneralHUD() {
	GeneralHUD = Cast<AGeneralHUD>(this->GetHUD());
	if (GeneralHUD->IsValidLowLevel()) {
		ULogger::ScreenMessage(FColor::Red, "Cast worked");
	}
}