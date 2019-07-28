#include "StatusEffect.h"

AStatusEffect::AStatusEffect()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AStatusEffect::BeginPlay()
{
	Super::BeginPlay();
	
}

float AStatusEffect::AttackModifier(float InDamage) {
	return InDamage + 0.0f;
}

float AStatusEffect::DefenseModifier(float InDamage) {
	return InDamage + 0.0f;
}