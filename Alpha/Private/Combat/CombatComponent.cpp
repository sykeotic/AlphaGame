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
	ULogger::ScreenMessage(FColor::Yellow, "Using Combat Weapon: ");
	ULogger::ScreenMessage(FColor::Yellow, FString::FromInt(CurrentWeaponIndex));
	if (CurrentWeapon) {
		ULogger::ScreenMessage(FColor::Yellow, "Current Weapon Exists");
		CurrentWeapon->OnUse();
	}
}

void UCombatComponent::CycleNextWeapon() {
	ULogger::ScreenMessage(FColor::Red, "Curr Weapon Index: ");
	ULogger::ScreenMessage(FColor::Red, FString::FromInt(CurrentWeaponIndex));	
	ULogger::ScreenMessage(FColor::Blue, "Weapon Array Length: ");
	ULogger::ScreenMessage(FColor::Blue, FString::FromInt(WeaponCount));

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

void UCombatComponent::SpawnWeapon(UMaterial* InWeaponMaterial, UStaticMesh* InStaticMesh, FName SocketLocation, ERange IN_RANGE, EActorType IN_ACTOR_TYPE, FName ProjectileSpawnLocation, float Dmg, float InRange) {
	FVector Location(0.0f, 0.0f, 0.0f);
	FRotator Rotation(0.0f, 0.0f, 0.0f);
	FActorSpawnParameters SpawnInfo;
	ACombatActor* Weapon;
	if (IN_RANGE == ERange::MELEE && IN_ACTOR_TYPE == EActorType::WEAPON) {
		Weapon = Cast<ACombatActor>(GetWorld()->SpawnActor<AMeleeCombatWeapon>(Location, Rotation, SpawnInfo));
	}
	else if (IN_RANGE == ERange::RANGED && IN_ACTOR_TYPE == EActorType::WEAPON) {
		Weapon = Cast<ACombatActor>(GetWorld()->SpawnActor<ARangedCombatWeapon>(Location, Rotation, SpawnInfo));
	}
	else {
		Weapon = nullptr;
	}
	Weapon->SetComponentOwner(this);
	Weapon->AssignWeaponValues(InStaticMesh, ProjectileSpawnLocation, IN_RANGE, IN_ACTOR_TYPE, Dmg, InRange);
	AddWeapon(Weapon, SocketLocation);
}

void UCombatComponent::AddWeapon(ACombatActor* InActor, FName SocketLocation) {
	ULogger::ScreenMessage(FColor::Green, "AddWeapon() Weapon Count:");
	ULogger::ScreenMessage(FColor::Green, FString::FromInt(WeaponCount));
	WeaponArray.AddUnique(InActor);
	Owner->WeaponSocketLocation = SocketLocation;
	if(CurrentWeapon)
		ULogger::ScreenMessage(FColor::White, "CurrWeapon is valid");
	if (InActor)
		ULogger::ScreenMessage(FColor::White, "InActor is valid");
	if (WeaponCount < 2)
		ULogger::ScreenMessage(FColor::White, "Weapon Count less than 2");
	if (WeaponCount < 1 && InActor) {
		CurrentWeaponIndex = 0;
		SetCurrentWeapon(WeaponArray[CurrentWeaponIndex]);
	}
	WeaponCount++;
}

void UCombatComponent::SetCurrentWeapon(ACombatActor* InActor) {
	if (CurrentWeapon) {
		FDetachmentTransformRules DetachRules(EDetachmentRule::KeepRelative, true);
		CurrentWeapon->DetachFromActor(DetachRules); 
	}
	CurrentWeapon = InActor;
	FAttachmentTransformRules AttachRules(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, true);
	CurrentWeapon->AttachToComponent(Owner->GetMesh(), AttachRules, Owner->WeaponSocketLocation);
}

void UCombatComponent::UseCurrentAbility(FVector InLocation, FRotator InRotation) {
	ULogger::ScreenMessage(FColor::Red, "Using Combat Ability");
	if (CurrentAbility) {
		CurrentAbility->OnUse();
	}
}

void UCombatComponent::CycleNextAbility() {

}

void UCombatComponent::CyclePreviousAbility() {

}

void UCombatComponent::GetAbilityAt(uint8 WeaponIndex) {

}


void UCombatComponent::RemoveAbility(uint8 InPosition) {
	ULogger::ScreenMessage(FColor::Red, "Removing Ability");
}

void UCombatComponent::AddAbility(ACombatActor* InActor) {
	ULogger::ScreenMessage(FColor::Red, "Adding Ability");
	AbilityArray.AddUnique(InActor);
	AbilityCount++;
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