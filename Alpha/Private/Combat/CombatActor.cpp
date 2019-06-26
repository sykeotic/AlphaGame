#include "CombatActor.h"
#include "Logger.h"

ACombatActor::ACombatActor()
{
	PrimaryActorTick.bCanEverTick = false;
	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	RootComponent = BoxComponent;

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	MeshComp->SetupAttachment(RootComponent);
}

void ACombatActor::OnUse() {

}

void ACombatActor::SetCombatComponentOwner(UCombatComponent* InComp) {
	ComponentOwner = InComp;
}

UCombatComponent* ACombatActor::GetCombatComponentOwner() {
	return ComponentOwner;
}

void ACombatActor::AssignWeaponValues(float InCooldown, UStaticMesh* InStaticMesh, FName InProjectileSpawnLocation, ERange IN_RANGE, EActorType IN_ACTOR_TYPE, float InDmg, float InRange) {
	MeshComp->SetStaticMesh(MeshComp->GetStaticMesh());
	MeshComp->SetMaterial(0, WeaponMaterial);
	MeshComp->SetStaticMesh(InStaticMesh);
	MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Damage = InDmg;
	UseRange = InRange;
	UseCooldown = InCooldown;
	ProjectileSpawnLocation = InProjectileSpawnLocation;
	RANGE_TYPE = IN_RANGE;
	ACTOR_TYPE = IN_ACTOR_TYPE;
}