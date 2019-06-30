#include "CombatActor.h"
#include "TimerManager.h"
#include "Runtime/Engine/Classes/Engine/StaticMesh.h"
#include "CombatComponent.h"
#include "CombatUtils.h"
#include "PlayableCharacter.h"
#include "Runtime/Engine/Classes/Sound/SoundCue.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
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

	TimeBetweenShots = 60.0f / 128.5f;
}

void ACombatActor::BeginPlay() {
	Super::BeginPlay();
}

void ACombatActor::OnEquip(bool bPlayAnimation) {
	bPendingEquip = true;
	AssertActorState();
	ULogger::ScreenMessage(FColor::Green, "Equip");
	if (bPlayAnimation)
	{
		ULogger::ScreenMessage(FColor::Green, "Equip Can Play Anim");
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
	//BoolSpam();
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
	if (LastFireTime > 0 && UseCooldown > 0.0f &&
		LastFireTime + UseCooldown > GameTime)
	{
		GetWorldTimerManager().SetTimer(TimerHandle_HandleFiring, this, &ACombatActor::HandleUse, LastFireTime + UseCooldown - GameTime, false);
	}
	else
	{
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

void ACombatActor::HandleUse() {
	if (CanUse())
	{
		if (GetNetMode() != NM_DedicatedServer)
		{
			ULogger::ScreenMessage(FColor::Green, "HANDLE USE Simulating Actor Use");
			StartSimulatingActorUse();
		}

		if (ComponentOwner->CharacterOwner && ComponentOwner->CharacterOwner->IsLocallyControlled())
		{
			OnUse();

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

		bRefiring = (ACTOR_STATE == ECombatActorState::USING && TimeBetweenShots > 0.0f);
		if (bRefiring)
		{
			GetWorldTimerManager().SetTimer(TimerHandle_HandleFiring, this, &ACombatActor::HandleUse, TimeBetweenShots, false);
		}
	}

	LastFireTime = GetWorld()->GetTimeSeconds();
}

UAudioComponent* ACombatActor::PlayActorSound(USoundCue* SoundToPlay)
{
	UAudioComponent* AC = nullptr;
	if (SoundToPlay && ComponentOwner->CharacterOwner)
	{
		AC = UGameplayStatics::SpawnSoundAttached(SoundToPlay, ComponentOwner->CharacterOwner->GetRootComponent());
	}

	return AC;
}



void ACombatActor::StartSimulatingActorUse(){

	if (ActorFX)
	{
		ULogger::ScreenMessage(FColor::Red, ProjectileSpawnLocation.ToString());
		UsePSC = UGameplayStatics::SpawnEmitterAttached(ActorFX, MeshComp, ProjectileSpawnLocation);
	}

	ULogger::ScreenMessage(FColor::Green, "Simulating Use");
	if (!bPlayingFireAnim)
	{
		ULogger::ScreenMessage(FColor::Green, "Branch Simulating Use");
		PlayActorAnimation(FireAnim);
		bPlayingFireAnim = true;
	}

	PlayActorSound(UseSound);
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
	ULogger::ScreenMessage(FColor::Green, "Stopping Use");
	if (bPlayingFireAnim)
	{
		ULogger::ScreenMessage(FColor::Green, "Branch Stopping Use");
		StopActorAnimation(FireAnim);
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