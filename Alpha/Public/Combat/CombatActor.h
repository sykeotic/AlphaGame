#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Runtime/Engine/Classes/Animation/AnimMontage.h"
#include "Runtime/Engine/Classes/Sound/SoundCue.h"
#include "Runtime/Engine/Classes/Components/BoxComponent.h"
#include "CombatActor.generated.h"

class UCombatComponent;
class APlayableCharacter;

UENUM(BlueprintType)
enum class EValidTargets : uint8 {
	ALLIES UMETA(DisplayName = "Allies"),
	ENEMIES UMETA(DisplayName = "Enemies"),
	ALL UMETA(DisplayName = "All"),
	SELF UMETA(DisplayName = "Self")
};

UENUM(BlueprintType)
enum class ERange : uint8 {
	MELEE UMETA(DisplayName = "Melee"),
	RANGED UMETA(DisplayName = "Ranged"),
	THROWN UMETA(DisplayName = "Thrown")
};

UENUM(BlueprintType)
enum class EActorType : uint8 {
	WEAPON UMETA(DisplayName = "Weapon"),
	SPELL UMETA(DisplayName = "Spell"),
	CONSUMABLE UMETA(DisplayName = "Consumable")
};

UENUM(BlueprintType)
enum class ECombatActorState : uint8 {
	USING UMETA(DisplayName = "Using"),
	IDLE UMETA(DisplayName = "Idle"),
	EQUIPPING UMETA(DisplayName = "Equipping"),
	RELOADING UMETA(DisplayName = "Reloading")
};

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

	void StopUse();

	void BoolSpam();

	float ResolveDamageModifiers(APlayableCharacter* OffensivePlayer, APlayableCharacter* DefensivePlayer, ACombatActor* OffensiveCombatActor);

	virtual void AssignWeaponValues(float InCooldown, UStaticMesh* InStaticMesh, FName InProjectileSpawnLocation, ERange IN_RANGE, EActorType IN_ACTOR_TYPE, float InDmg, float InRange);
};
