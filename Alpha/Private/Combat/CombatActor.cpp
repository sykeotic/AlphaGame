#include "CombatActor.h"
#include "Logger.h"

ACombatActor::ACombatActor()
{
	PrimaryActorTick.bCanEverTick = false;
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	MeshComp->SetupAttachment(RootComponent);
}

void ACombatActor::OnUse() {

}

void ACombatActor::SetComponentOwner(UCombatComponent* InComp) {
	ComponentOwner = InComp;
}

UCombatComponent* ACombatActor::GetComponentOwner() {
	return ComponentOwner;
}
