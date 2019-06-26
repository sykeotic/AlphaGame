#include "PlayableCharacter.h"
#include "CombatComponent.h"
#include "CombatActor.h"
#include "StatsComponent.h"
#include "Logger.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Camera/CameraComponent.h"
#include "RangedCombatWeapon.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "MeleeCombatWeapon.h"
#include "Components/InputComponent.h"
#include "Runtime/Engine/Classes/Components/DecalComponent.h"
#include "Engine.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"

APlayableCharacter::APlayableCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	StatsComponent = CreateDefaultSubobject<UStatsComponent>(TEXT("StatsComponent"));
	StatsComponent->SetOwner(Cast<APlayableCharacter>(this));
	StatsComponent->SetAlive(true);

	CombatComponent = CreateDefaultSubobject<UCombatComponent>(TEXT("CombatComponent"));
	CombatComponent->SetCharacterOwner(this);

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);	

	CursorToWorld = CreateDefaultSubobject<UDecalComponent>("CursorToWorld");
	CursorToWorld->SetupAttachment(RootComponent);

	bCanAttack = true;
	bIsAttacking = false;
}

void APlayableCharacter::SetDecal(UMaterial* InMaterial, FVector InSize, FRotator RelRotation) {
	CursorToWorld->DecalSize = FVector(16.0f, 32.0f, 32.0f);
	CursorToWorld->SetRelativeRotation(FRotator(90.0f, 0.0f, 0.0f).Quaternion());
	CursorToWorld->SetVisibility(false);
	CursorToWorld->SetDecalMaterial(InMaterial);
}

void APlayableCharacter::SwitchOnDecal() {
	CursorToWorld->SetVisibility(true);
}

void APlayableCharacter::SwitchOffDecal() {
	CursorToWorld->SetVisibility(false);
}

void APlayableCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bIsAttacking && bCanAttack) {
		CharacterAttackStart();
	}

}

void APlayableCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction("LeftMouseClick", IE_Pressed, this, &APlayableCharacter::CharacterAttackStart);
	PlayerInputComponent->BindAction("LeftMouseClick", IE_Released, this, &APlayableCharacter::CharacterAttackStop);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAxis("MoveForward", this, &APlayableCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &APlayableCharacter::MoveRight);

	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &APlayableCharacter::TurnAtRate);

	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &APlayableCharacter::LookUpAtRate);
}

FVector APlayableCharacter::GetPawnViewLocation() const{
		if (FollowCamera) {
			return FollowCamera->GetComponentLocation();
		}
		return Super::GetPawnViewLocation();
}

void APlayableCharacter::TurnAtRate(float Rate)
{
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void APlayableCharacter::LookUpAtRate(float Rate)
{
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void APlayableCharacter::MoveForward(float Value)
{
	if ((Controller != NULL) && (Value != 0.0f))
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void APlayableCharacter::MoveRight(float Value)
{
	if ((Controller != NULL) && (Value != 0.0f))
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		AddMovementInput(Direction, Value);
	}
}

void APlayableCharacter::CharacterAttackStart() {
	ULogger::ScreenMessage(FColor::Green, "Character Attacking");
	if (!bIsAttacking && bCanAttack) {
		ULogger::ScreenMessage(FColor::Green, "Character Can Attack");
		bIsAttacking = true;
		bCanAttack = false;
		FTimerHandle Handle;
		FTimerHandle DelayHandle;
		GetWorld()->GetTimerManager().SetTimer(Handle, [this]() {
			CombatComponent->UseCurrentWeapon();
		}, .25, false);
		GetWorld()->GetTimerManager().SetTimer(DelayHandle, [this]() {
			bCanAttack = true;
		}, CombatComponent->CurrentWeapon->UseCooldown, false);
	}
	else if (bIsAttacking && bCanAttack) {
		bCanAttack = false;
		FTimerHandle Handle;
		FTimerHandle DelayHandle;
		CombatComponent->UseCurrentWeapon();
		GetWorld()->GetTimerManager().SetTimer(DelayHandle, [this]() {
			bCanAttack = true;
		}, CombatComponent->CurrentWeapon->UseCooldown, false);
	}
}

void APlayableCharacter::CharacterAttackStop() {
	if (bIsAttacking == true) {
		bIsAttacking = false;
	}
}

bool APlayableCharacter::IsCharacterAttacking() {
	return bIsAttacking;
}

void APlayableCharacter::SetBaseTurnRate(float InRate) {
	BaseTurnRate = InRate;
}

void APlayableCharacter::SetBaseLookUpRate(float InRate) {
	BaseLookUpRate = InRate;
}

float APlayableCharacter::TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, class AActor* DamageCauser) {
	if (StatsComponent->IsAlive()) {
		StatsComponent->TakeDamage(Damage);
		if (StatsComponent->GetCurrentHealth() <= 0.f) {
			ReceiveOnCharacterDeath();
		}
	}
	return Damage;
}

void APlayableCharacter::AssignCameraValues(float InBaseTurnRate, float InBaseLookupRate, bool bUseYaw, bool bUsePitch, bool bUseRoll, float BoomArmLength, bool bInUseControlRotation, FTransform RelTransform) {
	ULogger::ScreenMessage(FColor::Red, "Assigning Camera Stats");
	SetBaseTurnRate(InBaseTurnRate);
	SetBaseLookUpRate(InBaseLookupRate);
	bUseControllerRotationPitch = bUsePitch;
	bUseControllerRotationRoll = bUseRoll;
	bUseControllerRotationYaw = bUseYaw;
	CameraBoom->bEnableCameraLag = true;
	CameraBoom->bUsePawnControlRotation = bInUseControlRotation;
	CameraBoom->TargetArmLength = BoomArmLength;
	CameraBoom->SetRelativeTransform(RelTransform);
}

void APlayableCharacter::AssignStatValues(float JumpVelocity, FRotator RotationRate, float MaxHealth, float MoveSpeed, float CapsuleRadius, float CapsuleHeight, FVector MeshRotation) {
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->JumpZVelocity = JumpVelocity;
	GetCharacterMovement()->RotationRate = RotationRate;
	GetCharacterMovement()->MaxWalkSpeed = MoveSpeed;
	GetCapsuleComponent()->InitCapsuleSize(CapsuleRadius, CapsuleHeight);
	GetMesh()->SetRelativeLocation(MeshRotation);
}

void APlayableCharacter::AssignCombatMesh(float InCooldown, FVector InLocation, FRotator InRotation, UMaterial* InWeaponMaterial, UStaticMesh* InStaticMesh, FName InSocket, ERange IN_RANGE, EActorType IN_ACTOR_TYPE, FName ProjSpawn, float InDmg, float InRange) {
	CombatComponent->SpawnWeapon(InCooldown, InLocation, InRotation, InWeaponMaterial, InStaticMesh, InSocket, IN_RANGE, IN_ACTOR_TYPE, ProjSpawn, InDmg, InRange);
}

void APlayableCharacter::AssignCharacterMesh(UMaterial* InMaterial_0, UMaterial* InMaterial_1, USkeletalMesh* InMesh) {
	GetMesh()->SetSkeletalMesh(InMesh);
	GetMesh()->SetMaterial(0, InMaterial_0);
	GetMesh()->SetMaterial(1, InMaterial_1);
}