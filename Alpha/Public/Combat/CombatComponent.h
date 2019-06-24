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

	void UseCurrentWeapon();

	UFUNCTION(BlueprintCallable)
	void CycleNextWeapon();

	void CyclePreviousWeapon();
	void GetWeaponAt(uint8 WeaponIndex);

	void UseCurrentAbility(FVector InLocation, FRotator InRotation);
	void CycleNextAbility();
	void CyclePreviousAbility();
	void GetAbilityAt(uint8 AbilityIndex);

	void SetCurrentWeapon(ACombatWeapon* InActor);

	void DestroyThisComponent();
	APlayableCharacter* GetOwner();
	void SetOwner(APlayableCharacter* InChar);
	ACombatWeapon* CurrentWeapon;

protected:
	TArray<ACombatWeapon*> WeaponArray;
	TArray<ACombatAbility*> AbilityArray;
	ACombatAbility* CurrentAbility;
	APlayableCharacter* Owner;
	uint8 AbilityCount;
	uint8 WeaponCount;
	uint8 CurrentWeaponIndex;

	bool bCanUseActor;
	bool bIsUsingActor;

public:	
	UFUNCTION(BlueprintCallable)
		void AddWeapon(ACombatWeapon* InActor, FName SocketLocation);

	UFUNCTION(BlueprintCallable)
		void SpawnWeapon(float InCooldown, FVector InLocation, FRotator InRotation, UMaterial* InWeaponMaterial, UStaticMesh* InStaticMesh, FName SocketLocation, ERange IN_RANGE, EActorType IN_ACTOR_TYPE, FName ProjectileSpawnLocation, float Dmg, float InRange);

	UFUNCTION(BlueprintCallable)
		void RemoveWeapon(uint8 WeaponIndex);

	UFUNCTION(BlueprintCallable, Category = "Game")
		void AssignCombatActors();

	UFUNCTION(BlueprintCallable)
		void AddAbility(ACombatAbility* InActor);

	UFUNCTION(BlueprintCallable)
		void RemoveAbility(uint8 AbilityIndex);
		
};
