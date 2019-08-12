#include "StatsComponent.h"
#include "UnrealNetwork.h"
#include "GameplayUtils.h"
#include "Logger.h"

UStatsComponent::UStatsComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UStatsComponent::DisplayCurrentHealth() {
	ULogger::ScreenMessage(FColor::Red, "Health After: " + FString::SanitizeFloat(CurrentHealth));
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

EArmorType UStatsComponent::GetArmorType() {
	return ArmorType;
}

void UStatsComponent::TakeDamage(float Damage) {
	ULogger::ScreenMessage(FColor::Red, "Health Before: " + FString::SanitizeFloat(CurrentHealth));
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

void UStatsComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const {
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UStatsComponent, CurrentHealth);
}
