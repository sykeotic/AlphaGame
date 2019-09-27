// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Combat/Actors/BaseCombatActor.h"
#include "Runtime/Engine/Classes/Components/BoxComponent.h"
#include "MeleeCombatActorData.h"
#include "MeleeCombatActor.generated.h"

/**
 * 
 */
UCLASS()
class ALPHA_API AMeleeCombatActor : public ABaseCombatActor
{
	GENERATED_BODY()

public:
	AMeleeCombatActor();

	virtual void OnUse() override;
	virtual void ExecuteUse() override;
	virtual void AssignValues(UBaseCombatActorData* InData) override;

	virtual void BeginPlay() override;

	UFUNCTION()
		void WeaponBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

private:
	UBoxComponent* MeleeBoxComponent;

	FTimerHandle OverlapWindowTimer;
	FTimerHandle ClearTimer;

	int32 SectionCounter;

	FMeleeCombatActorDataStruct MeleeCombatActorStruct;

	TArray<AActor*> OverlappedActors;

	bool bCanOverlap;

	void SetOverlappingToFalse();

	void ClearOverlappedArray();
};
