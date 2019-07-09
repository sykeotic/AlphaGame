#include "CombatComponent.h"
#include "CombatActor.h"
#include "CombatAbility.h"
#include "RangedCombatWeapon.h"
#include "MeleeCombatWeapon.h"
#include "PlayableCharacter.h"
#include "Runtime/Engine/Classes/Materials/Material.h"
#include "Logger.h"

UCombatComponent::UCombatComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	WeaponCount = 0;
	CurrentWeapon = nullptr;
	AbilityCount = 0;
	CurrentAbility = nullptr;
}

void UCombatComponent::UseCurrentWeapon() {
	if (CurrentWeapon) {
		CurrentWeapon->HandleUse();
	}
}

void UCombatComponent::CycleNextWeapon() {
	if (CurrentWeaponIndex + 2 < WeaponCount) {
		CurrentWeaponIndex++;
	}
	else {
		CurrentWeaponIndex = 0;
	}
	SetCurrentWeapon(WeaponArray[CurrentWeaponIndex], true);
}

void UCombatComponent::CyclePreviousWeapon() {

}

void UCombatComponent::GetWeaponAt(uint8 WeaponIndex) {

}

void UCombatComponent::SetCurrentWeapon(ACombatWeapon* InActor, bool bEquipAnim) {
	if (CurrentWeapon) {
		CurrentWeapon->OnUnEquip();
	} 
	CurrentWeapon = InActor;
	CurrentWeapon->OnEquip(bEquipAnim);
	CurrentWeapon->AddActorLocalRotation(CurrentWeapon->WeaponRotation);
}

void UCombatComponent::SetCurrentAbility(ACombatAbility* InActor, bool bEquipAnim) {
	if (CurrentAbility) {
		CurrentAbility->OnUnEquip();
	}
	CurrentAbility = InActor;
	CurrentAbility->OnEquip(bEquipAnim);
}

void UCombatComponent::UseCurrentAbility() {
	ULogger::ScreenMessage(FColor::Red, "Using Combat Ability");
	if (CurrentAbility) {
		CurrentAbility->HandleUse();
	}
}

void UCombatComponent::CycleNextAbility() {
	if (CurrentAbilityIndex + 2 < AbilityCount) {
		CurrentAbilityIndex++;
	}
	else {
		CurrentAbilityIndex = 0;
	}
	SetCurrentAbility(AbilityArray[CurrentAbilityIndex], true);
}

void UCombatComponent::CyclePreviousAbility() {

}

void UCombatComponent::GetAbilityAt(uint8 AbilityIndex) {

}

APlayableCharacter* UCombatComponent::GetCharacterOwner() {
	return CharacterOwner;
}

void UCombatComponent::SetCharacterOwner(APlayableCharacter* InChar) {
	CharacterOwner = InChar;
}