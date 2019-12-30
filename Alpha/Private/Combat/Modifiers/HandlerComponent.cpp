#include "HandlerComponent.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "Logger.h"
#include "Runtime/CoreUObject/Public/UObject/UObjectGlobals.h"
#include "Modifier.h"
#include "BaseEffect.h"
#include "Kismet/GameplayStatics.h"

UHandlerComponent::UHandlerComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UHandlerComponent::BeginPlay()
{
	Super::BeginPlay();
	TickRate = .5f;
	ActorOwner->GetWorldTimerManager().SetTimer(ManualTickTimer, this, &UHandlerComponent::ManualTick, TickRate, true);
}

void UHandlerComponent::ActivateModifier(AModifier* InModifier, AActor* Origin)
{
	if (Entries.Num() + 1 <= 30) {
		FEntry NewEntry;
		NewEntry = FEntry();
		NewEntry.bIsConditionTrue = false;
		NewEntry.StartTime = GetWorld()->GetTimeSeconds();
		NewEntry.Origin = Origin;
		FActorSpawnParameters SpawnInfo;
		NewEntry.Modifier = GetWorld()->SpawnActor<AModifier>(InModifier->ModifierData.ModifierClass, FVector::ZeroVector, FRotator::ZeroRotator);
		NewEntry.Modifier->AssignValues(InModifier->ModifierData);
		NewEntry.Modifier->SetActorOwner(ActorOwner);
		NewEntry.Modifier->SetOriginatingActor(Origin);
		NewEntry.Modifier->FeedbackType = InModifier->FeedbackType;
		Entries.Add(NewEntry);
		if (NewEntry.Modifier->FeedbackType == EFeedbackAppliedType::ACTIVE) {
			NewEntry.Modifier->PlayAllEffectFeedbacks();
		}
		UpdateModifiers();
	} 
}

void UHandlerComponent::RemoveEntry(int32 InIndex)
{
	if (Entries.Num() <= InIndex + 1 && InIndex >= 0) {
		AModifier* RemovedMod = Entries[InIndex].Modifier;
		RemovedMod->Deactivate(ActorOwner);
		Entries.RemoveAt(InIndex);
	}
}

void UHandlerComponent::UpdateModifiers()
{
	TArray<int32> RemoveItems;
	for (int i = 0; i < Entries.Num(); i++) {
		if (Entries[i].Modifier) {
			if (Entries[i].Modifier->ModifierData.bHasDuration) {
				if (!Entries[i].Modifier->GetConditionTree()) {
					if (Entries[i].Modifier->FeedbackType == EFeedbackAppliedType::ACTIVE) {
						Entries[i].Modifier->PlayAllEffectFeedbacks();
					}						
					Entries[i].Modifier->SetIsActive(true);
				}
				else {
					if (Entries[i].Modifier->AreConditionsTrue()) {
						if (Entries[i].Modifier->FeedbackType == EFeedbackAppliedType::ACTIVE) {
							Entries[i].Modifier->PlayAllEffectFeedbacks();
						}						
						Entries[i].Modifier->SetIsActive(true);
					}
					else {
						Entries[i].Modifier->SetIsActive(false);
					}
				}
				if (Entries[i].Modifier->IsActive()) {
					ApplyEffects(Entries[i].Modifier, ActorOwner);
				}
				float CurrentTime = GetWorld()->GetTimeSeconds();
				float EndTime = Entries[i].StartTime + Entries[i].Modifier->GetContext().Duration;
				if (EndTime <= CurrentTime) {
					RemoveItems.AddUnique(i);
				}
			}
			else {
				if (Entries[i].Modifier->AreConditionsTrue()) {
					ApplyEffects(Entries[i].Modifier, ActorOwner);
					RemoveItems.AddUnique(i);
				}
			}
		}
	}
	for (int32 CurrIndex : RemoveItems) {
		RemoveEntry(CurrIndex);
	}
}

void UHandlerComponent::ManualTick()
{
	UpdateModifiers();
}

void UHandlerComponent::ApplyEffects(AModifier* InModifier, AActor* AffectedActor)
{
	InModifier->ApplyEffects(AffectedActor);
}

void UHandlerComponent::SetActorOwner(AActor* InOwner)
{
	ActorOwner = InOwner;
}
