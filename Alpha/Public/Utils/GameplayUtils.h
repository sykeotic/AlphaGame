// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Runtime/Engine/Classes/Engine/DataTable.h"
#include "Runtime/Engine/Classes/Animation/AnimMontage.h"
#include "Runtime/Engine/Classes/Sound/SoundCue.h"
#include "GameplayUtils.generated.h"

class ACombatActor;

UENUM(BlueprintType)
enum class EArmorType : uint8 {
	NO_ARMOR UMETA(DisplayName = "No Armor"),
	LIGHT_ARMOR UMETA(DisplayName = "Light Armor"),
	MEDIUM_ARMOR UMETA(DisplayName = "Medium Armor"),
	HEAVY_ARMOR UMETA(DisplayName = "Heavy Armor"),
	INVULNERABLE UMETA(DisplayName = "Invulnerable")
};

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

USTRUCT(BlueprintType)
struct FPlayerCameraData : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float BaseTurnRate;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float BaseLookupRate;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bUseControllerYaw;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bUseControllerPitch;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bUseControllerRoll;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float BoomArmLength;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bUsePawnControlRotation;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FTransform RelTransform;
};

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

USTRUCT(BlueprintType)
struct FPawnGraphicsData : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector MeshRotation;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UMaterial* Material_0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UMaterial* Material_1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USkeletalMesh* SkeletalMesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName WeaponSocketLocation;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float CapsuleRadius;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float CapsuleHeight;

	//Decal
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UMaterial* DecalMaterial;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector DecalSize;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FRotator DecalRotation;
};

USTRUCT(BlueprintType)
struct FPawnStatsData : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxHealth;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EArmorType ArmorType;
};

USTRUCT(BlueprintType)
struct FCharacterData : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float JumpVelocity;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MoveSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FRotator RotationRate;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FPlayerCameraData CameraData;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FPawnGraphicsData GraphicsData;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FPawnStatsData StatsData;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FCombatActorData> WeaponData;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FCombatActorData> AbilityData;
};

USTRUCT(BlueprintType)
struct FRaceData : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName RaceName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName RacialPassive;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FName> PlayableHeroesData;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FName> PlayablePawnsData;
};

USTRUCT(BlueprintType)
struct FGameModeData : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName GameModeName;
};

/**
 *
 */
class APlayableCharacter;
class UCombatComponent;
class UStatsComponent;
class ACombatActor;

UCLASS()
class ALPHA_API UGameplayUtils : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	static void ConstructPlayableCharacter(APlayableCharacter* InChar, FCharacterData* InRow);
	static void ConstructCombatComponent(UCombatComponent* InComp, FCharacterData* InRow);
	static void ConstructStatsComponent(UStatsComponent* InStats, FPawnStatsData* InRow);
	static void ConstructCombatActor(ACombatActor* InActor, FCombatActorData* InRow);

};
