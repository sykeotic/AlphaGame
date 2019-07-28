#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "ObjectiveOverlapActor.h"
#include "TestGameMode.generated.h"

UCLASS()
class ALPHA_API ATestGameMode : public AGameMode
{
	GENERATED_BODY()

protected:
	ATestGameMode();

	virtual void BeginPlay() override;

	TArray<class APlayerController*> HumanPlayerControllerClass;

	uint32 MaxNumPlayers;

	bool MyReadyToStartMatch();

	UPROPERTY(EditDefaultsOnly)
		bool bDisplayLogs;

	virtual void PostLogin(APlayerController* NewPlayer) override;

public:
	void InitPlayers();
};
