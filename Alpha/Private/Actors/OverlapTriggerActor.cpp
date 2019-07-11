#include "OverlapTriggerActor.h"

AOverlapTriggerActor::AOverlapTriggerActor()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AOverlapTriggerActor::BeginPlay()
{
	Super::BeginPlay();
	
}

void AOverlapTriggerActor::OnOverlap() {

}