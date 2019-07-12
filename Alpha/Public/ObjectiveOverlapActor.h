// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/NonPlayableActors/OverlapTriggerActor.h"
#include "ObjectiveOverlapActor.generated.h"

/**
 * 
 */
UCLASS()
class ALPHA_API AObjectiveOverlapActor : public AOverlapTriggerActor
{
	GENERATED_BODY()

public:

	AObjectiveOverlapActor();

	UPROPERTY(EditDefaultsOnly)
	UStaticMeshComponent* MeshComp;

	UPROPERTY(EditDefaultsOnly)
	USphereComponent* SphereComponent;

	virtual void BeginPlay() override;

	UFUNCTION()
	virtual void OnOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

	class UTeamComponent* OwningTeam;
};
