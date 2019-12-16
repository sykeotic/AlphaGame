// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Engine/DataTable.h"
#include "BasePawnData.generated.h"

class UBaseCombatActorData;

UENUM(BlueprintType)
enum class EArmorType : uint8 {
	NO_ARMOR UMETA(DisplayName = "No Armor"),
	LIGHT_ARMOR UMETA(DisplayName = "Light Armor"),
	MEDIUM_ARMOR UMETA(DisplayName = "Medium Armor"),
	HEAVY_ARMOR UMETA(DisplayName = "Heavy Armor"),
	INVULNERABLE UMETA(DisplayName = "Invulnerable")
};

USTRUCT(BlueprintType)
struct FPawnStatsData : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float MaxHealth;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		EArmorType ArmorType;
};

USTRUCT(BlueprintType)
struct FPawnGraphicsData : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FVector MeshLocation;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FRotator MeshRotation;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FVector MeshScale;
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
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<UAnimInstance> AnimInstanceClass;

	//Decal
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UMaterial* DecalMaterial;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FVector DecalSize;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FRotator DecalRotation;
};

USTRUCT(BlueprintType)
struct FPlayerCameraData : public FTableRowBase
{
	GENERATED_BODY()

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
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float CameraLagSpeed;
};

USTRUCT(BlueprintType)
struct FCharacterData : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float JumpVelocity;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float MoveSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FRotator RotationRate;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString CollisionProfile;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FPawnGraphicsData PawnGraphicsData;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FPawnStatsData PawnStatsData;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FPlayerCameraData PlayerCameraData;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<UBaseCombatActorData*> Weapons;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<UBaseCombatActorData*> Abilities;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UAnimMontage* DeathAnimation;
};

/**
 * 
 */
UCLASS(BlueprintType, DefaultToInstanced, EditInlineNew)
class ALPHA_API UBasePawnData : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FCharacterData CharacterData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FName CharacterName;
	
};
