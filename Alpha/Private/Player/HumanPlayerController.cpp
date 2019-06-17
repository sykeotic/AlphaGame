// Fill out your copyright notice in the Description page of Project Settings.

#include "HumanPlayerController.h"
#include "PlayableCharacter.h"
#include "Camera/CameraComponent.h"
#include "PlayableGeneralPawn.h"
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
	ULogger::ScreenMessage(FColor::Red, "Spawning Hero");
}

void AHumanPlayerController::GeneralSelect() {
	ULogger::ScreenMessage(FColor::Red, "Spawning General");
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
	ULogger::ScreenMessage(FColor::Red, "Selection Pressed");
}

void AHumanPlayerController::General_SelectionReleased() {
	ULogger::ScreenMessage(FColor::Red, "Selection Released");
}

void AHumanPlayerController::General_SetupInputComponent() {
	InputComponent->BindAction("LeftMouseClick", IE_Pressed, this, &AHumanPlayerController::General_SelectionPressed);
	InputComponent->BindAction("LeftMouseClick", IE_Released, this, &AHumanPlayerController::General_SelectionReleased);

	InputComponent->BindAction("RightMouseClick", IE_Pressed, this, &AHumanPlayerController::General_MoveReleased);
	InputComponent->BindAxis("MoveForward", this, &AHumanPlayerController::General_MoveForward);
	InputComponent->BindAxis("MoveRight", this, &AHumanPlayerController::General_MoveRight);

}


void AHumanPlayerController::General_MoveForward(float Value)
{
	if (Value != 0.0f)
	{
		const FRotator Rotation = GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		GeneralChar->AddMovementInput(Direction, Value);
	}
}

void AHumanPlayerController::General_MoveRight(float Value)
{
	if (Value != 0.0f)
	{
		const FRotator Rotation = GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		GeneralChar->AddMovementInput(Direction, Value);
	}
}
