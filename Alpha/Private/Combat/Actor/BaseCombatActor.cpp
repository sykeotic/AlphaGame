#include "BaseCombatActor.h"
#include "Runtime/Engine/Classes/Engine/StaticMesh.h"
#include "Logger.h"
#include "TimerManager.h"
#include "Combat/Modifiers/HandlerComponent.h"
#include "Feedback.h"
#include "Runtime/Engine/Classes/Engine/World.h"
#include "Modifier.h"
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

UCombatComponent* ABaseCombatActor::GetCombatComponentOwner()
{
	return ComponentOwner;
}

void ABaseCombatActor::AttachMeshToOwner(FName AttachPoint)
{
	if (ComponentOwner->GetCharacterOwner())
	{
		DetachMeshFromOwner();

		USkeletalMeshComponent* OwnerMesh = ComponentOwner->GetCharacterOwner()->GetMesh();
		MeshComp->SetHiddenInGame(false);
		MeshComp->AttachToComponent(OwnerMesh, FAttachmentTransformRules::SnapToTargetNotIncludingScale, AttachPoint);

		if (!BaseCombatActorData.AlwaysDisplayMesh) {
			MeshComp->SetVisibility(false);
		}
	}
}

void ABaseCombatActor::DetachMeshFromOwner()
{
	MeshComp->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
	MeshComp->SetHiddenInGame(true);
}

