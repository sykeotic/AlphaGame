#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Runtime/Engine/Classes/Components/SphereComponent.h"
#include "OverlapTriggerActor.generated.h"

UCLASS(ABSTRACT)
class AOverlapTriggerActor : public AActor
{
	GENERATED_BODY()
	
public:	
	AOverlapTriggerActor();

	virtual void OnOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

protected:
	virtual void BeginPlay() override;
};
