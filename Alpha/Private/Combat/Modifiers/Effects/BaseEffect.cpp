#include "BaseEffect.h"
#include "Logger.h"

void ABaseEffect::ApplyEffectsToActor(AActor* AffectedActor)
{
	ULogger::ScreenMessage(FColor::Orange, "BaseEffect::ApplyEffects");
}

void ABaseEffect::AssignValues(UBaseEffectData* InData)
{
	BaseEffectData = InData->BaseEffectDataStruct;
}
