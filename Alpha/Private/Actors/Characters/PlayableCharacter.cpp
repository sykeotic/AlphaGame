#include "PlayableCharacter.h"
#include "StatsComponent.h"
#include "Logger.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "TimerManager.h"
#include "Camera/CameraComponent.h"
#include "Engine/DataTable.h"
#include "Combat/Components/CombatComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "CombatUtils.h"
#include "Combat/Actors/BaseCombatActor.h"
#include "GameplayUtils.h"
#include "Components/InputComponent.h"
#include "Runtime/Engine/Classes/Components/DecalComponent.h"
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

	bIsAttacking = false;
}

void APlayableCharacter::BeginPlay() {
	Super::BeginPlay();
	UBasePawnData* BasePawnData = LoadObject<UBasePawnData>(NULL, TEXT("BasePawnData'/Game/Data/DataAssets/TestBoi.TestBoi'"), NULL, LOAD_None, NULL);
	InitCharacterData(BasePawnData);
	InitCombatComponent();
	StatsComponent->SetCurrentHealth(StatsComponent->GetMaxHealth());
}

void APlayableCharacter::InitCharacterData(UBasePawnData* BaseData) {
	if (BaseData) {
		CharacterData = BaseData->CharacterData;
		CameraData = BaseData->CharacterData.PlayerCameraData;
		PawnStatsData = BaseData->CharacterData.PawnStatsData;
		GraphicsData = BaseData->CharacterData.PawnGraphicsData;
		SetCharacterValues();
	}
}

void APlayableCharacter::InitCombatComponent() {
	for (int i = 0; i < CharacterData.Weapons.Num(); i++) {
		FActorSpawnParameters SpawnInfo;
		ABaseCombatActor* Weapon;
		Weapon = Cast<ABaseCombatActor>(GetWorld()->SpawnActor<ABaseCombatActor>(CharacterData.Weapons[i]->BaseCombatActorDataStruct.ActorClass, FVector::ZeroVector, FRotator::ZeroRotator, SpawnInfo));
		Weapon->SetCombatComponentOwner(CombatComponent);
		Weapon->AssignValues(CharacterData.Weapons[i]);
		CombatComponent->AddWeaponToArray(Weapon);
		Weapon->GetMesh()->IgnoreActorWhenMoving(this, true);
		if (i == 0) {
			CombatComponent->SetCurrentWeaponIndex(0);
			CombatComponent->SetCurrentWeapon(Weapon, true);
		}
		CombatComponent->WeaponCount++;
	}

	for (int i = 0; i < CharacterData.Abilities.Num(); i++) {

	}
}

float APlayableCharacter::GetCurrentHPPercent() {
	return StatsComponent->GetCurrentHealth() / StatsComponent->GetMaxHealth();
}

UDecalComponent* APlayableCharacter::GetDecal() {
	return CursorToWorld;
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

	PlayerInputComponent->BindAction("AbilityNext", IE_Pressed, this, &APlayableCharacter::AbilityNext);
	PlayerInputComponent->BindAction("AbilityPrevious", IE_Pressed, this, &APlayableCharacter::AbilityPrevious);

	PlayerInputComponent->BindAction("WeaponNext", IE_Pressed, this, &APlayableCharacter::WeaponNext);
	PlayerInputComponent->BindAction("WeaponPrevious", IE_Pressed, this, &APlayableCharacter::WeaponPrevious);

	PlayerInputComponent->BindAxis("MoveForward", this, &APlayableCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &APlayableCharacter::MoveRight);

	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &APlayableCharacter::TurnAtRate);

	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &APlayableCharacter::LookUpAtRate);
}

