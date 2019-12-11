// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Runtime/Engine/Classes/Components/DecalComponent.h"
#include "Engine/DataTable.h"
#include "BasePawnData.h"
#include "PlayableCharacter.generated.h"

class UCombatUtils;
class ACombatActor;
class UCombatComponent;
class UStatsComponent;
class UTeamComponent;
class USpringArmComponent;
class USoundCue;
class ABaseCombatActor;
class AModifier;

UCLASS(Blueprintable)
class APlayableCharacter : public ACharacter
{
	GENERATED_BODY()

	APlayableCharacter();

public:
	FCharacterData CharacterData;
	FPlayerCameraData CameraData;
	FPawnStatsData PawnStatsData;
	FPawnGraphicsData GraphicsData;

	void SetCharacterValues();
	void InitCombatComponent();

	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void BeginPlay() override;

	void MoveForward(float Value);
	void MoveRight(float Value);
	void TurnAtRate(float Rate);
	void LookUpAtRate(float Rate);
	void SetBaseTurnRate(float InRate);
	void SetBaseLookUpRate(float InRate);

	UFUNCTION(BlueprintCallable)
		void AbilityNext();
	UFUNCTION(BlueprintCallable)
		void AbilityPrevious();

	UFUNCTION(BlueprintCallable)
		void WeaponNext();
	UFUNCTION(BlueprintCallable)
		void WeaponPrevious();

	UFUNCTION(BlueprintCallable)
		void CharacterAttackStart();
	UFUNCTION(BlueprintCallable)
		void CharacterAttackStop();

	UFUNCTION(BlueprintCallable)
		void CharacterAbilityStart();
	UFUNCTION(BlueprintCallable)
		void CharacterAbilityStop();


	void SwitchOnDecal();
	void SwitchOffDecal();

	void SetSpawnFX(UParticleSystemComponent* InComp);
	UParticleSystemComponent* GetSpawnFX();
	void PlaySpawnFX(UParticleSystem* InVFX, USoundCue* InSound);
	void DestroySpawnFX();
	
	bool IsAIPlayer();
	void SetIsAIPlayer(bool InBool);

	void SetAttackingFalse();

	UCombatComponent* GetCombatComponent();
	UTeamComponent* GetOwnerTeam();
	UStatsComponent* GetStatsComponent();
	UDecalComponent* GetDecal();
	USpringArmComponent* GetCameraSpringArm();

	void SetOwnerTeam(UTeamComponent* InTeam);

	void ApplyModifiers(AModifier* InModifier, AActor* InActor);

	void InitCharacterData(UBasePawnData* BaseData);

	bool CharacterCanAttack();

	UFUNCTION(BlueprintCallable)
	float GetCurrentHPPercent();

	UFUNCTION(BlueprintCallable)
	virtual FVector GetPawnViewLocation() const override;
	
	void HandleDeath();

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
	void DestroyActor();

	FTimerHandle AttackStopTimer;

	float BaseLookUpRate;
	float BaseTurnRate;

	class UTeamComponent* OwnerTeam;
	class UHandlerComponent* HandlerComponent;

	bool bIsAIPlayer;

	UParticleSystemComponent* SpawnEffect;
};
