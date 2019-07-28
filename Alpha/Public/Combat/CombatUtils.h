// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "PlayableCharacter.h"
#include "CombatUtils.generated.h"

class ACombatActor;

UCLASS()
class ALPHA_API UCombatUtils : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	
	static float CalculateDamage(float InDamage, APlayableCharacter* DefendingCharacter, APlayableCharacter* OffensivePlayer, ACombatActor* AttackingCombatActor);

private:

	static float ResolveAttackModifier(APlayableCharacter* InChar, float InDamage);

	static float ResolveDefenseModifier(APlayableCharacter* InChar, float InDamage);

	static float ResolveArmorModifier(float InDamage, APlayableCharacter* DefendingCharacter, APlayableCharacter* OffensivePlayer, ACombatActor* AttackingCombatActor);

	static float ResolveReducedDamageFromArmor(float InDamage, float ArmorPierce, APlayableCharacter* DefendingCharacter);

	static float ResolveArmorPierce(float BaseModifier, float ArmorPierce);
};
