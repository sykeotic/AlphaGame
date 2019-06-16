#include "StatsComponent.h"

UStatsComponent::UStatsComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UStatsComponent::TakeDamage(float Damage) {

}

void UStatsComponent::SetOwner(APlayableCharacter* InChar) {

}

APlayableCharacter* UStatsComponent::GetOwner() {
	return Owner;
}