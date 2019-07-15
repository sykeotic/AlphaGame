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
	ULogger::ScreenMessage(FColor::Red, "Weapon Count: " + FString::FromInt(WeaponCount));
	if (CurrentWeaponIndex + 1 >= WeaponCount) {
		CurrentWeaponIndex = 0;
	}
	else {
		CurrentWeaponIndex++;
	}
	ULogger::ScreenMessage(FColor::Red, "Current Weapon Index: " + FString::FromInt(CurrentWeaponIndex));
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
	if (CurrentAbilityIndex + 1 >= AbilityCount) {
		CurrentAbilityIndex = 0;		
	}
	else {
		CurrentAbilityIndex++;
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