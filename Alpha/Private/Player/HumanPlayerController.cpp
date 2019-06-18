// Fill out your copyright notice in the Description page of Project Settings.

#include "HumanPlayerController.h"
#include "PlayableCharacter.h"
#include "Camera/CameraComponent.h"
#include "PlayableGeneralPawn.h"
#include "GeneralHUD.h"
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
	Possess(HeroChar);
	HeroChar->SetupPlayerInputComponent(InputComponent);
	ULogger::ScreenMessage(FColor::Red, "Spawning Hero");
}

void AHumanPlayerController::GeneralSelect() {
	ULogger::ScreenMessage(FColor::Red, "Spawning General");
	GeneralHUD = Cast<AGeneralHUD>(this->GetHUD());
	if (GeneralHUD->IsValidLowLevel()) {
		ULogger::ScreenMessage(FColor::Red, "Cast worked");
	}
	DefaultMouseCursor = EMouseCursor::Crosshairs;
	FVector Location(100.0f, 0.0f, 0.0f);
	FRotator Rotation(0.0f, 0.f, 0.0f);
	FActorSpawnParameters SpawnInfo;
	GeneralChar = GetWorld()->SpawnActor<APlayableGeneralPawn>(Location, Rotation, SpawnInfo);
	Possess(GeneralChar);
	GeneralChar->GeneralCamera->AddRelativeRotation(FRotator(-70.f, 0.f, 0.f));
	GeneralChar->GeneralCamera->AddRelativeLocation(FVector(0.f, 0.f, 600.f));
	General_SetupInputComponent();
}

void AHumanPlayerController::General_MoveReleased() {
	ULogger::ScreenMessage(FColor::Red, "Move Released");
}

void AHumanPlayerController::General_SelectionPressed() {
	GeneralHUD->InitialPoint = GeneralHUD->Get2DMousePosition();
	GeneralHUD->bStartSelecting = true;
}

void AHumanPlayerController::General_SelectionReleased() {
	GeneralHUD->bStartSelecting = false;
}

void AHumanPlayerController::General_SetupInputComponent() {
	InputComponent->BindAction("LeftMouseClick", IE_Pressed, this, &AHumanPlayerController::General_SelectionPressed);
	InputComponent->BindAction("LeftMouseClick", IE_Released, this, &AHumanPlayerController::General_SelectionReleased);

	InputComponent->BindAction("RightMouseClick", IE_Pressed, this, &AHumanPlayerController::General_MoveReleased);
}