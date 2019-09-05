#include "BaseCombatActor.h"
#include "Runtime/Engine/Classes/Engine/StaticMesh.h"
#include "Logger.h"
#include "TimerManager.h"
#include "Feedback.h"
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
	MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	AddActorLocalRotation(BaseCombatActorData.MeshRotation);
	InitModifiers();
	// ULogger::ScreenMessage(FColor::Emerald, "BaseCombatActor::AssignValues || Modifiers: " + FString::FromInt(Modifiers.Num()));
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
	return bOwnerCanFire && bStateOK;
}

void ABaseCombatActor::HandleUse()
{
	if (CanUse())
	{
		if (GetNetMode() != NM_DedicatedServer)
		{
			FTimerHandle DelayAnimHandle;
			if (BaseCombatActorData.ExecutionDelay > 0) {
				GetWorldTimerManager().SetTimer(DelayAnimHandle, this, &ABaseCombatActor::StartSimulatingActorUse, BaseCombatActorData.ExecutionDelay, false);
			}
			else {
				StartSimulatingActorUse();
			}
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
			ULogger::ScreenMessage(FColor::Red, "HandleUse::Refiring");
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
		Modifier->AssignValues(BaseCombatActorData.ModifierData[i]->ModifierData);
		//ULogger::ScreenMessage(FColor::Cyan, "BaseCombatActor::InitModifiers || Effect Num: " + FString::FromInt(Modifier->GetEffectCount()));
		Modifiers.Add(Modifier);
	}
	//ULogger::ScreenMessage(FColor::Cyan, "BaseCombatActor::InitModifiers || Modifier Num: " + FString::FromInt(Modifiers.Num()));
}

void ABaseCombatActor::ApplyModifiers(AActor* InActor)
{
	APlayableCharacter* HitChar;
	HitChar = Cast<APlayableCharacter>(InActor);
	if (HitChar) {
		ULogger::ScreenMessage(FColor::Green, "BaseCombatActor::ApplyModifiers - Hit Char Valid");
		for (AModifier* CurrMod : Modifiers) {
			ULogger::ScreenMessage(FColor::Green, "BaseCombatActor::ApplyModifiers - Curr Mod Valid");
			HitChar->ApplyModifiers(CurrMod, InActor);
		}
	}
}

void ABaseCombatActor::StartSimulatingActorUse()
{
	if (BaseCombatActorData.Feedback) {
		GetWorldTimerManager().SetTimer(VisualFXTimerHandle, this, &ABaseCombatActor::PlayVisualFX, BaseCombatActorData.Feedback->VisualFXBuffer, false);
		USoundCue* SoundToPlay = BaseCombatActorData.Feedback->PickRandomSound();
		if (SoundToPlay) {
			PlaySoundFX(SoundToPlay);
		}
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
	if (BaseCombatActorData.Feedback->VisualFX)
	{
		UsePSC = UGameplayStatics::SpawnEmitterAttached(BaseCombatActorData.Feedback->VisualFX, MeshComp, BaseCombatActorData.VisualFXSpawnLocation);
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
		LastFireTime + BaseCombatActorData.UseCooldown + BaseCombatActorData.ExecutionDelay> GameTime)
	{
		ULogger::ScreenMessage(FColor::Red, "OnBurstStarted::If");
		GetWorldTimerManager().SetTimer(TimerHandle_HandleFiring, this, &ABaseCombatActor::HandleUse, LastFireTime + BaseCombatActorData.UseCooldown + BaseCombatActorData.ExecutionDelay - GameTime, false);
	}
	else
	{
		NextValidFireTime = GameTime + BaseCombatActorData.UseCooldown + BaseCombatActorData.ExecutionDelay;
		ULogger::ScreenMessage(FColor::Red, "OnBurstStarted::Else");
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
