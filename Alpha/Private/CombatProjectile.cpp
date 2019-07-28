#include "CombatProjectile.h"
#include "Runtime/Engine/Classes/GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "CombatActor.h"
#include "Logger.h"
#include "PlayableCharacter.h"
#include "CombatComponent.h"
#include "Runtime/Engine/Classes/Components/SphereComponent.h"

ACombatProjectile::ACombatProjectile()
{
	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	CollisionComp->InitSphereRadius(15.0f);
	RootComponent = CollisionComp;

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComp"));
	ProjectileMovement->UpdatedComponent = CollisionComp;
	ProjectileMovement->InitialSpeed = 3000.f;
	ProjectileMovement->MaxSpeed = 3000.f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bShouldBounce = true;
	ProjectileMovement->Bounciness = 0.3f;
}

void ACombatProjectile::BeginPlay() {
	Super::BeginPlay();
	CollisionComp->SetGenerateOverlapEvents(true);
	CollisionComp->OnComponentBeginOverlap.AddDynamic(this, &ACombatProjectile::OnHit);
}

void ACombatProjectile::Fire(const FVector& ShootDirection) {
	if (ProjectileMovement)
	{
		ProjectileMovement->Velocity = ShootDirection * ProjectileMovement->InitialSpeed;
	}
}

void ACombatProjectile::OnHit(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && (OtherActor != this) && OtherComp && CombatActorOwner != nullptr && OtherActor != CombatActorOwner->ComponentOwner->CharacterOwner)
	{
		FRotator OwnerView;
		FVector OwnerLoc;
		CombatActorOwner->ComponentOwner->GetOwner()->GetActorEyesViewPoint(OwnerLoc, OwnerView);
		FHitResult Hit;
		FVector ShotDirection = OwnerView.Vector();
		APlayableCharacter* DamagedChar = Cast<APlayableCharacter>(OtherActor);
		if (DamagedChar) {
			ULogger::ScreenMessage(FColor::Red, "Damaged Char Is Character");
			UGameplayStatics::ApplyPointDamage(OtherActor, this->CombatActorOwner->ResolveDamageModifiers(this->CombatActorOwner->ComponentOwner->CharacterOwner, DamagedChar, this->CombatActorOwner), ShotDirection, Hit, CombatActorOwner->ComponentOwner->CharacterOwner->GetInstigatorController(), this, CombatActorOwner->DamageType);
		} else {
			UGameplayStatics::ApplyPointDamage(OtherActor, CombatActorOwner->Damage, ShotDirection, Hit, CombatActorOwner->ComponentOwner->CharacterOwner->GetInstigatorController(), this, CombatActorOwner->DamageType);
		}
		if (bDiesUponCollision) {
			this->Destroy();
		}
	}
}

void ACombatProjectile::SetCombatActorOwner(ACombatActor* NewOwner) {
	CombatActorOwner = NewOwner;
	CollisionComp->IgnoreComponentWhenMoving(NewOwner->MeshComp, true);
	CollisionComp->IgnoreComponentWhenMoving(NewOwner->ComponentOwner->CharacterOwner->GetMesh(), true);
}