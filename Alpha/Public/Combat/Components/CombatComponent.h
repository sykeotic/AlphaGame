// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CombatComponent.generated.h"

class APlayableCharacter;
class ABaseCombatActor;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ALPHA_API UCombatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UCombatComponent();

	APlayableCharacter* GetCharacterOwner();
	void SetCharacterOwner(APlayableCharacter* InChar);

	void CycleNextWeapon();
	void UseCurrentWeapon();
	void CyclePreviousWeapon();
	ABaseCombatActor* GetWeaponAt(uint8 WeaponIndex);
	void SetCurrentWeapon(ABaseCombatActor* InActor, bool bEquipAnim);
	ABaseCombatActor* GetCurrentWeapon();
	void AddWeaponToArray(ABaseCombatActor* InActor);

	void CycleNextAbility();
	void UseCurrentAbility();
	void CyclePreviousAbility();
	ABaseCombatActor* GetAbilityAt(uint8 AbilityIndex);
	void SetCurrentAbility(ABaseCombatActor* InActor, bool bEquipAnim);
	ABaseCombatActor* GetCurrentAbility();
	void AddAbilityToArray(ABaseCombatActor* InActor);

	void SetCurrentWeaponIndex(uint8 InIndex);
	void SetCurrentAbilityIndex(uint8 InIndex);

	uint8 GetCurrentWeaponIndex();
	uint8 GetCurrentAbilityIndex();

	void HandleDeath();

protected:
	virtual void BeginPlay() override;

private:

	TArray<ABaseCombatActor*> WeaponArray;
	TArray<ABaseCombatActor*> AbilityArray;

	APlayableCharacter* Owner;

	uint8 CurrentWeaponIndex;
	uint8 CurrentAbilityIndex;

	ABaseCombatActor* CurrentWeapon;
	ABaseCombatActor* CurrentAbility;
		
};
