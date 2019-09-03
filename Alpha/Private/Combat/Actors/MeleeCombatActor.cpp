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
}

void AMeleeCombatActor::OnUse() {
	Super::OnUse();
}

void AMeleeCombatActor::ExecuteUse()
{
	bCanOverlap = true;
	GetWorldTimerManager().SetTimer(OverlapWindowTimer, this, &AMeleeCombatActor::SetOverlappingToFalse, MeleeCombatActorStruct.HitBoxActiveDuration, false);
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
			ULogger::ScreenMessage(FColor::Orange, "MELEE HIT");
		}
	}
}

void AMeleeCombatActor::SetOverlappingToFalse()
{
	OverlappedActors.Empty();
	bCanOverlap = false;
}
