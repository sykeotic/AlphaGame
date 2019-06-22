// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "StatsComponent.h"
#include "CombatComponent.h"
#include "CombatActor.h"
#include "Runtime/Engine/Classes/Components/DecalComponent.h"
#include "PlayableCharacter.generated.h"

UCLASS(Blueprintable)
class APlayableCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	APlayableCharacter();

	void JumpStarted(ETouchIndex::Type FingerIndex, FVector Location);
	void JumpStopped(ETouchIndex::Type FingerIndex, FVector Location);
	void MoveForward(float Value);
	void MoveRight(float Value);
	void TurnAtRate(float Rate);
	void LookUpAtRate(float Rate);
	void SetBaseTurnRate(float InRate);
	void SetBaseLookUpRate(float InRate);

	UFUNCTION(BlueprintCallable)
	void SetDecal(UMaterial* InMaterial, FVector InSize, FRotator RelRotation);
	void SwitchOnDecal();
	void SwitchOffDecal();

	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	class UStatsComponent* StatsComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	class UCombatComponent* CombatComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;

	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "OnCharacterDeath"))
	void ReceiveOnCharacterDeath();

	UFUNCTION(BlueprintCallable, Category = "Game")
	void AssignCameraValues(float InBaseTurnRate, float InBaseLookupRate, bool bUseYaw, bool bUsePitch, bool bUseRoll, float BoomArmLength, bool bInUseControlRotation, FTransform RelTransform);

	UFUNCTION(BlueprintCallable, Category = "Game")
	void AssignStatValues(float JumpVelocity, FRotator RotationRate, float MaxHealth, float MoveSpeed, float CapsuleRadius, float CapsuleHeight, FVector MeshRotation);

	UFUNCTION(BlueprintCallable)
	void AssignCharacterMesh(UMaterial* InMaterial_0, UMaterial* InMaterial_1, USkeletalMesh* InMesh);

	UFUNCTION(BlueprintCallable)
	void AssignCombatMesh(UMaterial* InWeaponMaterial, UStaticMesh* InStaticMesh, FName InSocket, ERange IN_RANGE, EActorType IN_ACTOR_TYPE, FName ProjSpawn, float InDmg, float InRange);

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UDecalComponent* CursorToWorld;

	void CharacterAttackStart();
	void CharacterAttackStop();
	bool IsCharacterAttacking();
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual FVector GetPawnViewLocation() const override;
	float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, class AActor* DamageCauser) override;

	float BaseLookUpRate;
	float BaseTurnRate;
	FName WeaponSocketLocation;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Camera, meta = (AllowPrivateAccess = "true"))
	bool bIsAttacking;
};
