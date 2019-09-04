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
	ULogger::ScreenMessage(FColor::Green, "Applying Effects");
}

void AModifier::AssignValues(FModifierDataStruct InData)
{
	ModifierData = InData;
}
