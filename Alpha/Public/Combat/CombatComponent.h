// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Runtime/Engine/Classes/Materials/Material.h"
#include "CombatComponent.generated.h"

class ACombatActor;
class APlayableCharacter;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UCombatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UCombatComponent();

	void UseCurrentWeapon(FVector InLocation, FRotator InRotation);

	UFUNCTION(BlueprintCallable)
	void CycleNextWeapon();

	void CyclePreviousWeapon();
	void GetWeaponAt(uint8 WeaponIndex);

	void UseCurrentAbility(FVector InLocation, FRotator InRotation);
	void CycleNextAbility();
	void CyclePreviousAbility();
	void GetAbilityAt(uint8 WeaponIndex);

	void SetCurrentWeapon(ACombatActor* InActor);

	void DestroyThisComponent();
	APlayableCharacter* GetOwner();
	void SetOwner(APlayableCharacter* InChar);

protected:
	TArray<ACombatActor*> WeaponArray;
	TArray<ACombatActor*> AbilityArray;
	ACombatActor* CurrentWeapon;
	ACombatActor* CurrentAbility;
	APlayableCharacter* Owner;
	uint8 AbilityCount;
	uint8 WeaponCount;
	uint8 CurrentWeaponIndex;

	bool bCanUseActor;
	bool bIsUsingActor;

public:	
	UFUNCTION(BlueprintCallable)
		void AddWeapon(ACombatActor* InActor, UMaterial* InWeaponMaterial, UStaticMesh* InStaticMesh, FString SocketLocation);

	UFUNCTION(BlueprintCallable)
		void RemoveWeapon(uint8 WeaponIndex);

	UFUNCTION(BlueprintCallable, Category = "Game")
		void AssignCombatActors();

	UFUNCTION(BlueprintCallable)
		void AddAbility(ACombatActor* InActor);

	UFUNCTION(BlueprintCallable)
		void RemoveAbility(uint8 AbilityIndex);
		
};
