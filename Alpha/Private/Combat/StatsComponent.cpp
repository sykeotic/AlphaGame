#include "StatsComponent.h"
#include "Logger.h"

UStatsComponent::UStatsComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UStatsComponent::DisplayCurrentHealth() {
	ULogger::ScreenMessage(FColor::Red, FString::SanitizeFloat(CurrentHealth));
}

float UStatsComponent::GetCurrentHealth() {
	return CurrentHealth;
}

void UStatsComponent::SetMaxHealth(float InMaxHealth) {
	MaxHealth = InMaxHealth;
}

void UStatsComponent::SetCurrentHealth(float InHealth) {
	CurrentHealth = InHealth;
}

bool UStatsComponent::IsAlive() {
	return bIsAlive;
}

void UStatsComponent::TakeDamage(float Damage) {
	CurrentHealth -= Damage;
	if (CurrentHealth <= 0) {
		bIsAlive = false;
	}
	else {
		bIsAlive = true;
	}
	DisplayCurrentHealth();
}

void UStatsComponent::Heal(float HealAmount) {
	if (HealAmount >= 0 && (HealAmount + CurrentHealth) <= MaxHealth) {
		CurrentHealth += HealAmount;
	}
	else if (HealAmount >= 0 && HealAmount + CurrentHealth > MaxHealth) {
		CurrentHealth = MaxHealth;
	}
	DisplayCurrentHealth();
}

void UStatsComponent::SetAlive(bool InIsAlive) {
	bIsAlive = InIsAlive;
}

void UStatsComponent::SetOwner(APlayableCharacter* InChar) {
	Owner = InChar;
}

APlayableCharacter* UStatsComponent::GetOwner() {
	return Owner;
}

float UStatsComponent::GetMaxHealth() {
	return MaxHealth;
}