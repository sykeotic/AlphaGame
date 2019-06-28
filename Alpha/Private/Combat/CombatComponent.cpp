#include "CombatComponent.h"
#include "CombatActor.h"
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
}

void UCombatComponent::UseCurrentWeapon() {
	if (CurrentWeapon) {
		bIsUsingActor = true;
		CurrentWeapon->OnUse();
	}
}

void UCombatComponent::CycleNextWeapon() {
	if (CurrentWeaponIndex + 1 < WeaponCount) {
		CurrentWeaponIndex++;
	}
	else {
		CurrentWeaponIndex = 0;
	}
	SetCurrentWeapon(WeaponArray[CurrentWeaponIndex]);
}

void UCombatComponent::CyclePreviousWeapon() {

}

void UCombatComponent::GetWeaponAt(uint8 WeaponIndex) {

}

void UCombatComponent::RemoveWeapon(uint8 InPosition) {
	ULogger::ScreenMessage(FColor::Red, "Removing Weapon");
}

void UCombatComponent::SpawnWeapon(float InCooldown, FVector InLocation, FRotator InRotation, UMaterial* InWeaponMaterial, UStaticMesh* InStaticMesh, FName SocketLocation, ERange IN_RANGE, EActorType IN_ACTOR_TYPE, FName ProjectileSpawnLocation, float Dmg, float InRange) {
	FActorSpawnParameters SpawnInfo;
	ACombatWeapon* Weapon;
	if (IN_RANGE == ERange::MELEE && IN_ACTOR_TYPE == EActorType::WEAPON) {
		Weapon = Cast<ACombatWeapon>(GetWorld()->SpawnActor<AMeleeCombatWeapon>(MeleeWeaponClass, InLocation, InRotation, SpawnInfo));
	}
	else if (IN_RANGE == ERange::RANGED && IN_ACTOR_TYPE == EActorType::WEAPON) {
		Weapon = Cast<ACombatWeapon>(GetWorld()->SpawnActor<ARangedCombatWeapon>(RangedWeaponClass, InLocation, InRotation, SpawnInfo));
	}
	else {
		Weapon = nullptr;
	}
	Weapon->SetCombatComponentOwner(this);
	Weapon->WeaponLocation = InLocation;
	Weapon->WeaponRotation = InRotation;
	Weapon->AssignWeaponValues(InCooldown, InStaticMesh, ProjectileSpawnLocation, IN_RANGE, IN_ACTOR_TYPE, Dmg, InRange);
	AddWeapon(Weapon, SocketLocation);
}

void UCombatComponent::AddWeapon(ACombatWeapon* InActor, FName InSocketLocation) {
	ULogger::ScreenMessage(FColor::Green, "AddWeapon() Weapon Count:");
	ULogger::ScreenMessage(FColor::Green, FString::FromInt(WeaponCount));
	WeaponArray.AddUnique(InActor);
	CharacterOwner->WeaponSocketLocation = InSocketLocation;
	if (WeaponCount < 1 && InActor) {
		CurrentWeaponIndex = 0;
		SetCurrentWeapon(WeaponArray[CurrentWeaponIndex]);
	}
	WeaponCount++;
}

void UCombatComponent::SetCurrentWeapon(ACombatWeapon* InActor) {
	if (CurrentWeapon) {
		CurrentWeapon->OnUnEquip();
	} 
	CurrentWeapon = InActor;
	CurrentWeapon->OnEquip(true);
	ULogger::ScreenMessage(FColor::Red,CurrentWeapon->WeaponRotation.ToString());
	CurrentWeapon->AddActorLocalRotation(CurrentWeapon->WeaponRotation);
}

void UCombatComponent::UseCurrentAbility(FVector InLocation, FRotator InRotation) {
	ULogger::ScreenMessage(FColor::Red, "Using Combat Ability");
	if (CurrentAbility) {
		//CurrentAbility->OnUse();
	}
}

void UCombatComponent::CycleNextAbility() {

}

void UCombatComponent::CyclePreviousAbility() {

}

void UCombatComponent::GetAbilityAt(uint8 AbilityIndex) {

}


void UCombatComponent::RemoveAbility(uint8 InPosition) {
	ULogger::ScreenMessage(FColor::Red, "Removing Ability");
}

void UCombatComponent::AddAbility(ACombatAbility* InActor) {
	ULogger::ScreenMessage(FColor::Red, "Adding Ability");
	AbilityArray.AddUnique(InActor);
	AbilityCount++;
}

void UCombatComponent::DestroyThisComponent() {

}

void UCombatComponent::AssignCombatActors() {

}

APlayableCharacter* UCombatComponent::GetCharacterOwner() {
	return CharacterOwner;
}

void UCombatComponent::SetCharacterOwner(APlayableCharacter* InChar) {
	CharacterOwner = InChar;
}