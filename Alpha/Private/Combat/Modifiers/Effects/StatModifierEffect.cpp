#include "StatModifierEffect.h"
#include "StatModifierEffectData.h"
#include "StatsComponent.h"

void AStatModifierEffect::ApplyEffectsToActor(AActor* AffectedActor, bool bPlayFeedback)
{
	Super::ApplyEffectsToActor(AffectedActor, bPlayFeedback);
	UStatsComponent* TempStats = AffectedActor->FindComponentByClass<UStatsComponent>();
	if (StatModifierData.Type == EStatModifierType::HP) {
		if (TempStats) {
			TempStats->AdjustHealth(-StatModifierData.ModifierOperand);
		}
	}
}

void AStatModifierEffect::AssignValues(UBaseEffectData* InData)
{
	Super::AssignValues(InData);
	UStatModifierEffectData* TempData = Cast<UStatModifierEffectData>(InData);
	StatModifierData = TempData->StatModifierEffectDataStruct;
}

void AStatModifierEffect::DeactivateEffect(AActor* AffectedActor)
{

}
