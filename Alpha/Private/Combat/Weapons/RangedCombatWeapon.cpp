#include "RangedCombatWeapon.h"
#include "Logger.h"

ARangedCombatWeapon::ARangedCombatWeapon()
{
	PrimaryActorTick.bCanEverTick = false;

}

void ARangedCombatWeapon::OnUse(){
	ULogger::ScreenMessage(FColor::Red, "Ranged Weapon Firing");
}