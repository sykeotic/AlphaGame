#include "MeleeCombatWeapon.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Logger.h"
#include "CombatUtils.h"
#include "CombatComponent.h"
#include "PlayableCharacter.h"

AMeleeCombatWeapon::AMeleeCombatWeapon()
{
	PrimaryActorTick.bCanEverTick = false;
	MeleeBoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("MeleeBoxComponent"));
	MeleeBoxComponent->SetupAttachment(RootComponent);	
}

void AMeleeCombatWeapon::BeginPlay() {
	Super::BeginPlay();
	MeleeBoxComponent->SetGenerateOverlapEvents(true);
	MeleeBoxComponent->OnComponentBeginOverlap.AddDynamic(this, &AMeleeCombatWeapon::WeaponBeginOverlap);
}

void AMeleeCombatWeapon::OnUse() {
	Super::OnUse();
}

void AMeleeCombatWeapon::ExecuteUse() {

}

void AMeleeCombatWeapon::AttachMeshToOwner(FName AttachPoint) {
	Super::AttachMeshToOwner(AttachPoint);
}

void AMeleeCombatWeapon::WeaponBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {
	
	if (ComponentOwner != nullptr && ACTOR_STATE == ECombatActorState::USING && bPlayingFireAnim) {
		if ((OtherActor != nullptr) && (OtherActor != ComponentOwner->CharacterOwner) && (OtherComp != nullptr) && (OtherActor != this) && ACTOR_STATE == ECombatActorState::USING)
		{
			FRotator OwnerView;
			FVector OwnerLoc;
			ComponentOwner->GetOwner()->GetActorEyesViewPoint(OwnerLoc, OwnerView);
			FHitResult Hit;
			FVector ShotDirection = OwnerView.Vector();
			APlayableCharacter* DamagedChar = Cast<APlayableCharacter>(OtherActor);
			if (DamagedChar) {
				ULogger::ScreenMessage(FColor::Red, "Damaged Char Is Character");
				UGameplayStatics::ApplyPointDamage(OtherActor, ResolveDamageModifiers(this->ComponentOwner->CharacterOwner, DamagedChar, this), ShotDirection, Hit, ComponentOwner->CharacterOwner->GetInstigatorController(), this, DamageType);
			} else {
				ULogger::ScreenMessage(FColor::Red, "Damaged Char Is Not Character");
				UGameplayStatics::ApplyPointDamage(OtherActor, Damage, ShotDirection, Hit, ComponentOwner->CharacterOwner->GetInstigatorController(), this, DamageType);
			}
			int8 SoundIndex = FMath::RandRange(0, ImpactSound.Num() - 1);
			PlayActorSound(ImpactSound[SoundIndex]);
		}
	}
}