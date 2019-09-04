#include "HandlerComponent.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "Logger.h"
#include "Modifier.h"
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

void UHandlerComponent::ActivateModifier(AModifier * InModifier, AActor* Origin)
{
	if (Entries.Num() + 1 >= 30 || !InModifier->GetContext().bHasDuration) {
		FEntry NewEntry;
		NewEntry = FEntry();
		NewEntry.bIsConditionTrue = false;
		NewEntry.StartTime = GetWorld()->GetTimeSeconds();
		NewEntry.Origin = Origin;
		NewEntry.Modifier = InModifier;
		Entries.Add(NewEntry);
		UpdateModifiers();
	}
}

void UHandlerComponent::RemoveEntry(int32 InIndex)
{
	Entries.RemoveAt(InIndex);
}

void UHandlerComponent::UpdateModifiers()
{
	TArray<int32> RemoveItems;
	for (int i = 0; i < Entries.Num(); i++) {
		if (Entries[i].Modifier->IsActive()) {
			ApplyEffects(Entries[i].Modifier);
		}
		else {
			Entries[i].Modifier->AreConditionsTrue() == true ? Entries[i].Modifier->SetIsActive(true) : Entries[i].Modifier->SetIsActive(false);
		}

		float CurrentTime = GetWorld()->GetTimeSeconds();
		float EndTime = Entries[i].StartTime + Entries[i].Modifier->GetContext().Duration;
		bool bPersistent = Entries[i].Modifier->GetContext().bHasDuration;
		if ( !bPersistent || EndTime <= CurrentTime) {
			RemoveItems.AddUnique(i);
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

void UHandlerComponent::ApplyEffects(AModifier* InModifier)
{
	InModifier->ApplyEffects();
}

void UHandlerComponent::SetActorOwner(AActor* InOwner)
{
	ActorOwner = InOwner;
}
