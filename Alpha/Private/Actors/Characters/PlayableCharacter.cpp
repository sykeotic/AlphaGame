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
#include "Engine.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"

APlayableCharacter::APlayableCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	StatsComponent = CreateDefaultSubobject<UStatsComponent>(TEXT("StatsComponent"));
	StatsComponent->SetOwner(this);

	CombatComponent = CreateDefaultSubobject<UCombatComponent>(TEXT("CombatComponent"));
	CombatComponent->SetOwner(this);

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);	
}

void APlayableCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void APlayableCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAction("LeftMouseClick", IE_Pressed, this, &APlayableCharacter::CharacterAttackStart);
	PlayerInputComponent->BindAction("LeftMouseClick", IE_Released, this, &APlayableCharacter::CharacterAttackStop);

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

float APlayableCharacter::TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, class AActor* DamageCauser) {
	StatsComponent->TakeDamage(Damage);
	return Damage;
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
	bIsAttacking = true;
	ULogger::ScreenMessage(FColor::Green, "Character Attacking");
	CombatComponent->UseCurrentWeapon();
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

void APlayableCharacter::SpawnWeapon(UMaterial* InWeaponMaterial, UStaticMesh* InStaticMesh, FString SocketLocation, ERange IN_RANGE, EActorType IN_ACTOR_TYPE) {
	FVector Location(0.0f, 0.0f, 0.0f);
	FRotator Rotation(0.0f, 0.0f, 0.0f);
	FActorSpawnParameters SpawnInfo;
	ACombatActor* Weapon;
	if (IN_RANGE == ERange::MELEE && IN_ACTOR_TYPE == EActorType::WEAPON) {
		Weapon = Cast<ACombatActor>(GetWorld()->SpawnActor<AMeleeCombatWeapon>(Location, Rotation, SpawnInfo));
	} else if (IN_RANGE == ERange::RANGED && IN_ACTOR_TYPE == EActorType::WEAPON)	{
		Weapon = Cast<ACombatActor>(GetWorld()->SpawnActor<ARangedCombatWeapon>(Location, Rotation, SpawnInfo));
	} else {
		Weapon = nullptr;
	}
	CombatComponent->AddWeapon(Weapon, InWeaponMaterial, InStaticMesh, SocketLocation);
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

void APlayableCharacter::AssignStatValues(float JumpVelocity, FRotator RotationRate, float MaxHealth, float MoveSpeed) {
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->JumpZVelocity = JumpVelocity;
	GetCharacterMovement()->RotationRate = RotationRate;
	GetCharacterMovement()->MaxWalkSpeed = MoveSpeed;
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
	GetMesh()->SetRelativeLocation(FVector(0.f, 0.f, -90.f));
}

void APlayableCharacter::AssignCombatMesh(UMaterial* InWeaponMaterial, UStaticMesh* InStaticMesh, FString InSocket, ERange IN_RANGE, EActorType IN_ACTOR_TYPE) {
	SpawnWeapon(InWeaponMaterial, InStaticMesh, InSocket, IN_RANGE, IN_ACTOR_TYPE);
}

void APlayableCharacter::AssignCombatValues(float Damage, int32 MaxAmmo, float Range, FString ProjectileSpawn, EActorType WeaponType, ERange WeaponRange) {

}

void APlayableCharacter::AssignCharacterMesh(UMaterial* InMaterial_0, UMaterial* InMaterial_1, USkeletalMesh* InMesh) {
	GetMesh()->SetSkeletalMesh(InMesh);
	GetMesh()->SetMaterial(0, InMaterial_0);
	GetMesh()->SetMaterial(1, InMaterial_1);
}