FVector APlayableCharacter::GetPawnViewLocation() const {
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

void APlayableCharacter::AbilityNext() {
	if (!bIsAttacking) {
		CombatComponent->CycleNextAbility();
	}
}

void APlayableCharacter::AbilityPrevious() {
	if (!bIsAttacking) {
		CombatComponent->CyclePreviousAbility();
	}
}

void APlayableCharacter::WeaponNext() {
	if (!bIsAttacking) {
		CombatComponent->CycleNextWeapon();
	}
}

void APlayableCharacter::WeaponPrevious() {
	if (!bIsAttacking) {
		CombatComponent->CyclePreviousWeapon();
	}
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
	if (CombatComponent->GetCurrentWeapon()) {
		const float GameTime = GetWorld()->GetTimeSeconds();
		if (!bIsAttacking && CombatComponent->GetCurrentWeapon()->GetNextValidFireTime() <= GetWorld()->GetTimeSeconds()) {
			bIsAttacking = true;
			CombatComponent->UseCurrentWeapon();
		}
	}
}

void APlayableCharacter::CharacterAttackStop() {
	if (CombatComponent->GetCurrentWeapon()) {
		const float GameTime = GetWorld()->GetTimeSeconds();
		float StopAttackTime = CombatComponent->GetCurrentWeapon()->GetNextValidFireTime() - GameTime;
		if (bIsAttacking)
		{
			if (StopAttackTime > 0)
				GetWorldTimerManager().SetTimer(AttackStopTimer, this, &APlayableCharacter::SetAttackingFalse, StopAttackTime, false);
			else {
				SetAttackingFalse();
			}
		}
	}
}

void APlayableCharacter::CharacterAbilityStart() {
	if (CombatComponent->GetCurrentAbility()) {
		const float GameTime = GetWorld()->GetTimeSeconds();
		if (!bIsAttacking && CombatComponent->GetCurrentAbility()->GetNextValidFireTime() <= GetWorld()->GetTimeSeconds()) {
			bIsAttacking = true;

			CombatComponent->UseCurrentAbility();
		}
	}
}

void APlayableCharacter::CharacterAbilityStop() {
	if (CombatComponent->GetCurrentAbility()) {
		const float GameTime = GetWorld()->GetTimeSeconds();
		float StopAttackTime = CombatComponent->GetCurrentAbility()->GetNextValidFireTime() - GameTime;
		if (bIsAttacking)
		{
			if (StopAttackTime > 0)
				GetWorldTimerManager().SetTimer(AttackStopTimer, this, &APlayableCharacter::SetAttackingFalse, StopAttackTime, false);
			else {
				SetAttackingFalse();
			}
		}
	}
}

void APlayableCharacter::SetAttackingFalse() {
	bIsAttacking = false;
	if (CombatComponent->GetCurrentAbility())
		CombatComponent->GetCurrentAbility()->StopUse();
	if (CombatComponent->GetCurrentWeapon())
		CombatComponent->GetCurrentWeapon()->StopUse();
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

UCombatComponent* APlayableCharacter::GetCombatComponent() {
	return CombatComponent;
}

UStatsComponent* APlayableCharacter::GetStatsComponent() {
	return StatsComponent;
}

UTeamComponent* APlayableCharacter::GetOwnerTeam() {
	return OwnerTeam;
}

void APlayableCharacter::SetOwnerTeam(UTeamComponent* InTeam) {
	OwnerTeam = InTeam;
}

USpringArmComponent* APlayableCharacter::GetCameraSpringArm() {
	return CameraBoom;
}

void APlayableCharacter::SetCharacterValues() {
	SetBaseTurnRate(CameraData.BaseTurnRate);
	SetBaseLookUpRate(CameraData.BaseLookupRate);
	bUseControllerRotationPitch = CameraData.bUseControllerPitch;
	bUseControllerRotationRoll = CameraData.bUseControllerRoll;
	bUseControllerRotationYaw = CameraData.bUseControllerYaw;
	GetCameraSpringArm()->bEnableCameraLag = true;
	GetCameraSpringArm()->bUsePawnControlRotation = CameraData.bUsePawnControlRotation;
	GetCameraSpringArm()->TargetArmLength = CameraData.BoomArmLength;
	GetCameraSpringArm()->SetRelativeTransform(CameraData.RelTransform);
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->JumpZVelocity = CharacterData.JumpVelocity;
	GetCharacterMovement()->RotationRate = CharacterData.RotationRate;
	GetCharacterMovement()->MaxWalkSpeed = CharacterData.MoveSpeed;
	GetCapsuleComponent()->InitCapsuleSize(GraphicsData.CapsuleRadius, GraphicsData.CapsuleHeight);
	GetMesh()->SetRelativeLocation(GraphicsData.MeshLocation);
	GetMesh()->SetRelativeRotation(GraphicsData.MeshRotation);
	GetMesh()->SetSkeletalMesh(GraphicsData.SkeletalMesh);
	GetMesh()->SetMaterial(0, GraphicsData.Material_0);
	GetMesh()->SetMaterial(1, GraphicsData.Material_1);
	GetMesh()->SetAnimInstanceClass(GraphicsData.AnimInstanceClass);
	GetDecal()->DecalSize = GraphicsData.DecalSize;
	GetDecal()->SetRelativeRotation(GraphicsData.DecalRotation.Quaternion());
	GetDecal()->SetVisibility(false);
	GetDecal()->SetDecalMaterial(GraphicsData.DecalMaterial);
}