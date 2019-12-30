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

UConditionTree* AModifier::GetConditionTree()
{
	return Context.Conditions;
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

void AModifier::SetOriginatingActor(AActor* InActor)
{
	OriginatingActor = InActor;
}

AActor* AModifier::GetOriginatingActor()
{
	return OriginatingActor;
}

void AModifier::ApplyEffects(AActor* AffectedActor)
{
	for ( ABaseEffect* CurrEffect : Effects) {
		switch (ModifierData.FeedbackType)
		{
		default:
			break;
		case EFeedbackAppliedType::EFFECTS_APPLIED:
			CurrEffect->ApplyEffectsToActor(AffectedActor, true);
			break;
		case EFeedbackAppliedType::ACTIVE:
			CurrEffect->ApplyEffectsToActor(AffectedActor, false);
			break;
		case EFeedbackAppliedType::ALIVE:
			CurrEffect->ApplyEffectsToActor(AffectedActor, false);
			break;
		}
		
	}
}

void AModifier::PlayAllEffectFeedbacks()
{
	for (ABaseEffect* CurrEffect : Effects) {
		CurrEffect->ShowEffectFeedback(ActorOwner);
	}
}

void AModifier::AssignValues(FModifierDataStruct InData)
{
	ModifierData = InData;
	FeedbackType = InData.FeedbackType;
	Context.bHasDuration = ModifierData.bHasDuration;
	Context.bIsActive = false;
	Context.Duration = ModifierData.Duration;
	Context.Conditions = DuplicateObject<UConditionTree>(ModifierData.Conditions, this);
	Context.Conditions->SetModifierOwner(this);
	Context.Conditions->InitExpressions();
	for (int i = 0; i < ModifierData.BaseEffectData.Num(); i++) {
		FActorSpawnParameters SpawnInfo;
		ABaseEffect* TempEffect;
		TempEffect = Cast<ABaseEffect>(GetWorld()->SpawnActor<ABaseEffect>(ModifierData.BaseEffectData[i]->BaseEffectDataStruct.EffectClass, FVector::ZeroVector, FRotator::ZeroRotator, SpawnInfo));
		if (TempEffect) {
			TempEffect->AssignValues(ModifierData.BaseEffectData[i]);
			TempEffect->SetModifierOwner(this);
			Effects.Add(TempEffect);
		}
		else {
			ULogger::ScreenMessage(FColor::Cyan, "TempEffect NULL");
		}
	}
}

void AModifier::Deactivate(AActor* InActor)
{
	for (ABaseEffect* CurrEff : Effects) {
		CurrEff->DeactivateEffect(InActor);
	}
}
