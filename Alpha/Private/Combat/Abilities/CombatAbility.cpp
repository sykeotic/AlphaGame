#include "CombatAbility.h"

ACombatAbility::ACombatAbility()
{
	PrimaryActorTick.bCanEverTick = true;

}

void ACombatAbility::BeginPlay()
{
	Super::BeginPlay();
}

void ACombatAbility::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

