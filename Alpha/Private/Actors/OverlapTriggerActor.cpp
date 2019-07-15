#include "OverlapTriggerActor.h"

AOverlapTriggerActor::AOverlapTriggerActor()
{
	PrimaryActorTick.bCanEverTick = false;

}

void AOverlapTriggerActor::BeginPlay()
{
	Super::BeginPlay();
}

void AOverlapTriggerActor::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {

}

void AOverlapTriggerActor::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) {

}