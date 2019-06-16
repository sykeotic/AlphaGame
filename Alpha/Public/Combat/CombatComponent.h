// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CombatComponent.generated.h"

class ACombatActor;
class APlayableCharacter;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UCombatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UCombatComponent();

	void UseCurrentWeapon();
	void CycleNextWeapon();
	void CyclePreviousWeapon();
	void GetWeaponAt(int8 WeaponIndex);
	void AddWeapon(ACombatActor* InActor);
	void RemoveWeapon(int8 WeaponIndex);

	void UseCurrentAbility();
	void CycleNextAbility();
	void CyclePreviousAbility();
	void GetAbilityAt(int8 WeaponIndex);
	void AddAbility(ACombatActor* InActor);
	void RemoveAbility(int8 AbilityIndex);

	void DestroyThisComponent();
	APlayableCharacter* GetOwner();
	void SetOwner(APlayableCharacter* InChar);

	UFUNCTION(BlueprintCallable, Category = "Game")
	void AssignCombatActors();

	TArray<ACombatActor*> WeaponArray;
	TArray<ACombatActor*> AbilityArray;
	ACombatActor* CurrentWeapon;
	ACombatActor* CurrentAbility;
	APlayableCharacter* Owner;

	bool bCanUseActor;
	bool bIsUsingActor;

protected:

public:	
		
};
