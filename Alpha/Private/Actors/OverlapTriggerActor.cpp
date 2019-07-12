#include "OverlapTriggerActor.h"

AOverlapTriggerActor::AOverlapTriggerActor()
{
	PrimaryActorTick.bCanEverTick = false;

}

void AOverlapTriggerActor::BeginPlay()
{
	Super::BeginPlay();
}

void AOverlapTriggerActor::OnOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {

}