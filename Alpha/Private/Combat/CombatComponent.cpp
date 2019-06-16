#include "CombatComponent.h"
#include "CombatActor.h"
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

void UCombatComponent::AddWeapon(ACombatActor* InActor, UMaterial* InWeaponMaterial, UStaticMesh* InStaticMesh, FString SocketLocation) {
	ULogger::ScreenMessage(FColor::Green, "AddWeapon() Weapon Count:");
	ULogger::ScreenMessage(FColor::Green, FString::FromInt(WeaponCount));
	WeaponArray.AddUnique(InActor);
	InActor->WeaponMaterial = InWeaponMaterial;
	InActor->MeshComp->SetStaticMesh(InStaticMesh);
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
	CurrentWeapon->MeshComp->SetStaticMesh(InActor->MeshComp->GetStaticMesh());
	CurrentWeapon->MeshComp->SetMaterial(0, InActor->WeaponMaterial);
	FAttachmentTransformRules AttachRules(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, true);
	CurrentWeapon->AttachToComponent(Owner->GetMesh(), AttachRules, FName(*Owner->WeaponSocketLocation));
	CurrentWeapon->MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
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