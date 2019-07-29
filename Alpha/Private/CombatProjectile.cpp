#include "CombatProjectile.h"
#include "Runtime/Engine/Classes/GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "CombatActor.h"
#include "Logger.h"
#include "PlayableCharacter.h"
#include "Runtime/Engine/Classes/Particles/ParticleSystemComponent.h"
#include "Runtime/Engine/Classes/Sound/SoundCue.h"
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

}

void ACombatProjectile::Fire(const FVector& ShootDirection) {
	CollisionComp->SetGenerateOverlapEvents(true);/*
	CollisionComp->IgnoreActorWhenMoving(CombatActorOwner, true);
	CollisionComp->IgnoreComponentWhenMoving(CombatActorOwner->MeshComp, true);
	CollisionComp->IgnoreActorWhenMoving(CombatActorOwner->ComponentOwner->CharacterOwner, true);
	CollisionComp->IgnoreComponentWhenMoving(CombatActorOwner->ComponentOwner->CharacterOwner->GetMesh(), true);
	CollisionComp->IgnoreActorWhenMoving(CombatActorOwner->ComponentOwner->CurrentAbility, true);
	CollisionComp->IgnoreComponentWhenMoving(CombatActorOwner->ComponentOwner->CurrentAbility->MeshComp, true);
	CollisionComp->IgnoreActorWhenMoving(CombatActorOwner->ComponentOwner->CurrentWeapon, true);
	CollisionComp->IgnoreComponentWhenMoving(CombatActorOwner->ComponentOwner->CurrentWeapon->MeshComp, true);*/
	CollisionComp->OnComponentBeginOverlap.AddDynamic(this, &ACombatProjectile::OnHit);
	if (ProjectileMovement)
	{
		ProjectileMovement->Velocity = ShootDirection * ProjectileMovement->InitialSpeed;
	}
}

void ACombatProjectile::OnHit(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && (OtherActor != this) && OtherComp && CombatActorOwner != nullptr && OtherActor != CombatActorOwner && OtherActor != CombatActorOwner->ComponentOwner->CharacterOwner && OtherActor != CombatActorOwner->ComponentOwner->CurrentWeapon && OtherComp != CombatActorOwner->ComponentOwner->CharacterOwner->GetMesh())
	{
		FRotator OwnerView;
		FVector OwnerLoc;
		CombatActorOwner->ComponentOwner->GetOwner()->GetActorEyesViewPoint(OwnerLoc, OwnerView);
		FHitResult Hit;
		FVector ShotDirection = OwnerView.Vector();
		APlayableCharacter* DamagedChar = Cast<APlayableCharacter>(OtherActor);
		if (DamagedChar) {
			UsePSC = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactFX, OtherActor->GetActorTransform(), true);
			UGameplayStatics::SpawnSoundAttached(PickRandomSound(), OtherActor->GetRootComponent());
			ULogger::ScreenMessage(FColor::Red, "Damaged Char Is Character");
			UGameplayStatics::ApplyPointDamage(OtherActor, this->CombatActorOwner->ResolveDamageModifiers(this->CombatActorOwner->ComponentOwner->CharacterOwner, DamagedChar, this->CombatActorOwner), ShotDirection, Hit, CombatActorOwner->ComponentOwner->CharacterOwner->GetInstigatorController(), this, CombatActorOwner->DamageType);
		} else {
			UsePSC = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactFX, OtherActor->GetActorTransform(), true);
			UGameplayStatics::SpawnSoundAttached(PickRandomSound(), OtherActor->GetRootComponent());
			UGameplayStatics::ApplyPointDamage(OtherActor, CombatActorOwner->Damage, ShotDirection, Hit, CombatActorOwner->ComponentOwner->CharacterOwner->GetInstigatorController(), this, CombatActorOwner->DamageType);
		}
		UsePSC->SetRelativeScale3D(FXScaleTransform);
		if (bDiesUponCollision) {
			this->Destroy();
		}
	}
}

USoundCue* ACombatProjectile::PickRandomSound() {
	int8 Index = FMath::RandRange(0, ImpactSound.Num() - 1);
	return ImpactSound[Index];
}

void ACombatProjectile::SetCombatActorOwner(ACombatActor* NewOwner) {
	CombatActorOwner = NewOwner;
	CollisionComp->IgnoreComponentWhenMoving(NewOwner->MeshComp, true);
	CollisionComp->IgnoreComponentWhenMoving(NewOwner->ComponentOwner->CharacterOwner->GetMesh(), true);
}