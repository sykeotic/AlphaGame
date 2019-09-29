#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "MainMenuGameMode.h"
#include "PlayableGameInstance.generated.h"

class UGameModeData;

UCLASS(BlueprintType)
class ALPHA_API UPlayableGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:

	UGameModeData* GetGameModeData();
	TMap<uint8, FMainMenuTeamStruct> GetTeamData();

	void SetGameModeData(UGameModeData* InData);
	void SetTeamData(TMap<uint8, FMainMenuTeamStruct> InMap);

private:

	UGameModeData* CurrentGameModeData;
	TMap<uint8, FMainMenuTeamStruct> TeamData;
};