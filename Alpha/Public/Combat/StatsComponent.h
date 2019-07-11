// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
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

	void SetAlive(bool IsAlive);
	void SetCurrentHealth(float InCurrHealth);
	void SetMaxHealth(float InMaxHealth);
	void DisplayCurrentHealth();

	float GetCurrentHealth();
	float GetMaxHealth();
	bool IsAlive();

	void SetOwner(APlayableCharacter* InChar);
	APlayableCharacter* GetOwner();

	bool bIsAlive;

	UPROPERTY(Replicated)
	float CurrentHealth;

	float MaxHealth;
	APlayableCharacter* Owner;

protected:

public:	
			
};
