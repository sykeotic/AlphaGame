// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HandlerComponent.generated.h"

class AModifier;
class APlayableCharacter;

USTRUCT(BlueprintType)
struct FEntry {

	GENERATED_BODY()

public:

	float StartTime;

	AActor* Origin;

	bool bIsConditionTrue;

	AModifier* Modifier;

	FEntry() {
		StartTime = 0.0f;
		Origin = nullptr;
		bIsConditionTrue = false;
		Modifier = nullptr;
	}

	FORCEINLINE bool operator==(const FEntry &InEntry) {
		return Origin == InEntry.Origin
			&& StartTime == InEntry.StartTime
			&& Modifier == InEntry.Modifier;
	}
};


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ALPHA_API UHandlerComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UHandlerComponent();

	// virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;

public:	
		
	void ActivateModifier(AModifier* InModifier, AActor* Origin);
	void RemoveEntry(int32 InIndex);
	void UpdateModifiers();

	void ManualTick();

	void ApplyEffects(AModifier* InModifier, AActor* AffectedActor);

	void SetActorOwner(AActor* InOwner);

private:
	float TickRate;

	TArray<FEntry> Entries;

	FTimerHandle ManualTickTimer;

	AActor* ActorOwner;

};
