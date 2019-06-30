#include "CombatWeapon.h"
#include "CombatComponent.h"
#include "PlayableCharacter.h"
#include "Logger.h"

ACombatWeapon::ACombatWeapon()
{
	PrimaryActorTick.bCanEverTick = false;
	VALID_TARGETS = EValidTargets::ENEMIES;
	ACTOR_TYPE = EActorType::WEAPON;
}

void ACombatWeapon::BeginPlay() {
	Super::BeginPlay();
}

void ACombatWeapon::AttachMeshToOwner(FName AttachPoint) {
	Super::AttachMeshToOwner(AttachPoint);
}

void ACombatWeapon::OnUse() {
	Super::OnUse();
	ULogger::ScreenMessage(FColor::Yellow, "ACombatWeapon");
}