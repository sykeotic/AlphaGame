// Fill out your copyright notice in the Description page of Project Settings.

#include "HumanPlayerController.h"
#include "PlayableCharacter.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
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
	ULogger::ScreenMessage(FColor::Red, "Spawning Hero");
}

void AHumanPlayerController::AssignCameraValues(float BaseTurnRate, float BaseLookupRate, bool bUseYaw, bool bUsePitch, bool bUseRoll) {
	ULogger::ScreenMessage(FColor::Red, "Assigning Camera Stats");
	if (HeroChar) {
		HeroChar->SetBaseTurnRate(BaseTurnRate);
		HeroChar->SetBaseLookUpRate(BaseLookupRate);
		HeroChar->bUseControllerRotationPitch = bUsePitch;
		HeroChar->bUseControllerRotationRoll = bUseRoll;
		HeroChar->bUseControllerRotationYaw = bUseYaw;
		if(HeroChar->CameraBoom)
			HeroChar->CameraBoom->bEnableCameraLag = true;
	}
}

void AHumanPlayerController::AssignHeroStatValues(float JumpVelocity, FRotator RotationRate, float MaxHealth, float MoveSpeed) {
	HeroChar->GetCharacterMovement()->bOrientRotationToMovement = true;
	HeroChar->GetCharacterMovement()->JumpZVelocity = JumpVelocity;
	HeroChar->GetCharacterMovement()->RotationRate = RotationRate;
	HeroChar->GetCharacterMovement()->MaxWalkSpeed = MoveSpeed;
	HeroChar->GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
	HeroChar->GetMesh()->SetRelativeLocation(FVector(0.f, 0.f, -90.f));
}