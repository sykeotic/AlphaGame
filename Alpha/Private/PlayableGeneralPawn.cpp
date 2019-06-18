#include "PlayableGeneralPawn.h"
#include "Camera/CameraComponent.h"


APlayableGeneralPawn::APlayableGeneralPawn()
{
	PrimaryActorTick.bCanEverTick = true;

	GeneralCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	GeneralCamera->SetupAttachment(RootComponent);
}

void APlayableGeneralPawn::BeginPlay()
{
	Super::BeginPlay();	
}

void APlayableGeneralPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void APlayableGeneralPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}