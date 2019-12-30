#include "MeleeCombatActor.h"
#include "TimerManager.h"
#include "MeleeCombatActorData.h"
#include "Logger.h"
#include "PlayableCharacter.h"
#include "Combat/Components/CombatComponent.h"
#include "BaseCombatActorData.h"
#include "Components/SphereComponent.h"
#include "Runtime/Engine/Classes/Components/BoxComponent.h"

AMeleeCombatActor::AMeleeCombatActor()
{
	PrimaryActorTick.bCanEverTick = false;
	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	SphereComponent->SetupAttachment(RootComponent);
}

void AMeleeCombatActor::BeginPlay()
{
	Super::BeginPlay();
	SphereComponent->SetGenerateOverlapEvents(true);
	SphereComponent->OnComponentBeginOverlap.AddDynamic(this, &AMeleeCombatActor::ActorBeginOverlap);
	SphereComponent->OnComponentEndOverlap.AddDynamic(this, &AMeleeCombatActor::ActorEndOverlap);
	SectionCounter = 0;
}

void AMeleeCombatActor::OnUse() {
	Super::OnUse();
}

void AMeleeCombatActor::ExecuteUse()
{
	ApplyModifiersToSphere();
}

void AMeleeCombatActor::AssignValues(UBaseCombatActorData* InData)
{
	Super::AssignValues(InData);
	UMeleeCombatActorData* TempData = Cast<UMeleeCombatActorData>(InData);
	MeleeCombatActorStruct = TempData->MeleeCombatActorDataStruct;
	SphereComponent->SetSphereRadius(MeleeCombatActorStruct.SphereRadius);
}

void AMeleeCombatActor::PlayVisualFX()
{
	Super::PlayVisualFX();
}

void AMeleeCombatActor::ActorBeginOverlap(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if (GetCombatComponentOwner() != nullptr && GetCombatComponentOwner() != NULL) {
		if (!OverlappedActors.Contains(OtherActor) && (OtherActor != nullptr) && (OtherActor != GetCombatComponentOwner()->GetCharacterOwner()) && (OtherComp != nullptr) && (OtherActor != this)) {
			OverlappedActors.AddUnique(OtherActor);
		}
	}
}

void AMeleeCombatActor::ActorEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (GetCombatComponentOwner() != nullptr && GetCombatComponentOwner() != NULL) {
		if (OverlappedActors.Contains(OtherActor) && (OtherActor != nullptr) && (OtherActor != GetCombatComponentOwner()->GetCharacterOwner()) && (OtherComp != nullptr) && (OtherActor != this)) {
			OverlappedActors.Remove(OtherActor);
		}
	}
}

void AMeleeCombatActor::ApplyModifiersToSphere()
{
	for (AActor* CurrActor : OverlappedActors) {
		ApplyModifiers(CurrActor);
	}
}
