#include "CombatWeapon.h"

ACombatWeapon::ACombatWeapon()
{
	PrimaryActorTick.bCanEverTick = false;
	VALID_TARGETS = EValidTargets::ENEMIES;
	ACTOR_TYPE = EActorType::WEAPON;
}

void ACombatWeapon::OnUse() {

}