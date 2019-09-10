#include "BaseEffect.h"
#include "Logger.h"

void ABaseEffect::ApplyEffectsToActor(AActor* AffectedActor)
{

}

void ABaseEffect::AssignValues(UBaseEffectData* InData)
{
	BaseEffectData = InData->BaseEffectDataStruct;
}
