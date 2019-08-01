#include "ThrowableProjectile.h"
#include "Runtime/Engine/Classes/GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "CombatActor.h"
#include "Logger.h"
#include "PlayableCharacter.h"
#include "Runtime/Engine/Classes/Engine/StaticMesh.h"
#include "Runtime/Engine/Classes/Particles/ParticleSystemComponent.h"
#include "Runtime/Engine/Classes/Sound/SoundCue.h"
#include "CombatComponent.h"
#include "Runtime/Engine/Classes/Components/SphereComponent.h"


AThrowableProjectile::AThrowableProjectile()
{
	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	CollisionComp->InitSphereRadius(15.0f);

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	RootComponent = MeshComp;

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComp"));
	ProjectileMovement->UpdatedComponent = MeshComp;
	ProjectileMovement->InitialSpeed = 3000.f;
	ProjectileMovement->MaxSpeed = 3000.f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bShouldBounce = true;
	ProjectileMovement->Bounciness = 0.3f;
}

void AThrowableProjectile::BeginPlay() {
	Super::BeginPlay();

}

void AThrowableProjectile::Fire(const FVector& ShootDirection) {
	ULogger::ScreenMessage(FColor::Red, "FIRING");
	CollisionComp->SetGenerateOverlapEvents(true);
	CollisionComp->OnComponentBeginOverlap.AddDynamic(this, &AThrowableProjectile::OnHit);
	if (ProjectileMovement)
	{
		ULogger::ScreenMessage(FColor::Red, "PROJ MOVE");
		ProjectileMovement->Velocity = ShootDirection * ProjectileMovement->InitialSpeed;
		GetWorldTimerManager().SetTimer(DeathTimer, this, &AThrowableProjectile::Detonate, 5.0f, false);
	}
}

void AThrowableProjectile::OnHit(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && (OtherActor != this) && OtherComp && CombatActorOwner != nullptr && OtherActor != CombatActorOwner && OtherActor != CombatActorOwner->ComponentOwner->CharacterOwner && OtherActor != CombatActorOwner->ComponentOwner->CurrentWeapon && OtherComp != CombatActorOwner->ComponentOwner->CharacterOwner->GetMesh())
	{
		UGameplayStatics::SpawnSoundAttached(PickRandomSound(ImpactSound), OtherActor->GetRootComponent());
		if (bDiesUponCollision) {
			this->Destroy();
		}
	}
	if (ProjectileMovement->Velocity.Size() <= 0) {
		Detonate();
	}
}

void AThrowableProjectile::Detonate() {
	UGameplayStatics::SpawnSoundAtLocation(GetWorld(), PickRandomSound(DetonationSound), this->GetActorLocation(), this->GetActorRotation());
	DetonationPSC = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), DetonationFX, this->GetActorTransform(), true);
	DetonationPSC->SetRelativeScale3D(FXScaleTransform);
	ULogger::ScreenMessage(FColor::Red, "THROWABLE DETONATED");
	Destroy();
}

void AThrowableProjectile::SetCombatActorOwner(ACombatActor* NewOwner) {
	CombatActorOwner = NewOwner;
	CollisionComp->IgnoreComponentWhenMoving(NewOwner->MeshComp, true);
	CollisionComp->IgnoreComponentWhenMoving(NewOwner->ComponentOwner->CharacterOwner->GetMesh(), true);
}

USoundCue* AThrowableProjectile::PickRandomSound(TArray<USoundCue*> InSoundArray) {
	if (InSoundArray.Num() > 0) {
		int8 Index = FMath::RandRange(0, InSoundArray.Num() - 1);
		return InSoundArray[Index];
	}
	else {
		return nullptr;
	}
}
