#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "PlayablePawn.generated.h"

UCLASS()
class ALPHA_API APlayablePawn : public APawn
{
	GENERATED_BODY()

public:
	APlayablePawn();

protected:
	virtual void BeginPlay() override;

public:
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
