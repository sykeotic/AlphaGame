#include "CombatActor.h"
#include "TimerManager.h"
#include "Runtime/Engine/Classes/Engine/StaticMesh.h"
#include "CombatComponent.h"
#include "GameplayUtils.h"
#include "CombatUtils.h"
#include "PlayableCharacter.h"
#include "Runtime/Engine/Classes/Particles/ParticleSystemComponent.h"
#include "Runtime/Engine/Classes/Sound/SoundCue.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
#include "ConditionTree.h"
#include "Runtime/Engine/Classes/Animation/AnimMontage.h"
#include "Logger.h"

ACombatActor::ACombatActor()
{
	PrimaryActorTick.bCanEverTick = false;
	
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	RootComponent = MeshComp;

	ACTOR_STATE = ECombatActorState::IDLE;
	bIsEquipped = false;
	EquipDuration = 1.0f;
	NextValidFireTime = 0.0f;
}

void ACombatActor::BeginPlay() {
	Super::BeginPlay();
	ULogger::ScreenMessage(FColor::Green, "Conditions are: " + Conditions->IsConditionTreeTrue());
}

void ACombatActor::OnEquip(bool bPlayAnimation) {
	bPendingEquip = true;
	AssertActorState();
	if (bPlayAnimation)
	{
		float Duration = PlayActorAnimation(EquipAnim);
		if (Duration <= 0.0f)
		{
			Duration = 0.5f;
		}
		EquipStartedTime = GetWorld()->TimeSeconds;
		EquipDuration = Duration;
		GetWorldTimerManager().SetTimer(EquipFinishedTimerHandle, this, &ACombatActor::OnEquipFinished, Duration, false);
	}
	else
	{
		OnEquipFinished();
	}
}

void ACombatActor::AttachMeshToOwner(FName AttachPoint)
{
	if (ComponentOwner->CharacterOwner)
	{
		DetachMeshFromOwner();

		USkeletalMeshComponent* OwnerMesh = ComponentOwner->CharacterOwner->GetMesh();
		MeshComp->SetHiddenInGame(false);
		MeshComp->AttachToComponent(OwnerMesh, FAttachmentTransformRules::SnapToTargetNotIncludingScale, AttachPoint);
		MeshComp->AddLocalRotation(WeaponRotation);
		MeshComp->AddLocalOffset(WeaponLocation);
	}
}

void ACombatActor::OnUnEquip()
{
	bIsEquipped = false;
	StopUse();
	DetachMeshFromOwner();
	if (bPendingEquip)
	{
		StopActorAnimation(EquipAnim);
		bPendingEquip = false;

		GetWorldTimerManager().ClearTimer(EquipFinishedTimerHandle);
	}
	AssertActorState();
}


void ACombatActor::DetachMeshFromOwner()
{
	MeshComp->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
	MeshComp->SetHiddenInGame(true);
}

void ACombatActor::OnEquipFinished()
{
	AttachMeshToOwner(ActorSocketLocation);

	bIsEquipped = true;
	bPendingEquip = false;

	AssertActorState();
}

void ACombatActor::OnUse() {
	if (!bWantsToUse) {
		bWantsToUse = true;
		AssertActorState();
	}
}

bool ACombatActor::CanUse() {
	bool bOwnerCanFire = ComponentOwner->CharacterOwner && ComponentOwner->CharacterOwner->CharacterCanAttack();
	bool bStateOK = ACTOR_STATE == ECombatActorState::IDLE || ACTOR_STATE == ECombatActorState::USING;
	return bOwnerCanFire && bStateOK;
}

void ACombatActor::SetCombatComponentOwner(UCombatComponent* InComp) {
	ComponentOwner = InComp;
}

UCombatComponent* ACombatActor::GetCombatComponentOwner() {
	return ComponentOwner;
}

void ACombatActor::AssignWeaponValues(float InCooldown, UStaticMesh* InStaticMesh, FName InProjectileSpawnLocation, ERange IN_RANGE, EActorType IN_ACTOR_TYPE, float InDmg, float InRange) {
	MeshComp->SetStaticMesh(MeshComp->GetStaticMesh());
	MeshComp->SetMaterial(0, WeaponMaterial);
	MeshComp->SetStaticMesh(InStaticMesh);
	MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Damage = InDmg;
	UseRange = InRange;
	UseCooldown = InCooldown;
	RANGE_TYPE = IN_RANGE;
	ACTOR_TYPE = IN_ACTOR_TYPE;
}

