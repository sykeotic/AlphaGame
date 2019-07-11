#include "PlayableCharacter.h"
#include "CombatComponent.h"
#include "CombatActor.h"
#include "StatsComponent.h"
#include "Logger.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Camera/CameraComponent.h"
#include "RangedCombatWeapon.h"
#include "CombatAbility.h"
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

void APlayableCharacter::BeginPlay() {
	Super::BeginPlay();
	InitCombatComponent();
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

}

void APlayableCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction("LeftMouseClick", IE_Pressed, this, &APlayableCharacter::CharacterAttackStart);
	PlayerInputComponent->BindAction("LeftMouseClick", IE_Released, this, &APlayableCharacter::CharacterAttackStop);

	PlayerInputComponent->BindAction("RightMouseClick", IE_Pressed, this, &APlayableCharacter::CharacterAbilityStart);
	PlayerInputComponent->BindAction("RightMouseClick", IE_Released, this, &APlayableCharacter::CharacterAbilityStop);

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
	if (!bIsAttacking && GetWorld()->GetTimeSeconds() - CombatComponent->CurrentWeapon->LastFireTime > CombatComponent->CurrentWeapon->TimeBetweenShots) {
		bIsAttacking = true;
		if (CombatComponent->CurrentWeapon) {
			CombatComponent->UseCurrentWeapon();
		}
	}
}

void APlayableCharacter::CharacterAttackStop() {
	if (bIsAttacking)
	{
		bIsAttacking = false;
		if (CombatComponent->CurrentWeapon)
		{
			CombatComponent->CurrentWeapon->StopUse();
		}
	}
}

void APlayableCharacter::CharacterAbilityStart() {
	if (!bIsAttacking) {
		bIsAttacking = true;
		if (CombatComponent->CurrentAbility) {
			CombatComponent->UseCurrentAbility();
		}
	}
}

void APlayableCharacter::CharacterAbilityStop() {
	if (bIsAttacking)
	{
		bIsAttacking = false;
		if (CombatComponent->CurrentAbility)
		{
			CombatComponent->CurrentAbility->StopUse();
		}
	}
}

bool APlayableCharacter::IsCharacterAttacking() {
	return bIsAttacking;
}

bool APlayableCharacter::CharacterCanAttack() {
	return StatsComponent->IsAlive();
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

void APlayableCharacter::AssignStatValues(float JumpVelocity, FRotator RotationRate, float MoveSpeed, float CapsuleRadius, float CapsuleHeight, FVector MeshRotation) {
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->JumpZVelocity = JumpVelocity;
	GetCharacterMovement()->RotationRate = RotationRate;
	GetCharacterMovement()->MaxWalkSpeed = MoveSpeed;
	GetCapsuleComponent()->InitCapsuleSize(CapsuleRadius, CapsuleHeight);
	GetMesh()->SetRelativeLocation(MeshRotation);
}



void APlayableCharacter::AssignCharacterMesh(UMaterial* InMaterial_0, UMaterial* InMaterial_1, USkeletalMesh* InMesh, FName WeaponSocketLocation_In) {
	GetMesh()->SetSkeletalMesh(InMesh);
	GetMesh()->SetMaterial(0, InMaterial_0);
	GetMesh()->SetMaterial(1, InMaterial_1);
}

void APlayableCharacter::InitCombatComponent() {
	for (int i = 0; i < CombatComponent->WeaponClassArray.Num(); i++) {
		FActorSpawnParameters SpawnInfo;
		ACombatWeapon* Weapon;
		Weapon = Cast<ACombatWeapon>(GetWorld()->SpawnActor<ACombatWeapon>(CombatComponent->WeaponClassArray[i], FVector::ZeroVector, FRotator::ZeroRotator, SpawnInfo));
		Weapon->SetCombatComponentOwner(CombatComponent);
		Weapon->ActorSocketLocation = WeaponSocketLocation;
		CombatComponent->WeaponArray.AddUnique(Weapon);
		Weapon->MeshComp->IgnoreActorWhenMoving(this, true);
		if (i == 0) {
			CombatComponent->CurrentWeaponIndex = 0;
			CombatComponent->SetCurrentWeapon(Weapon, true);
		}
		CombatComponent->WeaponCount++;
	}

	for (int i = 0; i < CombatComponent->AbilityClassArray.Num(); i++) {
		FActorSpawnParameters SpawnInfo;
		ACombatAbility* Ability;
		Ability = Cast<ACombatAbility>(GetWorld()->SpawnActor<ACombatAbility>(CombatComponent->AbilityClassArray[i], FVector::ZeroVector, FRotator::ZeroRotator, SpawnInfo));
		Ability->SetCombatComponentOwner(CombatComponent);
		Ability->ActorSocketLocation = AbilitySocketLocation;
		CombatComponent->AbilityArray.AddUnique(Ability);
		Ability->MeshComp->IgnoreActorWhenMoving(this, true);
		if (i == 0) {
			CombatComponent->CurrentWeaponIndex = 0;
			CombatComponent->SetCurrentAbility(Ability, true);
		}
		CombatComponent->WeaponCount++;
	}
}

void APlayableCharacter::UnPossessed() {
	Super::UnPossessed();
	Multicast_HideMesh();
}

void APlayableCharacter::Multicast_HideMesh_Implementation() {
	GetMesh()->SetVisibility(false);
}