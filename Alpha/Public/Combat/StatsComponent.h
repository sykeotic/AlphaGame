// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameplayUtils.h"
#include "StatsComponent.generated.h"

class APlayableCharacter;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ALPHA_API UStatsComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UStatsComponent();

	void TakeDamage(float Damage);
	void Heal(float HealAmount);

	bool IsAlive();
	void SetAlive(bool IsAlive);

	float GetCurrentHealth();
	void SetCurrentHealth(float InCurrHealth);

	float GetMaxHealth();
	void SetMaxHealth(float InMaxHealth);

	void DisplayCurrentHealth();

	EArmorType GetArmorType();

	void SetOwner(APlayableCharacter* InChar);
	APlayableCharacter* GetOwner();

protected:

private:	
	bool bIsAlive;
	float MaxHealth;

	APlayableCharacter* Owner;
			
	UPROPERTY(Replicated)
	float CurrentHealth;

	UPROPERTY(EditDefaultsOnly, Category = "Stats")
	EArmorType ArmorType;
};
