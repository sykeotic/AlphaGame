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

class AModifier;
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
	UCombatComponent* GetCombatComponentOwner();

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

	TArray<AModifier*> GetModifiers();
	void InitModifiers();
	void ApplyModifiers(AActor* InActor);

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

	TArray<AModifier*> Modifiers;

protected:
	virtual void BeginPlay() override;

	UStaticMeshComponent* MeshComp;
	UCombatComponent* ComponentOwner;
	UAnimMontage* CurrentAnim;
	ECombatActorState ACTOR_STATE;

private:
	float EquipStartedTime;
	float EquipDuration;
	float NextValidFireTime;
	float LastFireTime;
	float BurstCounter;
	
	bool bPendingEquip;
	bool bIsEquipped;
	bool bRefiring;
	bool bPlayingUseAnimation;
	bool bPlayingSound;
	bool bPlaySoundEveryTime;
	bool bWantsToUse;

	FTimerHandle EquipFinishedTimerHandle;
	FTimerHandle TimerHandle_HandleFiring;
	FTimerHandle AnimationTimerHandle;
	FTimerHandle VisualFXTimerHandle;
	FTimerHandle SoundTimerHandle;
};
