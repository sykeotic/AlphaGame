#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Runtime/Engine/Classes/Animation/AnimMontage.h"
#include "Runtime/Engine/Classes/Sound/SoundCue.h"
#include "Runtime/Engine/Classes/Components/BoxComponent.h"
#include "GameplayUtils.h"
#include "CombatActor.generated.h"

USTRUCT(BlueprintType)
struct FCombatActorData : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UStaticMeshComponent* MeshComp;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UMaterial* WeaponMaterial;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FRotator ActorRotation;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FVector ActorLocation;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FName ActorSocketLocation;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UParticleSystem* ActorFX;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<UAnimMontage*> FireAnim;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UAnimMontage* EquipAnim;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<USoundCue*> UseSound;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<USoundCue*> ImpactSound;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		USoundCue* EquipSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FName ProjectileSpawnLocation;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<UDamageType> DamageType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray < TSubclassOf<class ACombatWeapon >> WeaponClassArray;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<class ACombatProjectile> ProjectileClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<class AThrowableProjectile> ThrowableProjectileClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool bPlaySoundEveryTime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float UseRange;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float UseCooldown;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float Damage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float SFXBuffer;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float ArmorPierce;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float EquipDuration;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float LastFireTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		EActorType ACTOR_TYPE;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		EValidTargets VALID_TARGETS;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		ERange RANGE_TYPE;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		ECombatActorState ACTOR_STATE;
};

class UCombatComponent;
class APlayableCharacter;

UCLASS(ABSTRACT)
class ACombatActor : public AActor
{
	GENERATED_BODY()
	
public:	
	ACombatActor();
	
	UPROPERTY(EditDefaultsOnly)
	UStaticMeshComponent* MeshComp;

	UPROPERTY(EditDefaultsOnly)
	UAnimMontage* EquipAnim;

	UAnimMontage* CurrentAnim;

	USoundCue* CurrentSound;

	UPROPERTY(EditDefaultsOnly)
	TArray<UAnimMontage*> FireAnim;

	UPROPERTY(EditDefaultsOnly)
	UMaterial* WeaponMaterial;

	UFUNCTION(BlueprintCallable)
	UCombatComponent* GetCombatComponentOwner();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	TSubclassOf<UDamageType> DamageType;

	UPROPERTY(EditDefaultsOnly)
	TArray < TSubclassOf<class ACombatWeapon >> WeaponClassArray;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class ACombatProjectile> ProjectileClass;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class AThrowableProjectile> ThrowableProjectileClass;

	UFUNCTION()
	void OnRep_BurstCounter();

	UPROPERTY(EditDefaultsOnly, Category = "Stats")
	FName ProjectileSpawnLocation;

	UPROPERTY(EditDefaultsOnly, Category = "Stats")
	FRotator WeaponRotation;
	
	UPROPERTY(EditDefaultsOnly, Category = "Stats")
	FVector WeaponLocation;

	UPROPERTY(EditDefaultsOnly, Category = "Sounds")
	TArray<USoundCue*> UseSound;

	UPROPERTY(EditDefaultsOnly, Category = "Sounds")
		TArray<USoundCue*> ImpactSound;

	UPROPERTY(EditDefaultsOnly, Category = "Sounds")
	USoundCue* EquipSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SFX")
	UParticleSystem* ActorFX;

	UPROPERTY(Transient)
	UParticleSystemComponent* UsePSC;

	bool bPlayingFireAnim;
	bool bWantsToUse;
	bool bRefiring;
	bool bIsEquipped;
	bool bPendingEquip;
	bool bPlayingSound;
	bool bFirstUse;

	UPROPERTY(EditDefaultsOnly, Category = "Stats")
	bool bPlaySoundEveryTime;

	UPROPERTY(EditDefaultsOnly, Category = "Stats")
	float UseRange;

	UPROPERTY(EditDefaultsOnly, Category = "Stats")
	float UseCooldown;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Stats")
	float Damage;

	float NextValidFireTime;

	UPROPERTY(EditDefaultsOnly, Category = "Stats")
	float SFXBuffer;

	UPROPERTY(EditDefaultsOnly, Category = "Stats")
	float ArmorPierce;

	float EquipStartedTime;

	UPROPERTY(EditDefaultsOnly, Category = "Stats")
	float EquipDuration;

	float LastFireTime;

	UPROPERTY(Transient, ReplicatedUsing = OnRep_BurstCounter)
	int32 BurstCounter;

	FTimerHandle TimerHandle_HandleFiring;
	FTimerHandle EquipFinishedTimerHandle;
	FTimerHandle AnimationTimer;
	FTimerHandle SoundTimer;
	FTimerHandle SFXTimer;

	FName ActorSocketLocation;

	UCombatComponent* ComponentOwner;

	EActorType ACTOR_TYPE;
	EValidTargets VALID_TARGETS;
	ERange RANGE_TYPE;
	ECombatActorState ACTOR_STATE;

	float PlayActorAnimation(UAnimMontage* Animation, float InPlayRate = 1.f, FName StartSectionName = NAME_None);

	void StopActorAnimation(UAnimMontage* Animation);

	void SetCombatComponentOwner(UCombatComponent* InComp);

	bool CanUse();

	virtual void BeginPlay() override;

	void AssertActorState();

	UAudioComponent* PlayActorSound(USoundCue* SoundToPlay);

	USoundCue* PickRandomSound(TArray<USoundCue*> InSoundArray);

	UAnimMontage* PickRandomAnim(TArray < UAnimMontage*> InAnimArray);

	void OnBurstStarted();

	void OnBurstFinished();

	void OnEquipFinished();

	void HandleUse();

	void StartSimulatingActorUse();

	virtual void AttachMeshToOwner(FName AttachPoint);

	void OnUnEquip();

	void DetachMeshFromOwner();

	void StopSimulatingActorUse();

	void SetSoundPlayingFalse();

	void StartSFX();

	void SetCombatActorState(ECombatActorState InState);

	void OnEquip(bool bPlayAnimation);

	virtual void OnUse();

	virtual void ExecuteUse();

	void StopUse();

	void BoolSpam();

	float ResolveDamageModifiers(APlayableCharacter* OffensivePlayer, APlayableCharacter* DefensivePlayer, ACombatActor* OffensiveCombatActor);

	virtual void AssignWeaponValues(float InCooldown, UStaticMesh* InStaticMesh, FName InProjectileSpawnLocation, ERange IN_RANGE, EActorType IN_ACTOR_TYPE, float InDmg, float InRange);
};
