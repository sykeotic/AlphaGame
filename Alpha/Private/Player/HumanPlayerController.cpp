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
}

void AHumanPlayerController::GeneralSelect() {
	if (bHeroChosen)
		UnPossess();
	ULogger::ScreenMessage(FColor::Red, "Spawning General");
	General_SetupInputComponent();
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
	bHeroChosen = true;
}

void AHumanPlayerController::General_MoveReleased() {
	ULogger::ScreenMessage(FColor::Red, "Move Released");
	if (SelectedActors.Num() > 0) {
		for (int32 i = 0; i < SelectedActors.Num(); i++) {
			FHitResult Hit;
			GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, false, Hit);
			FVector MoveLocation = Hit.Location + FVector(i / 2 * 100, i % 2 * 100, 0);
			if (!SelectedActors[i]->GetController()) {
				ULogger::ScreenMessage(FColor::Red, "Controller not found");
			}
			else {
				ULogger::ScreenMessage(FColor::Red, "Controller found");
			}
			UNavigationSystem::SimpleMoveToLocation(SelectedActors[i]->GetController(), MoveLocation);
			DrawDebugSphere(GetWorld(), MoveLocation, 25, 10, FColor::Red, 3);
		}
	}
}

void AHumanPlayerController::General_SelectionPressed() {
	ULogger::ScreenMessage(FColor::Red, "Selection Pressed");
	GeneralHUD->InitialPoint = GeneralHUD->Get2DMousePosition();
	GeneralHUD->bStartSelecting = true;
}

void AHumanPlayerController::General_SelectionReleased() {
	ULogger::ScreenMessage(FColor::Red, "Selection Released");
	GeneralHUD->bStartSelecting = false;
	SelectedActors = GeneralHUD->FoundActors;
}

void AHumanPlayerController::General_SetupInputComponent() {
	InputComponent->BindAction("LeftMouseClick", IE_Pressed, this, &AHumanPlayerController::General_SelectionPressed);
	InputComponent->BindAction("LeftMouseClick", IE_Released, this, &AHumanPlayerController::General_SelectionReleased);

	InputComponent->BindAction("RightMouseClick", IE_Pressed, this, &AHumanPlayerController::General_MoveReleased);
}