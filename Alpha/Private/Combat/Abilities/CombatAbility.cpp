#include "CombatAbility.h"
#include "PlayableCharacter.h"
#include "Logger.h"
#include "CombatComponent.h"

ACombatAbility::ACombatAbility()
{
	PrimaryActorTick.bCanEverTick = false;
}

void ACombatAbility::BeginPlay()
{
	Super::BeginPlay();
	MeshComp->SetVisibility(false);
}

void ACombatAbility::OnUse() {
	ULogger::ScreenMessage(FColor::Red, "ACA Use");
}