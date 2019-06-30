// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CombatUtils.h"
#include "Runtime/Engine/Classes/Materials/Material.h"
#include "CombatComponent.generated.h"

class ACombatActor;
class ACombatAbility;
class ACombatWeapon;
class APlayableCharacter;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UCombatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UCombatComponent();

	UFUNCTION(BlueprintCallable)
	void CycleNextWeapon();

	void UseCurrentWeapon();
	void CyclePreviousWeapon();
	void GetWeaponAt(uint8 WeaponIndex);
	void SetCurrentWeapon(ACombatWeapon* InActor, bool bEquipAnim);

	UFUNCTION(BlueprintCallable)
	void CycleNextAbility();

	void UseCurrentAbility();
	void CyclePreviousAbility();
	void GetAbilityAt(uint8 AbilityIndex);
	void SetCurrentAbility(ACombatAbility* InActor, bool bEquipAnim);

	void SetCharacterOwner(APlayableCharacter* InChar);

	TArray<ACombatWeapon*> WeaponArray;
	TArray<ACombatAbility*> AbilityArray;

	uint8 CurrentWeaponIndex;
	uint8 CurrentAbilityIndex;
	uint8 AbilityCount;
	uint8 WeaponCount;

	ACombatWeapon* CurrentWeapon;
	ACombatAbility* CurrentAbility;
	
	APlayableCharacter* CharacterOwner;

	UPROPERTY(EditDefaultsOnly)
	TArray < TSubclassOf<class ACombatWeapon >> WeaponClassArray;

	UPROPERTY(EditDefaultsOnly)
	TArray < TSubclassOf<class ACombatAbility >> AbilityClassArray;

protected:

	bool bCanUseActor;
	bool bIsUsingActor;

public:	

	UFUNCTION(BlueprintCallable)
	APlayableCharacter* GetCharacterOwner();
		
};
