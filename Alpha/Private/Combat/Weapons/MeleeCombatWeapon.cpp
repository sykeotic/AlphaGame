#include "MeleeCombatWeapon.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Logger.h"
#include "CombatComponent.h"
#include "PlayableCharacter.h"
#include "Logger.h"

AMeleeCombatWeapon::AMeleeCombatWeapon()
{
	PrimaryActorTick.bCanEverTick = false;
	MeleeBoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("MeleeBoxComponent"));
	MeleeBoxComponent->SetupAttachment(RootComponent);	
}

void AMeleeCombatWeapon::BeginPlay() {
	Super::BeginPlay();
	ULogger::ScreenMessage(FColor::Blue, "Melee Begin Play");
	MeleeBoxComponent->SetGenerateOverlapEvents(true);
	MeleeBoxComponent->OnComponentBeginOverlap.AddDynamic(this, &AMeleeCombatWeapon::WeaponBeginOverlap);
}

void AMeleeCombatWeapon::OnUse() {
	Super::OnUse();
	ULogger::ScreenMessage(FColor::Red, "Melee Swinging");
}

void AMeleeCombatWeapon::AttachMeshToOwner(FName AttachPoint) {
	Super::AttachMeshToOwner(AttachPoint);
}

void AMeleeCombatWeapon::WeaponBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {
	
	if (ComponentOwner != nullptr) {
		if ((OtherActor != nullptr) && (OtherActor != ComponentOwner->CharacterOwner) && (OtherComp != nullptr) && (OtherActor != this) && ACTOR_STATE == ECombatActorState::USING)
		{
			ULogger::ScreenMessage(FColor::Red, "Overlap called");
			FRotator OwnerView;
			FVector OwnerLoc;
			ComponentOwner->GetOwner()->GetActorEyesViewPoint(OwnerLoc, OwnerView);
			FHitResult Hit;
			FVector ShotDirection = OwnerView.Vector();
			UGameplayStatics::ApplyPointDamage(OtherActor, Damage, ShotDirection, Hit, ComponentOwner->CharacterOwner->GetInstigatorController(), this, DamageType);
		}
	}
}