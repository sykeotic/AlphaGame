#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "ObjectiveOverlapActor.h"
#include "TestGameMode.generated.h"

class UGameModeData;
class ATestGameState;
class APlayerController;

UCLASS()
class ALPHA_API ATestGameMode : public AGameMode
{
	GENERATED_BODY()

public:
	void InitTeams();

protected:
	ATestGameMode();

	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly)
		bool bDisplayLogs;

	virtual void PostLogin(APlayerController* NewPlayer) override;
	virtual void StartMatch() override;

private:

	uint8 MaxPlayersPerTeam;

	TArray<APlayerController*> HumanPlayers;
	ATestGameState* CurrentGameState;
};
