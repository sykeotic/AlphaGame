#include "StatModifierEffect.h"
#include "StatModifierEffectData.h"
#include "StatsComponent.h"

void AStatModifierEffect::ApplyEffectsToActor(AActor* AffectedActor)
{
	UStatsComponent* TempStats = AffectedActor->FindComponentByClass<UStatsComponent>();
	TempStats->AdjustHealth(-StatModifierData.ModifierOperand);
}

void AStatModifierEffect::AssignValues(UBaseEffectData* InData)
{
	Super::AssignValues(InData);
	UStatModifierEffectData* TempData = Cast<UStatModifierEffectData>(InData);
	StatModifierData = TempData->StatModifierEffectDataStruct;
}