void ACombatActor::AssertActorState()
{
	ECombatActorState NewState = ECombatActorState::IDLE;

	if (bIsEquipped)
	{
		if (bWantsToUse && CanUse() && !bPendingEquip)
		{
			NewState = ECombatActorState::USING;
		}
	}
	else if (bPendingEquip)
	{
		NewState = ECombatActorState::EQUIPPING;
	}
	SetCombatActorState(NewState);
}

void ACombatActor::SetCombatActorState(ECombatActorState InState)
{
	const ECombatActorState PrevState = ACTOR_STATE;

	if (PrevState == ECombatActorState::USING && InState != ECombatActorState::USING)
	{
		OnBurstFinished();
	}

	ACTOR_STATE = InState;

	if (PrevState != ECombatActorState::USING && InState == ECombatActorState::USING)
	{
		OnBurstStarted();
	}
}

void ACombatActor::OnBurstStarted()
{
	const float GameTime = GetWorld()->GetTimeSeconds();
	NextValidFireTime = GameTime + UseCooldown;
	if (LastFireTime > 0 && UseCooldown > 0.0f &&
		LastFireTime + UseCooldown > GameTime)
	{
		GetWorldTimerManager().SetTimer(TimerHandle_HandleFiring, this, &ACombatActor::HandleUse, LastFireTime + UseCooldown - GameTime, false);
	}
	else
	{
		NextValidFireTime = GameTime + UseCooldown;
		HandleUse();
	}
}


void ACombatActor::OnBurstFinished()
{
	BurstCounter = 0;
	StopSimulatingActorUse();

	GetWorldTimerManager().ClearTimer(TimerHandle_HandleFiring);
	bRefiring = false;
}

void ACombatActor::OnRep_BurstCounter()
{
	if (BurstCounter > 0)
	{
		StartSimulatingActorUse();
	}
	else
	{
		StopSimulatingActorUse();
	}
}

void ACombatActor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME_CONDITION(ACombatActor, BurstCounter, COND_SkipOwner);
}

void ACombatActor::ExecuteUse() {

}

void ACombatActor::HandleUse() {
	if (CanUse())
	{
		if (GetNetMode() != NM_DedicatedServer)
		{
			StartSimulatingActorUse();
		}

		if (ComponentOwner->CharacterOwner && ComponentOwner->CharacterOwner->IsLocallyControlled())
		{
			ExecuteUse();
			BurstCounter++;
		}
	}
	else if (ComponentOwner->CharacterOwner && ComponentOwner->CharacterOwner->IsLocallyControlled())
	{
		if (BurstCounter > 0)
		{
			OnBurstFinished();
		}
	}

	if (ComponentOwner->CharacterOwner && ComponentOwner->CharacterOwner->IsLocallyControlled())
	{

		bRefiring = (ACTOR_STATE == ECombatActorState::USING && UseCooldown > 0.0f);
		if (bRefiring)
		{
			GetWorldTimerManager().SetTimer(TimerHandle_HandleFiring, this, &ACombatActor::HandleUse, UseCooldown, false);
		}
	}

	LastFireTime = GetWorld()->GetTimeSeconds();
}

UAudioComponent* ACombatActor::PlayActorSound(USoundCue* SoundToPlay)
{
	UAudioComponent* AC = nullptr;
	if (!bPlayingSound || bPlaySoundEveryTime) {
		if (SoundToPlay && ComponentOwner->CharacterOwner)
		{
			bPlayingSound = true;
			AC = UGameplayStatics::SpawnSoundAttached(SoundToPlay, ComponentOwner->CharacterOwner->GetRootComponent());
			GetWorldTimerManager().SetTimer(SoundTimer, this, &ACombatActor::SetSoundPlayingFalse, SoundToPlay->GetDuration(), false);
		}
	}
	return AC;
}

void ACombatActor::SetSoundPlayingFalse() {
	bPlayingSound = false;
}

void ACombatActor::StartSimulatingActorUse(){
	GetWorldTimerManager().SetTimer(SFXTimer, this, &ACombatActor::StartSFX, SFXBuffer, false);
	if (!bPlayingFireAnim)
	{
		int8 AnimIndex = FMath::RandRange(0, FireAnim.Num() - 1);
		CurrentAnim = FireAnim[AnimIndex];
		PlayActorAnimation(CurrentAnim);
		bPlayingFireAnim = true;
	}
	int8 SoundIndex = FMath::RandRange(0, UseSound.Num() - 1);
	PlayActorSound(UseSound[SoundIndex]);
}

