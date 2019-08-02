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
	MeshComp->SetSimulatePhysics(true);
	RootComponent = MeshComp;
	CollisionComp->SetupAttachment(RootComponent);

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
	ProjectileMovement->InitialSpeed = InitialSpeed;
	ProjectileMovement->MaxSpeed = MaxSpeed;
	ProjectileMovement->bRotationFollowsVelocity = bRotationFollowsVelocity;
	ProjectileMovement->bShouldBounce = bShouldBounce;
	ProjectileMovement->Bounciness = Bounciness;
	CollisionComp->SetSphereRadius(BlastRadius);

}

void AThrowableProjectile::Fire(const FVector& ShootDirection) {
	ULogger::ScreenMessage(FColor::Red, "FIRING");
	MeshComp->SetGenerateOverlapEvents(true);
	CollisionComp->SetGenerateOverlapEvents(true);
	MeshComp->OnComponentBeginOverlap.AddDynamic(this, &AThrowableProjectile::OnHit);
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
	if (ProjectileMovement->Velocity.Size() <= 10) {
		ProjectileMovement->Velocity = FVector::ZeroVector;
		Detonate();
	}
}

void AThrowableProjectile::Detonate() {
	FVector DetVector = this->GetActorLocation();
	DetVector.Z += FXLocationTransform.Z;
	DetVector.Y += FXLocationTransform.Y;
	DetVector.X += FXLocationTransform.X;
	FRotator DetRot = this->GetActorRotation();
	UGameplayStatics::SpawnSoundAtLocation(GetWorld(), PickRandomSound(DetonationSound), this->GetActorLocation(), this->GetActorRotation());
	DetonationPSC = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), DetonationFX, DetVector, DetRot, true);
	DetonationPSC->SetRelativeScale3D(FXScaleTransform);
	FRotator OwnerView;
	FVector OwnerLoc;
	FHitResult Hit;
	ULogger::ScreenMessage(FColor::Red, "THROWABLE DETONATED");
	TArray<AActor*> OverActors;
	CollisionComp->GetOverlappingActors(OverActors);
	for (int i = 0; i < OverActors.Num(); i++) {
		AActor* OtherActor = OverActors[i];
		ULogger::ScreenMessage(FColor::Red, "ACTORS > 0");
		if (FVector::Dist(MeshComp->GetComponentLocation(), OtherActor->GetActorLocation()) <= BlastRadius) {
			ULogger::ScreenMessage(FColor::Red, "ACTOR DAMAGED");
			CombatActorOwner->ComponentOwner->GetOwner()->GetActorEyesViewPoint(OwnerLoc, OwnerView);
			FVector ShotDirection = OwnerView.Vector();
			APlayableCharacter* DamagedChar = Cast<APlayableCharacter>(OtherActor);
			if (DamagedChar) {
				UGameplayStatics::ApplyPointDamage(OtherActor, this->CombatActorOwner->ResolveDamageModifiers(this->CombatActorOwner->ComponentOwner->CharacterOwner, DamagedChar, this->CombatActorOwner), ShotDirection, Hit, CombatActorOwner->ComponentOwner->CharacterOwner->GetInstigatorController(), this, CombatActorOwner->DamageType);
			}
			else {
				ULogger::ScreenMessage(FColor::Red, "ACTOR CAST FAILED ");
			}
		}
	}
	Destroy();
}

void AThrowableProjectile::SetCombatActorOwner(ACombatActor* NewOwner) {
	CombatActorOwner = NewOwner;
	MeshComp->IgnoreComponentWhenMoving(NewOwner->MeshComp, true);
	MeshComp->IgnoreComponentWhenMoving(NewOwner->ComponentOwner->CharacterOwner->GetMesh(), true);
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
