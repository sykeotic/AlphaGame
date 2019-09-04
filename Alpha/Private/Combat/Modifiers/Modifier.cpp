#include "Modifier.h"
#include "ConditionTree.h"

AModifier::AModifier()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AModifier::BeginPlay()
{
	Super::BeginPlay();
}

bool AModifier::AreConditionsTrue()
{
	return Context.Conditions->IsConditionTreeTrue();
}

bool AModifier::IsActive()
{
	return Context.bIsActive;
}

void AModifier::SetIsActive(bool bActiveStatus)
{
	Context.bIsActive = bActiveStatus;
}

FContext AModifier::GetContext()
{
	return Context;
}

void AModifier::ApplyEffects()
{
	ULogger::ScreenMessage(FColor::Orange, "APPLYING EFFECTS");
}

void AModifier::AssignValues(FModifierDataStruct InData)
{
	ModifierData = InData;
	Context.bHasDuration = ModifierData.bHasDuration;
	Context.bIsActive = false;
	Context.Duration = ModifierData.Duration;
	Context.Conditions = ModifierData.Conditions;
}
