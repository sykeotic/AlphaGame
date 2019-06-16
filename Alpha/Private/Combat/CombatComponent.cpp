#include "CombatComponent.h"
#include "CombatActor.h"
#include "Logger.h"

UCombatComponent::UCombatComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UCombatComponent::UseCurrentWeapon() {
	ULogger::ScreenMessage(FColor::Red, "Using Combat Weapon");
	if (CurrentWeapon) {
		CurrentWeapon->OnUse();
	}
}

void UCombatComponent::CycleNextWeapon() {

}

void UCombatComponent::CyclePreviousWeapon() {

}

void UCombatComponent::GetWeaponAt(int8 WeaponIndex) {

}

void UCombatComponent::RemoveWeapon(int8 InPosition) {

}

void UCombatComponent::AddWeapon(ACombatActor* InActor) {

}

void UCombatComponent::UseCurrentAbility() {
	ULogger::ScreenMessage(FColor::Red, "Using Combat Ability");
	if (CurrentAbility) {
		CurrentAbility->OnUse();
	}
}

void UCombatComponent::CycleNextAbility() {

}

void UCombatComponent::CyclePreviousAbility() {

}

void UCombatComponent::GetAbilityAt(int8 WeaponIndex) {

}


void UCombatComponent::RemoveAbility(int8 InPosition) {

}

void UCombatComponent::AddAbility(ACombatActor* InActor) {

}

void UCombatComponent::DestroyThisComponent() {

}

void UCombatComponent::AssignCombatActors() {

}

APlayableCharacter* UCombatComponent::GetOwner() {
	return Owner;
}

void UCombatComponent::SetOwner(APlayableCharacter* InChar) {
	Owner = InChar;
}