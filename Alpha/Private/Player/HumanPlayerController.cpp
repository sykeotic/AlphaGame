// Fill out your copyright notice in the Description page of Project Settings.

#include "HumanPlayerController.h"
#include "PlayableCharacter.h"
#include "Camera/CameraComponent.h"
#include "PlayableGeneralPawn.h"
#include "Runtime/UMG/Public/Components/WidgetComponent.h"
#include "Runtime/UMG/Public/Blueprint/WidgetBlueprintLibrary.h"
#include "NavigationSystem.h"
#include "TeamComponent.h"
#include "Blueprint/UserWidget.h"
#include "GameplayUtils.h"
#include "GeneralHUD.h"
#include "Logger.h"

AHumanPlayerController::AHumanPlayerController() {
	ULogger::ScreenMessage(FColor::Red, "Spawning Controller");
	bHeroChosen = false;
	bGeneralChosen = false;

	WidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("WidgetComponent"));
}

void AHumanPlayerController::BeginPlay() {
	PlayerControllerData = UGameplayUtils::RetrievePlayerControllerDataRow("TestBoi");
	UserWidget = CreateWidget<UUserWidget>(this, PlayerControllerData->RoleSelectWidgetClass);
	UserWidget->AddToViewport();
	// WidgetComponent->SetWidgetClass(PlayerControllerData->RoleSelectWidgetClass);
	// WidgetComponent->SetWidget(UserWidget);
}

void AHumanPlayerController::HeroSelect(FString& InKey, FVector InLoc, FRotator InRot) {
	InKey = "TestBoi";
	InLoc = {30.0f, 0.0f, 272.0f};
	InRot = {0.0f, 0.0f, 0.0f};
	if (bGeneralChosen)
		UnPossess();
	PlayerType = EPlayerType::HERO;
	FActorSpawnParameters SpawnInfo;
	HeroChar = ControllerTeam->SpawnTeamCharacter(CharacterBlueprint, InLoc, InRot);
	Possess(HeroChar);
	Unbind();
	HeroChar->SetupPlayerInputComponent(InputComponent);
	ULogger::ScreenMessage(FColor::Red, "Spawning Hero");
	bHeroChosen = true;
	bGeneralChosen = false;
	bShowMouseCursor = false;
	bEnableTouchEvents = true;
	UserWidget->RemoveFromViewport();
	UserWidget = CreateWidget<UUserWidget>(this, PlayerControllerData->HeroWidgetClass);
	UserWidget->AddToViewport();
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
	UWidgetBlueprintLibrary::SetInputMode_GameAndUIEx(this, nullptr, EMouseLockMode::DoNotLock, false);
	bShowMouseCursor = true;
	bGeneralChosen = true;
	bHeroChosen = false;
	UserWidget->RemoveFromViewport();
	UserWidget = CreateWidget<UUserWidget>(this, PlayerControllerData->GeneralWidgetClass);
	UserWidget->AddToViewport();
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