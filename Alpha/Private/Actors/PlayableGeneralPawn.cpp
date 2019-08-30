#include "PlayableGeneralPawn.h"
#include "Logger.h"
#include "HumanPlayerController.h"
#include "GeneralHUD.h"
#include "Runtime/Engine/Classes/Components/SphereComponent.h"
#include "GeneralPawnMovement.h"
#include "Runtime/AIModule/Classes/AIController.h"
#include "Runtime/AIModule/Classes/Navigation/PathFollowingComponent.h"
#include "Runtime/NavigationSystem/Public/NavFilters/NavigationQueryFilter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"


APlayableGeneralPawn::APlayableGeneralPawn()
{
	PrimaryActorTick.bCanEverTick = true;

	USphereComponent* SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("RootComponent"));
	RootComponent = SphereComponent;
	SphereComponent->InitSphereRadius(40.0f);
	SphereComponent->SetCollisionProfileName(TEXT("NoCollision"));

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraAttachmentArm"));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->bEnableCameraLag = true;
	SpringArm->CameraLagSpeed = 3.0f;
	SpringArm->TargetArmLength = 1000.0f;

	GeneralCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	GeneralCamera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);

	PawnMovementComponent = CreateDefaultSubobject<UGeneralPawnMovement>(TEXT("PawnMovementComponent"));
	PawnMovementComponent->UpdatedComponent = RootComponent;
}

void APlayableGeneralPawn::BeginPlay()
{
	Super::BeginPlay();	
}

UPawnMovementComponent* APlayableGeneralPawn::GetMovementComponent() const
{
	return PawnMovementComponent;
}

void APlayableGeneralPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void APlayableGeneralPawn::SetPlayerOwner(AHumanPlayerController* InController) {
	Owner = InController;
}

void APlayableGeneralPawn::RightClickReleased() {
	ULogger::ScreenMessage(FColor::Red, "Move Released");
	for (int32 i = 0; i < SelectedActors.Num(); i++) {
		FHitResult Hit;
		Owner->GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, false, Hit);
		FVector MoveLocation = Hit.Location + FVector(i / 2 * 100, i % 2 * 100, 0);
		AAIController* Cont = Cast<AAIController>(SelectedActors[i]->GetController());
		Cont->MoveToLocation(MoveLocation);
		DrawDebugSphere(GetWorld(), MoveLocation, 25, 10, FColor::Red, false, 1.0f, 0, 3.f);
		ULogger::ScreenMessage(FColor::Red, "Moving");
	}
}

void APlayableGeneralPawn::LeftClickPressed() {
	ULogger::ScreenMessage(FColor::Red, "Selection Pressed");
	Owner->GeneralHUD->InitialPoint = Owner->GeneralHUD->Get2DMousePosition();
	Owner->GeneralHUD->bStartSelecting = true;
}

void APlayableGeneralPawn::LeftClickReleased() {
	ULogger::ScreenMessage(FColor::Red, "Selection Released");
	Owner->GeneralHUD->bStartSelecting = false;
	SelectedActors = Owner->GeneralHUD->FoundActors;
	ULogger::ScreenMessage(FColor::Green, "Size: " + FString::FromInt(SelectedActors.Num()));
}

void APlayableGeneralPawn::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) {
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction("LeftMouseClick", IE_Pressed, this, &APlayableGeneralPawn::LeftClickPressed);
	PlayerInputComponent->BindAction("LeftMouseClick", IE_Released, this, &APlayableGeneralPawn::LeftClickReleased);

	PlayerInputComponent->BindAction("RightMouseClick", IE_Pressed, this, &APlayableGeneralPawn::RightClickReleased);

	PlayerInputComponent->BindAxis("MoveForward", this, &APlayableGeneralPawn::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &APlayableGeneralPawn::MoveRight);
}

void APlayableGeneralPawn::TeardownInput() {
	
}


void APlayableGeneralPawn::MoveForward(float Value) {
	if (Value != 0.f) {
		FVector Loc = GetActorLocation();
		FVector NewVector = { 90.0, 0.0, 0.0 };
		AddMovementInput(NewVector, Value);
	}
}

void APlayableGeneralPawn::MoveRight(float Value) {
	if (Value != 0.f) {
		FVector NewVector = { 0.0, 90.0, 0.0 };
		AddMovementInput(NewVector, Value);
	}
}