#include "ModifyTagEffect.h"
#include "Logger.h"
#include "ModifyTagEffectData.h"

void AModifyTagEffect::ApplyEffectsToActor(AActor* AffectedActor)
{
	if (ModifyTagData.Type == EModifyTagType::TAUNT) {
		ULogger::ScreenMessage(FColor::Emerald, "TAUNTED");
	}
	else {
		ULogger::ScreenMessage(FColor::Emerald, "NO TAUNT");
	}
}

void AModifyTagEffect::AssignValues(UBaseEffectData* InData)
{
	Super::AssignValues(InData);
	UModifyTagEffectData* TempData = Cast<UModifyTagEffectData>(InData);
	ModifyTagData = TempData->ModifyTagEffectDataStruct;
}
