#include "PlayableGameInstance.h"
#include "Data/Game/GameInstanceData.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"

UGameModeData* UPlayableGameInstance::GetGameModeData()
{
	return CurrentGameModeData;
}

TMap<uint8, FMainMenuTeamStruct> UPlayableGameInstance::GetTeamData()
{
	return TeamData;
}

void UPlayableGameInstance::SetGameModeData(UGameModeData* InData)
{
	CurrentGameModeData = InData;
}

void UPlayableGameInstance::SetTeamData(TMap<uint8, FMainMenuTeamStruct> InMap)
{
	TeamData = InMap;
}
