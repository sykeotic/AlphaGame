// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/NonPlayableActors/OverlapTriggerActor.h"
#include "Runtime/Engine/Classes/Sound/SoundCue.h"
#include "ObjectiveOverlapActor.generated.h"

class AHumanPlayerController;
class UTeamComponent;

UENUM(BlueprintType)
enum class EObjectiveState : uint8 {
	CAPTURING UMETA(DisplayName = "Capturing"),
	CAPTURED UMETA(DisplayName = "Captured"),
	NEUTRAL UMETA(DisplayName = "Neutral")
};

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

	UPROPERTY(EditDefaultsOnly, Category = "Sounds")
	USoundCue* CapturedSound;

	class UAudioComponent* PlayActorSound(USoundCue* SoundToPlay);

	virtual void BeginPlay() override;

	UFUNCTION()
	virtual void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

	UFUNCTION()
	virtual void OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	void AssertObjectiveState();
	void StartCapturing();
	void ResetObjectiveFinished();
	void ResetObjective();
	void TimerTick();
	void AdjustModifier();
	void HandleCapture();

	void CreateCaptureDisplay(AHumanPlayerController* InController);
	void DestroyCaptureDisplay(AHumanPlayerController* InController);

	void ChangeState(EObjectiveState InState);

	UPROPERTY(EditDefaultsOnly)
	float RequiredCaptureScore;

	float CaptureModifier;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	float PercentProgress;

	UPROPERTY(EditInstanceOnly)
	int32 StartingTeam;

	float CurrentCaptureScore;

	bool bResetting;

	FTimerHandle CaptureTimer;
	FTimerHandle ResetTimer;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TArray<AActor*> InZoneActors;

	EObjectiveState OBJECTIVE_STATE;
	EObjectiveState PREV_STATE;

	UTeamComponent* OwningTeam;
	UTeamComponent* ContestingTeam;
};
