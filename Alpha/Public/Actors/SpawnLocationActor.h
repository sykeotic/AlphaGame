#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SpawnLocationActor.generated.h"

UCLASS()
class ALPHA_API ASpawnLocationActor : public AActor
{
	GENERATED_BODY()
	
public:	
	ASpawnLocationActor();

protected:
	virtual void BeginPlay() override;

public:	

};
