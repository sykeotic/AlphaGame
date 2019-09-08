#include "Modifier.h"
#include "Logger.h"
#include "BaseEffect.h"
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

int32 AModifier::GetEffectCount()
{
	return Effects.Num();
}

void AModifier::SetActorOwner(AActor* InActor)
{
	ActorOwner = InActor;
}

AActor* AModifier::GetActorOwner()
{
	return ActorOwner;
}

void AModifier::ApplyEffects(AActor* AffectedActor)
{
	ULogger::ScreenMessage(FColor::Cyan, "Modifier::ApplyEffects || Effects.Size(): " + FString::FromInt(Effects.Num()));
	for ( ABaseEffect* CurrEffect : Effects) {
		CurrEffect->ApplyEffectsToActor(AffectedActor);		
	}
}

void AModifier::AssignValues(FModifierDataStruct InData)
{
	ModifierData = InData;
	Context.bHasDuration = ModifierData.bHasDuration;
	Context.bIsActive = false;
	Context.Duration = ModifierData.Duration;
	Context.Conditions = ModifierData.Conditions;
	Context.Conditions->SetModifierOwner(this);
	Context.Conditions->InitExpressions();
	for (int i = 0; i < ModifierData.BaseEffectData.Num(); i++) {
		FActorSpawnParameters SpawnInfo;
		ABaseEffect* TempEffect;
		TempEffect = Cast<ABaseEffect>(GetWorld()->SpawnActor<ABaseEffect>(ModifierData.BaseEffectData[i]->BaseEffectDataStruct.EffectClass, FVector::ZeroVector, FRotator::ZeroRotator, SpawnInfo));
		if (TempEffect) {
			TempEffect->AssignValues(ModifierData.BaseEffectData[i]);
			Effects.Add(TempEffect);
			ULogger::ScreenMessage(FColor::Cyan, "TempEffect Added");
		}
		else {
			ULogger::ScreenMessage(FColor::Cyan, "TempEffect NULL");
		}
	}
	ULogger::ScreenMessage(FColor::Cyan, "Modifier::AssignValues || Effects.Size(): " + FString::FromInt(Effects.Num()));
}
