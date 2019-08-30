// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Runtime/Engine/Classes/Materials/Material.h"
#include "CombatComponent.generated.h"

class UCombatUtils;
class ACombatActor;
class ACombatAbility;
class ACombatWeapon;
class ABuffEffect;
class ADebuffEffect;
class ACrowdControlEffect;
class APlayableCharacter;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UCombatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UCombatComponent();

	UFUNCTION(BlueprintCallable)
	APlayableCharacter* GetCharacterOwner();

	UFUNCTION(BlueprintCallable)
	void CycleNextWeapon();
	void UseCurrentWeapon();	
	void CyclePreviousWeapon();
	void GetWeaponAt(uint8 WeaponIndex);
	void SetCurrentWeapon(ACombatActor* InActor, bool bEquipAnim);
	ACombatActor* GetCurrentWeapon();

	UFUNCTION(BlueprintCallable)
	void CycleNextAbility();
	void UseCurrentAbility();
	void CyclePreviousAbility();
	void GetAbilityAt(uint8 AbilityIndex);
	void SetCurrentAbility(ACombatActor* InActor, bool bEquipAnim);
	ACombatActor* GetCurrentAbility();

	void SetCharacterOwner(APlayableCharacter* InChar);

	APlayableCharacter* CharacterOwner;

	UPROPERTY(EditDefaultsOnly)
	TArray < TSubclassOf<class ACombatActor >> WeaponClassArray;

	UPROPERTY(EditDefaultsOnly)
	TArray < TSubclassOf<class ACombatActor >> AbilityClassArray;

	TArray<ABuffEffect*> ActiveBuffs;
	TArray<ADebuffEffect*> ActiveDebuffs;

protected:

	bool bCanUseActor;

private:
	TArray<ACombatActor*> WeaponArray;
	TArray<ACombatActor*> AbilityArray;
	TArray<ACrowdControlEffect*> ActiveCrowdControl;

	uint8 CurrentWeaponIndex;
	uint8 CurrentAbilityIndex;
	uint8 AbilityCount;
	uint8 WeaponCount;

	ACombatActor* CurrentWeapon;
	ACombatActor* CurrentAbility;

	bool bIsUsingActor;
		
};
