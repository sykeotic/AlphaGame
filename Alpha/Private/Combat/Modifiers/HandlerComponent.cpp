#include "HandlerComponent.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "Logger.h"
#include "Runtime/CoreUObject/Public/UObject/UObjectGlobals.h"
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
	ULogger::ScreenMessage(FColor::Green, "HandlerComponent::ActivateModifiers - Before");
	if (Entries.Num() + 1 >= 30 || !InModifier->GetContext().bHasDuration) {
		ULogger::ScreenMessage(FColor::Green, "HandlerComponent::ActivateModifiers - After");
		FEntry NewEntry;
		NewEntry = FEntry();
		NewEntry.bIsConditionTrue = false;
		NewEntry.StartTime = GetWorld()->GetTimeSeconds();
		NewEntry.Origin = Origin;
		FActorSpawnParameters SpawnInfo;
		NewEntry.Modifier = GetWorld()->SpawnActor<AModifier>(InModifier->GetClass(), FVector::ZeroVector, FRotator::ZeroRotator);
		NewEntry.Modifier->ModifierData.bHasDuration = InModifier->ModifierData.bHasDuration;
		NewEntry.Modifier->ModifierData.Conditions = InModifier->ModifierData.Conditions;
		NewEntry.Modifier->ModifierData.Duration = InModifier->ModifierData.Duration;
		NewEntry.Modifier->ModifierData.Feedback = InModifier->ModifierData.Feedback;
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
	ULogger::ScreenMessage(FColor::Emerald, "Num Entries: " + FString::FromInt(Entries.Num()));
	for (int i = 0; i < Entries.Num(); i++) {
		if (Entries[i].Modifier->AreConditionsTrue()) {
			Entries[i].Modifier->SetIsActive(true);
		}
		else {
			Entries[i].Modifier->SetIsActive(false);
		}
		if (Entries[i].Modifier->IsActive()) {
			ApplyEffects(Entries[i].Modifier);
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
