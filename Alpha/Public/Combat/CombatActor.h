#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CombatComponent.h"
#include "Runtime/Engine/Classes/Animation/AnimMontage.h"
#include "Runtime/Engine/Classes/Components/BoxComponent.h"
#include "CombatActor.generated.h"

UCLASS(ABSTRACT)
class ACombatActor : public AActor
{
	GENERATED_BODY()
	
public:	
	ACombatActor();
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Instanced, Category = "Mesh")
	UStaticMeshComponent* MeshComp;

	UPROPERTY(EditDefaultsOnly)
	UAnimMontage* EquipAnim;

	UPROPERTY(EditDefaultsOnly)
	UAnimMontage* FireAnim;

	UPROPERTY(EditDefaultsOnly)
	UMaterial* WeaponMaterial;

	UFUNCTION(BlueprintCallable)
	UCombatComponent* GetCombatComponentOwner();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	TSubclassOf<UDamageType> DamageType;

	UFUNCTION()
	void OnRep_BurstCounter();

	FName ProjectileSpawnLocation;

	bool bPlayingFireAnim;
	bool bWantsToUse;
	bool bRefiring;
	bool bIsEquipped;
	bool bPendingEquip;

	float UseRange;
	float UseCooldown;
	float Damage;
	float EquipStartedTime;
	float EquipDuration;
	float LastFireTime;
	float TimeBetweenShots;

	UPROPERTY(Transient, ReplicatedUsing = OnRep_BurstCounter)
	int32 BurstCounter;

	FTimerHandle TimerHandle_HandleFiring;
	FTimerHandle EquipFinishedTimerHandle;

	UCombatComponent* ComponentOwner;

	EActorType ACTOR_TYPE;
	EValidTargets VALID_TARGETS;
	ERange RANGE_TYPE;
	ECombatActorState ACTOR_STATE;

	float PlayActorAnimation(UAnimMontage* Animation, float InPlayRate = 1.f, FName StartSectionName = NAME_None);

	void StopActorAnimation(UAnimMontage* Animation);

	void SetCombatComponentOwner(UCombatComponent* InComp);

	bool CanUse();

	void AssertActorState();

	void OnBurstStarted();

	void OnBurstFinished();

	void OnEquipFinished();

	void HandleUse();

	void StartSimulatingActorUse();

	void AttachMeshToOwner(FName AttachPoint);

	void OnUnEquip();

	void DetachMeshFromOwner();

	void StopSimulatingActorUse();

	void SetCombatActorState(ECombatActorState InState);

	void OnEquip(bool bPlayAnimation);

	virtual void OnUse();

	void StopUse();

	void BoolSpam();

	virtual void AssignWeaponValues(float InCooldown, UStaticMesh* InStaticMesh, FName InProjectileSpawnLocation, ERange IN_RANGE, EActorType IN_ACTOR_TYPE, float InDmg, float InRange);
};
