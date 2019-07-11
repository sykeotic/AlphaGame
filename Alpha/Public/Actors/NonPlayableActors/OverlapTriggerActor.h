#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "OverlapTriggerActor.generated.h"

UCLASS(ABSTRACT)
class AOverlapTriggerActor : public AActor
{
	GENERATED_BODY()
	
public:	
	AOverlapTriggerActor();

protected:
	virtual void BeginPlay() override;

public:	

	virtual void OnOverlap();
};
