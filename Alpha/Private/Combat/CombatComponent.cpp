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
	ULogger::ScreenMessage(FColor::Red, "Before Curr Weapon Index: ");
	ULogger::ScreenMessage(FColor::Red, FString::FromInt(CurrentWeaponIndex));
	ULogger::ScreenMessage(FColor::Blue, "Weapon Array Length: ");
	ULogger::ScreenMessage(FColor::Blue, FString::FromInt(WeaponCount));
	if (CurrentWeaponIndex + 1 < WeaponCount) {
		CurrentWeaponIndex++;
	}
	else {
		CurrentWeaponIndex = 0;
	}
	ULogger::ScreenMessage(FColor::Red, "After Curr Weapon Index: ");
	ULogger::ScreenMessage(FColor::Red, FString::FromInt(CurrentWeaponIndex));
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
		Weapon = Cast<ACombatWeapon>(GetWorld()->SpawnActor<AMeleeCombatWeapon>(InLocation, InRotation, SpawnInfo));
	}
	else if (IN_RANGE == ERange::RANGED && IN_ACTOR_TYPE == EActorType::WEAPON) {
		Weapon = Cast<ACombatWeapon>(GetWorld()->SpawnActor<ARangedCombatWeapon>(InLocation, InRotation, SpawnInfo));
	}
	else {
		Weapon = nullptr;
	}
	Weapon->SetComponentOwner(this);
	Weapon->WeaponLocation = InLocation;
	Weapon->WeaponRotation = InRotation;
	Weapon->AssignWeaponValues(InCooldown, InStaticMesh, ProjectileSpawnLocation, IN_RANGE, IN_ACTOR_TYPE, Dmg, InRange);
	AddWeapon(Weapon, SocketLocation);
}

void UCombatComponent::AddWeapon(ACombatWeapon* InActor, FName SocketLocation) {
	ULogger::ScreenMessage(FColor::Green, "AddWeapon() Weapon Count:");
	ULogger::ScreenMessage(FColor::Green, FString::FromInt(WeaponCount));
	WeaponArray.AddUnique(InActor);
	Owner->WeaponSocketLocation = SocketLocation;
	if (WeaponCount < 1 && InActor) {
		CurrentWeaponIndex = 0;
		SetCurrentWeapon(WeaponArray[CurrentWeaponIndex]);
		FAttachmentTransformRules AttachRules(EAttachmentRule::KeepRelative, EAttachmentRule::KeepRelative, EAttachmentRule::SnapToTarget, true);
		InActor->AttachToComponent(Owner->GetMesh(), AttachRules, Owner->WeaponSocketLocation);
		InActor->MeshComp->SetVisibility(true);
	} else {
		FAttachmentTransformRules AttachRules(EAttachmentRule::KeepRelative, EAttachmentRule::KeepRelative, EAttachmentRule::SnapToTarget, true);
		InActor->AttachToComponent(Owner->GetMesh(), AttachRules, Owner->WeaponSocketLocation);
		InActor->MeshComp->SetVisibility(false);
	}
	WeaponCount++;
}

void UCombatComponent::SetCurrentWeapon(ACombatWeapon* InActor) {
	if (CurrentWeapon) {
		CurrentWeapon->MeshComp->SetVisibility(false);
	}
	CurrentWeapon = InActor;
	if (CurrentWeapon->RANGE_TYPE == ERange::RANGED) {
		Owner->bWeaponIsRanged = true;
	}
	else if (CurrentWeapon->RANGE_TYPE == ERange::MELEE) {
		Owner->bWeaponIsRanged = false;
	}
	CurrentWeapon->MeshComp->SetVisibility(true);
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

APlayableCharacter* UCombatComponent::GetOwner() {
	return Owner;
}

void UCombatComponent::SetOwner(APlayableCharacter* InChar) {
	Owner = InChar;
}