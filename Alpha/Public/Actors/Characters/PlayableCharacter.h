// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "StatsComponent.h"
#include "CombatComponent.h"
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
	virtual void Tick(float DeltaTime) override;
	//protected
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

	void CharacterAttackStart();
	void CharacterAttackStop();
	bool IsCharacterAttacking();
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual FVector GetPawnViewLocation() const override;
	float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, class AActor* DamageCauser) override;

	float BaseLookUpRate;
	float BaseTurnRate;
	bool bIsAttacking;
};
