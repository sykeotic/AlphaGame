#include "BaseCombatActor.h"
#include "Runtime/Engine/Classes/Engine/StaticMesh.h"
#include "TimerManager.h"
#include "PlayableCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Combat/Components/CombatComponent.h"

ABaseCombatActor::ABaseCombatActor()
{
	PrimaryActorTick.bCanEverTick = false;

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	RootComponent = MeshComp;
}

void ABaseCombatActor::BeginPlay()
{
	Super::BeginPlay();
	
}

void ABaseCombatActor::SetCombatActorData(FBaseCombatActorDataStruct InData)
{
	BaseCombatActorData = InData;
}

void ABaseCombatActor::SetCombatComponentOwner(UCombatComponent* InComponent)
{
	ComponentOwner = InComponent;
}

void ABaseCombatActor::AttachMeshToOwner(FName AttachPoint)
{
	if (ComponentOwner->GetCharacterOwner())
	{
		DetachMeshFromOwner();

		USkeletalMeshComponent* OwnerMesh = ComponentOwner->GetCharacterOwner()->GetMesh();
		MeshComp->SetHiddenInGame(false);
		MeshComp->AttachToComponent(OwnerMesh, FAttachmentTransformRules::SnapToTargetNotIncludingScale, AttachPoint);
	}
}

void ABaseCombatActor::DetachMeshFromOwner()
{
	MeshComp->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
	MeshComp->SetHiddenInGame(true);
}

void ABaseCombatActor::OnUse()
{
	if (!bWantsToUse) {
		bWantsToUse = true;
		AssertActorState();
	}
}

void ABaseCombatActor::ExecuteUse()
{

}


void ABaseCombatActor::AssignValues(UBaseCombatActorData* InData)
{
	BaseCombatActorData = InData->BaseCombatActorDataStruct;
	MeshComp->SetStaticMesh(BaseCombatActorData.MeshComp->GetStaticMesh());
	MeshComp->SetMaterial(0, BaseCombatActorData.ActorMaterial);
	MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void ABaseCombatActor::StopUse()
{
	if (bWantsToUse)
	{
		bWantsToUse = false;
		AssertActorState();
	}
}

bool ABaseCombatActor::CanUse()
{
	bool bOwnerCanFire = ComponentOwner->GetCharacterOwner() && ComponentOwner->GetCharacterOwner()->CharacterCanAttack();
	bool bStateOK = ACTOR_STATE == ECombatActorState::IDLE || ACTOR_STATE == ECombatActorState::USING;
	return bOwnerCanFire && bStateOK;
}

void ABaseCombatActor::HandleUse()
{
	if (CanUse())
	{
		if (GetNetMode() != NM_DedicatedServer)
		{
			// TODO
			// StartSimulatingActorUse();
		}

		if (ComponentOwner->GetCharacterOwner() && ComponentOwner->GetCharacterOwner()->IsLocallyControlled())
		{
			ExecuteUse();
			BurstCounter++;
		}
	}
	else if (ComponentOwner->GetCharacterOwner() && ComponentOwner->GetCharacterOwner()->IsLocallyControlled())
	{
		if (BurstCounter > 0)
		{
			OnBurstFinished();
		}
	}

	if (ComponentOwner->GetCharacterOwner() && ComponentOwner->GetCharacterOwner()->IsLocallyControlled())
	{

		bRefiring = (ACTOR_STATE == ECombatActorState::USING && BaseCombatActorData.UseCooldown > 0.0f);
		if (bRefiring)
		{
			GetWorldTimerManager().SetTimer(TimerHandle_HandleFiring, this, &ABaseCombatActor::HandleUse, BaseCombatActorData.UseCooldown, false);
		}
	}

	LastFireTime = GetWorld()->GetTimeSeconds();
}

void ABaseCombatActor::OnEquip(bool bPlayEquipAnim)
{
	bPendingEquip = true;
	AssertActorState();
	if (bPlayEquipAnim)
	{
		// float Duration = PlayActorAnimation(EquipAnim);
		// TODO
		float Duration = 0.0f;
		if (Duration <= 0.0f)
		{
			Duration = 0.5f;
		}
		EquipStartedTime = GetWorld()->TimeSeconds;
		EquipDuration = Duration;
		GetWorldTimerManager().SetTimer(EquipFinishedTimerHandle, this, &ABaseCombatActor::OnEquipFinished, Duration, false);
	}
	else
	{
		OnEquipFinished();
	}
}

void ABaseCombatActor::OnEquipFinished()
{
	AttachMeshToOwner(BaseCombatActorData.AttachToSocket);

	bIsEquipped = true;
	bPendingEquip = false;

	AssertActorState();
}

void ABaseCombatActor::OnUnEquip()
{
	bIsEquipped = false;
	StopUse();
	DetachMeshFromOwner();
	if (bPendingEquip)
	{
		// StopActorAnimation(EquipAnim);
		// TODO

		bPendingEquip = false;

		GetWorldTimerManager().ClearTimer(EquipFinishedTimerHandle);
	}
	AssertActorState();
}


void ABaseCombatActor::AssertActorState()
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

void ABaseCombatActor::SetCombatActorState(ECombatActorState InState)
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

void ABaseCombatActor::OnBurstStarted()
{
	const float GameTime = GetWorld()->GetTimeSeconds();
	NextValidFireTime = GameTime + BaseCombatActorData.UseCooldown;
	if (LastFireTime > 0 && BaseCombatActorData.UseCooldown > 0.0f &&
		LastFireTime + BaseCombatActorData.UseCooldown > GameTime)
	{
		GetWorldTimerManager().SetTimer(TimerHandle_HandleFiring, this, &ABaseCombatActor::HandleUse, LastFireTime + BaseCombatActorData.UseCooldown - GameTime, false);
	}
	else
	{
		NextValidFireTime = GameTime + BaseCombatActorData.UseCooldown;
		HandleUse();
	}
}


void ABaseCombatActor::OnBurstFinished()
{
	BurstCounter = 0;
	// StopSimulatingActorUse();

	GetWorldTimerManager().ClearTimer(TimerHandle_HandleFiring);
	bRefiring = false;
}

void ABaseCombatActor::OnRep_BurstCounter()
{
	if (BurstCounter > 0)
	{
		// StartSimulatingActorUse();
	}
	else
	{
		// StopSimulatingActorUse();
	}
}

UStaticMeshComponent* ABaseCombatActor::GetMesh()
{
	return MeshComp;
}

float ABaseCombatActor::GetNextValidFireTime()
{
	return 0.0f;
}
