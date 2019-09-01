// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseCombatActorData.h"
#include "BaseCombatActor.generated.h"

UENUM(BlueprintType)
enum class ECombatActorState : uint8 {
	USING UMETA(DisplayName = "Using"),
	IDLE UMETA(DisplayName = "Idle"),
	EQUIPPING UMETA(DisplayName = "Equipping"),
	RELOADING UMETA(DisplayName = "Reloading")
};

class UCombatComponent;
class UStaticMeshComponent;

UCLASS(ABSTRACT)
class ABaseCombatActor : public AActor
{
	GENERATED_BODY()
	
public:	
	ABaseCombatActor();

	FBaseCombatActorDataStruct BaseCombatActorData;

	UPROPERTY(Transient)
		UParticleSystemComponent* UsePSC;

	void SetCombatActorData(FBaseCombatActorDataStruct InData);
	void SetCombatComponentOwner(UCombatComponent* InComponent);

	void AttachMeshToOwner(FName AttachPoint);
	void DetachMeshFromOwner();

	virtual void OnUse();
	virtual void ExecuteUse();
	virtual void AssignValues(UBaseCombatActorData* InData);
	void StopUse();
	bool CanUse();
	void HandleUse();

	void OnEquip(bool bPlayEquipAnim);
	void OnEquipFinished();
	void OnUnEquip();

	void OnBurstStarted();
	void OnBurstFinished();

	void AssertActorState();
	void SetCombatActorState(ECombatActorState InState);

	void StartSimulatingActorUse();
	void StopSimulatingActorUse();
	void PlayVisualFX();
	UAudioComponent* PlaySoundFX(USoundCue* InSound);
	void SetSoundPlayingToFalse();
	float PlayActorAnimation(UAnimMontage* Animation, float InPlayRate = 1.f, FName StartSectionName = NAME_None);
	void StopActorAnimation(UAnimMontage* InAnim);


	UFUNCTION()
	void OnRep_BurstCounter();

	UStaticMeshComponent* GetMesh();

	float GetNextValidFireTime();

protected:
	virtual void BeginPlay() override;

private:
	UStaticMeshComponent* MeshComp;
	UCombatComponent* ComponentOwner;

	float EquipStartedTime;
	float EquipDuration;
	float NextValidFireTime;
	float LastFireTime;
	float BurstCounter;
	
	bool bPendingEquip;
	bool bIsEquipped;
	bool bWantsToUse;
	bool bRefiring;
	bool bPlayingUseAnimation;
	bool bPlayingSound;
	bool bPlaySoundEveryTime;

	UAnimMontage* CurrentAnim;

	FTimerHandle EquipFinishedTimerHandle;
	FTimerHandle TimerHandle_HandleFiring;
	FTimerHandle AnimationTimerHandle;
	FTimerHandle VisualFXTimerHandle;
	FTimerHandle SoundTimerHandle;

	ECombatActorState ACTOR_STATE;
};