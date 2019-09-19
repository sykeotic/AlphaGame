// Fill out your copyright notice in the Description page of Project Settings.

#include "HumanPlayerController.h"
#include "PlayableCharacter.h"
#include "Camera/CameraComponent.h"
#include "PlayableGeneralPawn.h"
#include "Runtime/UMG/Public/Components/WidgetComponent.h"
#include "Runtime/UMG/Public/Blueprint/WidgetBlueprintLibrary.h"
#include "NavigationSystem.h"
#include "PlayerControllerData.h"
#include "TeamComponent.h"
#include "Blueprint/UserWidget.h"
#include "GameplayUtils.h"
#include "GeneralHUD.h"
#include "PlayableGameInstance.h"
#include "Logger.h"

AHumanPlayerController::AHumanPlayerController() {
	ULogger::ScreenMessage(FColor::Red, "Spawning Controller");
	bHeroChosen = false;
	bGeneralChosen = false;

	WidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("WidgetComponent"));
}

void AHumanPlayerController::BeginPlay() {

}

void AHumanPlayerController::HeroSelect(uint8 HeroCharIndex) {
	if (bGeneralChosen)
		UnPossess();
	PlayerType = EPlayerType::HERO;
	FActorSpawnParameters SpawnInfo;
	HeroChar = ControllerTeam->SpawnTeamCharacter(InLocation, InRotation, HeroCharIndex);
	Possess(HeroChar);
	Unbind();
	if (HeroChar) {
		HeroChar->SetupPlayerInputComponent(InputComponent);
	}
	ULogger::ScreenMessage(FColor::Red, "Spawning Hero");
	bHeroChosen = true;
	bGeneralChosen = false;
	bShowMouseCursor = false;
	bEnableTouchEvents = true;
	UWidgetBlueprintLibrary::SetInputMode_GameOnly(this);
	if (UserWidget) {
		UserWidget->RemoveFromViewport();
	}
}

void AHumanPlayerController::GeneralSelect() {
	FVector InLoc = { 30.0f, 0.0f, 272.0f };
	FRotator InRot = { 0.0f, 0.0f, 0.0f };
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
}

void AHumanPlayerController::ShowHeroSelectWidget()
{
	// PlayerControllerData = LoadObject<UPlayerControllerData>(NULL, TEXT("PlayerControllerData'/Game/Data/DataAssets/Game/PlayerControllerData.PlayerControllerData'"), NULL, LOAD_None, NULL);
	UPlayableGameInstance* GameInst = Cast<UPlayableGameInstance>(GetGameInstance());
	UPlayerControllerData* TempData = GameInst->GameData->GameInstanceData.PlayerControllerData;
	if (TempData) {
		UserWidget = CreateWidget<UUserWidget>(this, TempData->PlayerControllerData.CharSelectionWidgetClass);
		UserWidget->AddToViewport();
	}
	else {
		ULogger::ScreenMessage(FColor::Red, "Controller Data not valid");
	}
	bShowMouseCursor = true;
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

void AHumanPlayerController::AssignData(UPlayerControllerData* InData)
{
	PlayerControllerData = InData;
}
