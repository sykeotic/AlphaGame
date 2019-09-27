#include "MeleeCombatActor.h"
#include "TimerManager.h"
#include "MeleeCombatActorData.h"
#include "Logger.h"
#include "PlayableCharacter.h"
#include "Combat/Components/CombatComponent.h"
#include "BaseCombatActorData.h"
#include "Runtime/Engine/Classes/Components/BoxComponent.h"

AMeleeCombatActor::AMeleeCombatActor()
{
	PrimaryActorTick.bCanEverTick = false;
	MeleeBoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("MeleeBoxComponent"));
	MeleeBoxComponent->SetupAttachment(RootComponent);
}

void AMeleeCombatActor::BeginPlay()
{
	Super::BeginPlay();
	MeleeBoxComponent->SetGenerateOverlapEvents(true);
	MeleeBoxComponent->OnComponentBeginOverlap.AddDynamic(this, &AMeleeCombatActor::WeaponBeginOverlap);
	SectionCounter = 0;
}

void AMeleeCombatActor::OnUse() {
	Super::OnUse();
}

void AMeleeCombatActor::ExecuteUse()
{
	SectionCounter = 0;
	// ULogger::ScreenMessage(FColor::Magenta, "AMeleeCombatActor::ExecuteUse");
	bCanOverlap = true;
	GetWorldTimerManager().SetTimer(OverlapWindowTimer, this, &AMeleeCombatActor::SetOverlappingToFalse, CurrentAnim->GetPlayLength(), false);
	ClearOverlappedArray();
}

void AMeleeCombatActor::AssignValues(UBaseCombatActorData* InData)
{
	Super::AssignValues(InData);
	UMeleeCombatActorData* TempData = Cast<UMeleeCombatActorData>(InData);
	MeleeCombatActorStruct = TempData->MeleeCombatActorDataStruct;
	MeleeBoxComponent->SetRelativeTransform(MeleeCombatActorStruct.CollisionHitBox->GetRelativeTransform());
	MeleeBoxComponent->AddRelativeRotation(InData->BaseCombatActorDataStruct.MeshRotation);
}

void AMeleeCombatActor::WeaponBeginOverlap(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if (GetCombatComponentOwner() != nullptr && GetCombatComponentOwner() != NULL) {
		if (!OverlappedActors.Contains(OtherActor) && (OtherActor != nullptr) && (OtherActor != GetCombatComponentOwner()->GetCharacterOwner()) && (OtherComp != nullptr) && (OtherActor != this) && bCanOverlap) {
			OverlappedActors.AddUnique(OtherActor);
			ApplyModifiers(OtherActor);
			ULogger::ScreenMessage(FColor::Orange, "MELEE HIT");
		}
	}
}

void AMeleeCombatActor::SetOverlappingToFalse()
{
	// ULogger::ScreenMessage(FColor::Magenta, "AMeleeCombatActor::SetOverlappingToFalse");
	bCanOverlap = false;
	OverlappedActors.Empty();
}

void AMeleeCombatActor::ClearOverlappedArray()
{
	if (ACTOR_STATE == ECombatActorState::USING) {
		// ULogger::ScreenMessage(FColor::Magenta, "AMeleeCombatActor::ClearOverlappedArray - TRUE");
		OverlappedActors.Empty();
		GetWorldTimerManager().SetTimer(ClearTimer, this, &AMeleeCombatActor::ClearOverlappedArray, CurrentAnim->GetSectionLength(SectionCounter++), false);
	}
	else {
		// ULogger::ScreenMessage(FColor::Magenta, "AMeleeCombatActor::ClearOverlappedArray - FALSE");
	}
}