void ABaseCombatActor::OnUse()
{
	if (!bWantsToUse || bRefiring) {
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
	if (BaseCombatActorData.MeshComp) {
		MeshComp->SetStaticMesh(BaseCombatActorData.MeshComp->GetStaticMesh());
	}
	MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	AddActorLocalRotation(BaseCombatActorData.MeshRotation);
	InitModifiers();
}

void ABaseCombatActor::StopUse()
{
	if (bWantsToUse)
	{
		GetWorldTimerManager().ClearTimer(TimerHandle_HandleFiring);
		bWantsToUse = false;
		AssertActorState();
	}
}

bool ABaseCombatActor::CanUse()
{
	bool bOwnerCanFire = ComponentOwner->GetCharacterOwner() && ComponentOwner->GetCharacterOwner()->CharacterCanAttack();
	bool bStateOK = ACTOR_STATE == ECombatActorState::IDLE || ACTOR_STATE == ECombatActorState::USING;
	return bOwnerCanFire && bStateOK && bWantsToUse;
}

void ABaseCombatActor::HandleUse()
{
	if (CanUse())
	{
		if (GetNetMode() != NM_DedicatedServer)
		{
			FTimerHandle DelayAnimHandle;
			StartSimulatingActorUse();
		}

		if (ComponentOwner->GetCharacterOwner() && ComponentOwner->GetCharacterOwner()->IsLocallyControlled())
		{
			FTimerHandle DelayAnimHandle;
			if (BaseCombatActorData.ExecutionDelay > 0) {
				GetWorldTimerManager().SetTimer(DelayAnimHandle, this, &ABaseCombatActor::ExecuteUse, BaseCombatActorData.ExecutionDelay, false);
			}
			else {
				ExecuteUse();
			}
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
			GetWorldTimerManager().SetTimer(TimerHandle_HandleFiring, this, &ABaseCombatActor::HandleUse, BaseCombatActorData.UseCooldown + BaseCombatActorData.ExecutionDelay, false);
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
		float Duration = PlayActorAnimation(BaseCombatActorData.EquipAnim);
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

	AddActorLocalRotation(BaseCombatActorData.MeshRotation);

	AssertActorState();
}

void ABaseCombatActor::OnUnEquip()
{
	bIsEquipped = false;
	StopUse();
	DetachMeshFromOwner();
	if (bPendingEquip)
	{
		StopActorAnimation(BaseCombatActorData.EquipAnim);

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

TArray<AModifier*> ABaseCombatActor::GetModifiers()
{
	return Modifiers;
}

void ABaseCombatActor::InitModifiers()
{
	for (int i = 0; i < BaseCombatActorData.ModifierData.Num(); i++) {
		FActorSpawnParameters SpawnInfo;
		AModifier* Modifier;
		Modifier = Cast<AModifier>(GetWorld()->SpawnActor<AModifier>(BaseCombatActorData.ModifierData[i]->ModifierData.ModifierClass, FVector::ZeroVector, FRotator::ZeroRotator, SpawnInfo));
		Modifier->SetActorOwner(GetCombatComponentOwner()->GetCharacterOwner());
		Modifier->AssignValues(BaseCombatActorData.ModifierData[i]->ModifierData);
		Modifiers.Add(Modifier);
	}
}

void ABaseCombatActor::ApplyModifiers(AActor* InActor)
{
	if (InActor) {
		UHandlerComponent* TempHandler = InActor->FindComponentByClass<UHandlerComponent>();
		if (TempHandler && Modifiers.Num() > 0) {
			for (AModifier* CurrMod : Modifiers) {
				if (CurrMod && GetCombatComponentOwner() && GetCombatComponentOwner()->GetCharacterOwner()) {
					TempHandler->ActivateModifier(CurrMod, GetCombatComponentOwner()->GetCharacterOwner());
				}
			}
		}
	}
}

void ABaseCombatActor::StartSimulatingActorUse()
{
	if (BaseCombatActorData.OnUseFeedback) {
		GetWorldTimerManager().SetTimer(VisualFXTimerHandle, this, &ABaseCombatActor::PlayVisualFX, BaseCombatActorData.OnUseFeedback->VisualFXBuffer, false);
		FTimerHandle SoundDelay;
		GetWorldTimerManager().SetTimer(SoundDelay, [this]() {
			USoundCue* SoundToPlayObj = BaseCombatActorData.OnUseFeedback->PickRandomSound();				
			PlaySoundFX(SoundToPlayObj);
		}, BaseCombatActorData.OnUseFeedback->SoundFXBuffer, false);			
	}
	if (!bPlayingUseAnimation)
	{
		int8 AnimIndex = FMath::RandRange(0, BaseCombatActorData.UseAnim.Num() - 1);
		CurrentAnim = BaseCombatActorData.UseAnim[AnimIndex];
		PlayActorAnimation(CurrentAnim);
		bPlayingUseAnimation = true;
	}
}

void ABaseCombatActor::StopSimulatingActorUse()
{
	if (bPlayingUseAnimation)
	{
		StopActorAnimation(CurrentAnim);
		bPlayingUseAnimation = false;
	}
}

void ABaseCombatActor::PlayVisualFX()
{
	if (BaseCombatActorData.OnUseFeedback->VisualFX)
	{
		if (MeshComp) {
			UsePSC = UGameplayStatics::SpawnEmitterAttached(BaseCombatActorData.OnUseFeedback->VisualFX, MeshComp, BaseCombatActorData.VisualFXSpawnLocation);
		}
	}
}

UAudioComponent* ABaseCombatActor::PlaySoundFX(USoundCue* InSound)
{
	UAudioComponent* AC = nullptr;
	if (!bPlayingSound || bPlaySoundEveryTime) {
		if (InSound && ComponentOwner->GetCharacterOwner())
		{
			bPlayingSound = true;
			AC = UGameplayStatics::SpawnSoundAttached(InSound, ComponentOwner->GetCharacterOwner()->GetRootComponent());
			GetWorldTimerManager().SetTimer(SoundTimerHandle, this, &ABaseCombatActor::SetSoundPlayingToFalse, InSound->GetDuration(), false);
		}
	}
	return AC;
}

void ABaseCombatActor::SetSoundPlayingToFalse()
{
	bPlayingSound = false;
}

float ABaseCombatActor::PlayActorAnimation(UAnimMontage* Animation, float InPlayRate, FName StartSectionName)
{
	float Duration = 0.0f;
	if (ComponentOwner->GetCharacterOwner())
	{
		if (Animation)
		{
			Duration = ComponentOwner->GetCharacterOwner()->PlayAnimMontage(Animation, InPlayRate, StartSectionName);
		}
	}
	else {
	}
	GetWorldTimerManager().SetTimer(AnimationTimerHandle, this, &ABaseCombatActor::StopSimulatingActorUse, Duration, false);
	return Duration;
}

void ABaseCombatActor::StopActorAnimation(UAnimMontage* InAnim)
{
	if (ComponentOwner->GetCharacterOwner())
	{
		if (InAnim)
		{
			ComponentOwner->GetCharacterOwner()->StopAnimMontage(InAnim);
		}
	}
}

void ABaseCombatActor::OnBurstStarted()
{
	const float GameTime = GetWorld()->GetTimeSeconds();
	NextValidFireTime = GameTime + BaseCombatActorData.UseCooldown + BaseCombatActorData.ExecutionDelay;
	if (LastFireTime > 0 && BaseCombatActorData.UseCooldown > 0.0f &&
		LastFireTime + BaseCombatActorData.UseCooldown + BaseCombatActorData.ExecutionDelay >= GameTime)
	{
		GetWorldTimerManager().SetTimer(TimerHandle_HandleFiring, this, &ABaseCombatActor::HandleUse, LastFireTime + BaseCombatActorData.UseCooldown + BaseCombatActorData.ExecutionDelay - GameTime, false);
	}
	else
	{
		NextValidFireTime = GameTime + BaseCombatActorData.UseCooldown + BaseCombatActorData.ExecutionDelay;
		HandleUse();
	}
}


void ABaseCombatActor::OnBurstFinished()
{
	BurstCounter = 0;
	StopSimulatingActorUse();
	GetWorldTimerManager().ClearTimer(TimerHandle_HandleFiring);
	bRefiring = false;
}

void ABaseCombatActor::OnRep_BurstCounter()
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

UStaticMeshComponent* ABaseCombatActor::GetMesh()
{
	return MeshComp;
}

float ABaseCombatActor::GetNextValidFireTime()
{
	return NextValidFireTime;
}

void ABaseCombatActor::SetNextValidFireTime(float InTime)
{
	NextValidFireTime = InTime;
}
