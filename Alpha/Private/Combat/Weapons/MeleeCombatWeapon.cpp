#include "MeleeCombatWeapon.h"
#include "Components/CapsuleComponent.h"
#include "Logger.h"

AMeleeCombatWeapon::AMeleeCombatWeapon()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AMeleeCombatWeapon::OnUse() {
	
}