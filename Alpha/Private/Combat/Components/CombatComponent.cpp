#include "..\..\..\Public\Combat\Components\CombatComponent.h"
#include "Logger.h"
#include "BaseCombatActor.h"

UCombatComponent::UCombatComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}


void UCombatComponent::BeginPlay()
{
	Super::BeginPlay();
}

APlayableCharacter * UCombatComponent::GetCharacterOwner()
{
	return Owner;
}

void UCombatComponent::SetCharacterOwner(APlayableCharacter* InChar)
{
	Owner = InChar;
}

void UCombatComponent::UseCurrentWeapon()
{
	if (CurrentWeapon) {
		CurrentWeapon->OnUse();
	}
}

void UCombatComponent::CycleNextWeapon()
{
	if (CurrentWeaponIndex + 1 >= WeaponArray.Num()) {
		CurrentWeaponIndex = 0;
	}
	else {
		CurrentWeaponIndex++;
	}
	SetCurrentWeapon(WeaponArray[CurrentWeaponIndex], true);
	CurrentAbility->SetNextValidFireTime(GetWorld()->GetTimeSeconds() + .5);
}

void UCombatComponent::CyclePreviousWeapon()
{
	if (CurrentWeaponIndex - 1 < 0) {
		CurrentWeaponIndex = WeaponArray.Num() - 1;
	}
	else {
		CurrentWeaponIndex--;
	}
	SetCurrentWeapon(WeaponArray[CurrentWeaponIndex], true);
	CurrentAbility->SetNextValidFireTime(GetWorld()->GetTimeSeconds() + .5);
}

ABaseCombatActor* UCombatComponent::GetWeaponAt(uint8 WeaponIndex)
{
	return WeaponArray[WeaponIndex];
}

void UCombatComponent::SetCurrentWeapon(ABaseCombatActor * InActor, bool bEquipAnim)
{
	if (CurrentWeapon) {
		CurrentWeapon->OnUnEquip();
	}
	CurrentWeapon = InActor;
	CurrentWeapon->OnEquip(bEquipAnim);
}

ABaseCombatActor * UCombatComponent::GetCurrentWeapon()
{
	return CurrentWeapon;
}

void UCombatComponent::AddWeaponToArray(ABaseCombatActor* InActor)
{
	WeaponArray.AddUnique(InActor);
}

void UCombatComponent::CycleNextAbility()
{
	if (CurrentAbilityIndex + 1 >= AbilityArray.Num()) {
		CurrentAbilityIndex = 0;
	}
	else {
		CurrentAbilityIndex++;
	}
	SetCurrentAbility(AbilityArray[CurrentAbilityIndex], true);
	CurrentAbility->SetNextValidFireTime(GetWorld()->GetTimeSeconds() + .5);
}

void UCombatComponent::UseCurrentAbility()
{
	if (CurrentAbility) {
		CurrentAbility->OnUse();
	}
}

void UCombatComponent::CyclePreviousAbility()
{
	if (CurrentAbilityIndex - 1 < 0) {
		CurrentAbilityIndex = AbilityArray.Num() - 1;
	}
	else {
		CurrentAbilityIndex--;
	}
	SetCurrentAbility(AbilityArray[CurrentAbilityIndex], true);
	CurrentAbility->SetNextValidFireTime(GetWorld()->GetTimeSeconds() + .5);
}

ABaseCombatActor* UCombatComponent::GetAbilityAt(uint8 AbilityIndex)
{
	return AbilityArray[AbilityIndex];
}

void UCombatComponent::SetCurrentAbility(ABaseCombatActor * InActor, bool bEquipAnim)
{
	if (CurrentAbility) {
		CurrentAbility->OnUnEquip();
	}
	CurrentAbility = InActor;
	CurrentAbility->OnEquip(bEquipAnim);
}

ABaseCombatActor * UCombatComponent::GetCurrentAbility()
{
	return CurrentAbility;
}

void UCombatComponent::AddAbilityToArray(ABaseCombatActor* InActor)
{
	AbilityArray.AddUnique(InActor);
}

void UCombatComponent::SetCurrentWeaponIndex(uint8 InIndex)
{
	CurrentWeaponIndex = InIndex;
}

void UCombatComponent::SetCurrentAbilityIndex(uint8 InIndex)
{
	CurrentAbilityIndex = InIndex;
}

uint8 UCombatComponent::GetCurrentWeaponIndex()
{
	return uint8();
}

uint8 UCombatComponent::GetCurrentAbilityIndex()
{
	return uint8();
}

void UCombatComponent::HandleDeath()
{
	for (ABaseCombatActor* CurrActor : WeaponArray) {
		CurrActor->Destroy();
	}

	for (ABaseCombatActor* CurrActor : AbilityArray) {
		CurrActor->Destroy();
	}
}
