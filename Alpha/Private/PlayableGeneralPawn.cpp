#include "PlayableGeneralPawn.h"
#include "Logger.h"
#include "HumanPlayerController.h"
#include "GeneralHUD.h"
#include "Camera/CameraComponent.h"


APlayableGeneralPawn::APlayableGeneralPawn()
{
	PrimaryActorTick.bCanEverTick = true;

	GeneralCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	GeneralCamera->SetupAttachment(RootComponent);

	GeneralHUD = Cast<AGeneralHUD>(this->GetHUD());
	if (GeneralHUD->IsValidLowLevel()) {
		ULogger::ScreenMessage(FColor::Red, "Cast worked");
	}
}

AGeneralHUD* APlayableGeneralPawn::GetHUD() {
	return GeneralHUD;
}

void APlayableGeneralPawn::BeginPlay()
{
	Super::BeginPlay();	
}

void APlayableGeneralPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void APlayableGeneralPawn::SetOwner(AHumanPlayerController* InController) {
	Owner = InController;
}

void APlayableGeneralPawn::RightClickReleased() {
	ULogger::ScreenMessage(FColor::Red, "Move Released");
	if (SelectedActors.Num() > 0) {
		for (int32 i = 0; i < SelectedActors.Num(); i++) {
			FHitResult Hit;
			Owner->GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, false, Hit);
			FVector MoveLocation = Hit.Location + FVector(i / 2 * 100, i % 2 * 100, 0);
			if (!SelectedActors[i]->GetController()) {
				ULogger::ScreenMessage(FColor::Red, "Controller not found");
			}
			else {
				ULogger::ScreenMessage(FColor::Red, "Controller found");
			}
			//UNavigationSystem::SimpleMoveToLocation(SelectedActors[i]->GetController(), MoveLocation);
			DrawDebugSphere(GetWorld(), MoveLocation, 25, 10, FColor::Red, 3);
		}
	}
}

void APlayableGeneralPawn::LeftClickPressed() {
	ULogger::ScreenMessage(FColor::Red, "Selection Pressed");
	GeneralHUD->InitialPoint = GeneralHUD->Get2DMousePosition();
	GeneralHUD->bStartSelecting = true;
}

void APlayableGeneralPawn::LeftClickReleased() {
	ULogger::ScreenMessage(FColor::Red, "Selection Released");
	GeneralHUD->bStartSelecting = false;
	SelectedActors = GeneralHUD->FoundActors;
}

void APlayableGeneralPawn::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) {
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction("LeftMouseClick", IE_Pressed, this, &APlayableGeneralPawn::LeftClickPressed);
	PlayerInputComponent->BindAction("LeftMouseClick", IE_Released, this, &APlayableGeneralPawn::LeftClickReleased);

	PlayerInputComponent->BindAction("RightMouseClick", IE_Pressed, this, &APlayableGeneralPawn::RightClickReleased);
}