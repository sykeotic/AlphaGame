#include "PlayablePawn.h"

APlayablePawn::APlayablePawn()
{
	PrimaryActorTick.bCanEverTick = false;
}

void APlayablePawn::BeginPlay()
{
	Super::BeginPlay();	
}

void APlayablePawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