void ACombatActor::StartSFX() {
	if (ActorFX)
	{
		UsePSC = UGameplayStatics::SpawnEmitterAttached(ActorFX, MeshComp, ProjectileSpawnLocation);
	}
}

void ACombatActor::StopUse() {
	if (bWantsToUse)
	{
		bWantsToUse = false;
		AssertActorState();
	}
}

void ACombatActor::StopSimulatingActorUse()
{
	if (bPlayingFireAnim)
	{
		StopActorAnimation(CurrentAnim);
		bPlayingFireAnim = false;
	}
}

float ACombatActor::PlayActorAnimation(UAnimMontage* Animation, float InPlayRate, FName StartSectionName)
{
	float Duration = 0.0f;
	if (ComponentOwner->CharacterOwner)
	{
		if (Animation)
		{
			Duration = ComponentOwner->CharacterOwner->PlayAnimMontage(Animation, InPlayRate, StartSectionName);
		}
	}
	GetWorldTimerManager().SetTimer(AnimationTimer, this, &ACombatActor::StopSimulatingActorUse, Duration, false);
	return Duration;
}


void ACombatActor::StopActorAnimation(UAnimMontage* Animation)
{
	if (ComponentOwner->CharacterOwner)
	{
		if (Animation)
		{
			ComponentOwner->CharacterOwner->StopAnimMontage(Animation);
		}
	}
}

float ACombatActor::ResolveDamageModifiers(APlayableCharacter* OffensiveCharacter, APlayableCharacter* DefensiveCharacter, ACombatActor* OffensiveCombatActor) {
	float DamageReturn = UCombatUtils::CalculateDamage(Damage, DefensiveCharacter, OffensiveCharacter, this);
	ULogger::ScreenMessage(FColor::Red, "Did This Much Damage: " + FString::SanitizeFloat(DamageReturn));
	return DamageReturn;
}

void ACombatActor::BoolSpam() {
	ULogger::ScreenMessage(FColor::Red, "bPlayingFireAnim: ");
	ULogger::ScreenMessage(FColor::Red, bPlayingFireAnim ? "True" : "False");
	ULogger::ScreenMessage(FColor::Green, "bWantsToUse");
	ULogger::ScreenMessage(FColor::Green, bWantsToUse ? "True" : "False");
	ULogger::ScreenMessage(FColor::Blue, "bRefiring");
	ULogger::ScreenMessage(FColor::Blue, bRefiring ? "True" : "False");
	ULogger::ScreenMessage(FColor::Yellow, "bIsEquipped");
	ULogger::ScreenMessage(FColor::Yellow, bIsEquipped ? "True" : "False");
	ULogger::ScreenMessage(FColor::Orange, "bPendingEquip");
	ULogger::ScreenMessage(FColor::Orange, bPendingEquip ? "True" : "False");
	ULogger::ScreenMessage(FColor::Purple, "ACTOR_STATE");
	if (ACTOR_STATE == ECombatActorState::EQUIPPING) {
		ULogger::ScreenMessage(FColor::Purple, "EQUIPPING");
	} else if (ACTOR_STATE == ECombatActorState::IDLE)
	{
		ULogger::ScreenMessage(FColor::Purple, "IDLE");
	} else if (ACTOR_STATE == ECombatActorState::RELOADING)
	{
		ULogger::ScreenMessage(FColor::Purple, "RELOADING");
	} else if (ACTOR_STATE == ECombatActorState::USING)
	{
		ULogger::ScreenMessage(FColor::Purple, "USING");
	}
}

USoundCue* ACombatActor::PickRandomSound(TArray<USoundCue*> InSoundArray) {
	if (InSoundArray.Num() > 0) {
		int8 Index = FMath::RandRange(0, InSoundArray.Num() - 1);
		return InSoundArray[Index];
	}
	else {
		return nullptr;
	}
}

UAnimMontage* ACombatActor::PickRandomAnim(TArray<UAnimMontage*> InAnimArray) {
	if (InAnimArray.Num() > 0) {
		int8 Index = FMath::RandRange(0, InAnimArray.Num() - 1);
		return InAnimArray[Index];
	}
	else {
		return nullptr;
	}
}