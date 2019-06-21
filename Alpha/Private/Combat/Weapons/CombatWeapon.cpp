#include "CombatWeapon.h"
#include "Logger.h"

ACombatWeapon::ACombatWeapon()
{
	PrimaryActorTick.bCanEverTick = false;
	VALID_TARGETS = EValidTargets::ENEMIES;
	ACTOR_TYPE = EActorType::WEAPON;
}

void ACombatWeapon::OnUse(FVector InLocation, FRotator InRotation) {
	Super::OnUse(InLocation, InRotation);
	ULogger::ScreenMessage(FColor::Yellow, "ACombatWeapon");
}