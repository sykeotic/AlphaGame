#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "TestGameMode.generated.h"

UCLASS()
class ALPHA_API ATestGameMode : public AGameMode
{
	GENERATED_BODY()

protected:
	ATestGameMode();

	virtual void BeginPlay() override;

	TArray<class APlayerController*> HumanPlayerControllerClass;

	virtual bool ReadyToStartMatch_Implementation() override;

	int32 MaxNumPlayers;

	virtual void PostLogin(APlayerController* NewPlayer) override;
};
