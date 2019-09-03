#include "BaseProjectile.h"
#include "Runtime/Engine/Classes/Engine/StaticMesh.h"
#include "Runtime/Engine/Classes/Components/SphereComponent.h"
#include "BaseCombatActor.h"
#include "Kismet/GameplayStatics.h"
#include "Runtime/Engine/Classes/Particles/ParticleSystemComponent.h"
#include "Runtime/Engine/Classes/Sound/SoundCue.h"
#include "Runtime/Engine/Public/DrawDebugHelpers.h"
#include "Feedback.h"
#include "Runtime/Engine/Classes/GameFramework/ProjectileMovementComponent.h"
#include "Logger.h"

ABaseProjectile::ABaseProjectile()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;

	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	CollisionComp->InitSphereRadius(15.0f);
	RootComponent = CollisionComp;

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	MeshComp->SetupAttachment(CollisionComp);

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComp"));
	ProjectileMovement->UpdatedComponent = CollisionComp;
	ProjectileMovement->InitialSpeed = 3000.f;
	ProjectileMovement->MaxSpeed = 3000.f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bShouldBounce = true;
	ProjectileMovement->ProjectileGravityScale = 0;
	ProjectileMovement->Bounciness = 0.3f;

	InitialLifeSpan = 3.0f;
}

void ABaseProjectile::Tick(float DeltaSeconds)
{

}

void ABaseProjectile::BeginPlay()
{
	Super::BeginPlay();	
}

void ABaseProjectile::Fire(const FVector& ShootDirection)
{
	CollisionComp->SetGenerateOverlapEvents(true);
	CollisionComp->OnComponentBeginOverlap.AddDynamic(this, &ABaseProjectile::OnHit);
	if (ProjectileMovement)
	{
		ProjectileMovement->Velocity = ShootDirection * ProjectileMovement->InitialSpeed;
		ProjectileMovement->UpdateComponentVelocity();
		DrawDebugLine(GetWorld(), ShootDirection, ProjectileMovement->Velocity, FColor::Red);
	}
}

void ABaseProjectile::SetCombatActorOwner(ABaseCombatActor* NewOwner)
{
	CombatActorOwner = NewOwner;
	CollisionComp->IgnoreComponentWhenMoving(CombatActorOwner->GetMesh(), true);
	CollisionComp->IgnoreComponentWhenMoving(CombatActorOwner->GetCombatComponentOwner()->GetCharacterOwner()->GetMesh(), true);
	CollisionComp->IgnoreComponentWhenMoving(MeshComp, true);
	CollisionComp->IgnoreActorWhenMoving(CombatActorOwner, true);
	CollisionComp->IgnoreActorWhenMoving(CombatActorOwner->GetCombatComponentOwner()->GetCharacterOwner(), true);

	MeshComp->IgnoreComponentWhenMoving(CombatActorOwner->GetMesh(), true);
	MeshComp->IgnoreComponentWhenMoving(CombatActorOwner->GetCombatComponentOwner()->GetCharacterOwner()->GetMesh(), true);
	MeshComp->IgnoreComponentWhenMoving(CollisionComp, true);
	MeshComp->IgnoreActorWhenMoving(CombatActorOwner, true);
	MeshComp->IgnoreActorWhenMoving(CombatActorOwner->GetCombatComponentOwner()->GetCharacterOwner(), true);
}

void ABaseProjectile::InitProjectileData(FProjectileDataStruct InData, FVector InLocation)
{
	ProjectileData = InData;
	CollisionComp->SetSphereRadius(ProjectileData.SphereRadius);
	CollisionComp->SetCollisionProfileName(ProjectileData.CollisionProfile);

	ProjectileMovement->InitialSpeed = ProjectileData.InitialSpeed;
	ProjectileMovement->MaxSpeed = ProjectileData.MaxSpeed;
	ProjectileMovement->bRotationFollowsVelocity = ProjectileData.bRotationFollowsVelocity;
	ProjectileMovement->bShouldBounce = ProjectileData.bShouldBounce;
	ProjectileMovement->Bounciness = ProjectileData.Bounciness;
	ProjectileMovement->ProjectileGravityScale = ProjectileData.GravityScale;

	MeshComp->SetStaticMesh(ProjectileData.MeshComp);
	MeshComp->SetCollisionResponseToAllChannels(ECR_Ignore);
}

void ABaseProjectile::OnHit(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (ProjectileData.bDiesUponCollision) {
		this->Destroy();
	}
	UsePSC = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ProjectileData.Feedback->VisualFX, SweepResult.ImpactPoint, OtherActor->GetActorRotation(), true);
	UGameplayStatics::SpawnSoundAttached(ProjectileData.Feedback->PickRandomSound(), OtherActor->GetRootComponent());
}
