// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Runtime/Engine/Classes/Components/DecalComponent.h"
#include "Engine/DataTable.h"
#include "PlayableCharacter.generated.h"


UENUM(BlueprintType)
enum class EArmorType : uint8 {
	NO_ARMOR UMETA(DisplayName = "No Armor"),
	LIGHT_ARMOR UMETA(DisplayName = "Light Armor"),
	MEDIUM_ARMOR UMETA(DisplayName = "Medium Armor"),
	HEAVY_ARMOR UMETA(DisplayName = "Heavy Armor"),
	INVULNERABLE UMETA(DisplayName = "Invulnerable")
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
struct FPawnGraphicsData : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FVector MeshLocation;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FRotator MeshRotation;
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
		FName GraphicsDataKey;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FName PawnStatsDataKey;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FName PlayerCameraDataKey;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<FName> WeaponDataKeys;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<FName> AbilityDataKeys;
	/*
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
	*/
};


class UCombatUtils;
class ACombatActor;
class UCombatComponent;
class UStatsComponent;
class UTeamComponent;
class USpringArmComponent;

UCLASS(Blueprintable)
class APlayableCharacter : public ACharacter
{
	GENERATED_BODY()

	APlayableCharacter();

public:
	FCharacterData* CharacterData;
	FPlayerCameraData* CameraData;
	FPawnStatsData* PawnStatsData;
	FPawnGraphicsData* GraphicsData;

	FCharacterData* GetCharacterDataStruct();
	void SetCharacterValues();

	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void BeginPlay() override;

	void MoveForward(float Value);
	void MoveRight(float Value);
	void TurnAtRate(float Rate);
	void LookUpAtRate(float Rate);
	void SetBaseTurnRate(float InRate);
	void SetBaseLookUpRate(float InRate);

	void AbilityNext();
	void AbilityPrevious();
	void WeaponNext();
	void WeaponPrevious();

	void SwitchOnDecal();
	void SwitchOffDecal();

	void CharacterAttackStart();
	void CharacterAttackStop();
	void CharacterAbilityStart();
	void CharacterAbilityStop();

	void SetAttackingFalse();

	UCombatComponent* GetCombatComponent();
	UTeamComponent* GetOwnerTeam();
	UStatsComponent* GetStatsComponent();
	UDecalComponent* GetDecal();
	USpringArmComponent* GetCameraSpringArm();

	void SetOwnerTeam(UTeamComponent* InTeam);

	bool CharacterCanAttack();

	float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, class AActor* DamageCauser) override;

	UFUNCTION(BlueprintCallable)
	float GetCurrentHPPercent();

	UFUNCTION(BlueprintCallable)
	virtual FVector GetPawnViewLocation() const override;

protected:	
	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "OnCharacterDeath"))
		void ReceiveOnCharacterDeath();

	void InitCombatComponent();

	void InitCharacterData(FName CharacterString);

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Camera, meta = (AllowPrivateAccess = "true"))
	bool bIsAttacking;

	UPROPERTY(EditDefaultsOnly, Category = "Stats")
	FName WeaponSocketLocation;

	UPROPERTY(EditDefaultsOnly, Category = "Stats")
	FName AbilitySocketLocation;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UDecalComponent* CursorToWorld;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;

	UPROPERTY(EditDefaultsOnly, Category = "Stats")
	class UStatsComponent* StatsComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	class UCombatComponent* CombatComponent;

private:
	FTimerHandle AttackStopTimer;

	float BaseLookUpRate;
	float BaseTurnRate;

	class UTeamComponent* OwnerTeam;
};
