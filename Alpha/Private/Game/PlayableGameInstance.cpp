#include "PlayableGameInstance.h"
#include "Data/Game/GameInstanceData.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"

UGameModeData* UPlayableGameInstance::GetGameModeData()
{
	UE_LOG(LogTemp, Warning, TEXT("UPlayableGameInstance::GetGameModeData - Getting GameModeData"));
	return CurrentGameModeData;
}

TMap<uint8, FMainMenuTeamStruct> UPlayableGameInstance::GetTeamData()
{
	return TeamData;
}

void UPlayableGameInstance::SetPersistentID(int32 InID)
{
	PersistentID = InID;
}

int32 UPlayableGameInstance::GetPersistentID()
{
	return PersistentID;
}

void UPlayableGameInstance::SetGameModeData(UGameModeData* InData)
{
	UE_LOG(LogTemp, Warning, TEXT("UPlayableGameInstance::SetGameModeData - GameModeData Assigned"));
	CurrentGameModeData = InData;
	if (CurrentGameModeData) {
		UE_LOG(LogTemp, Warning, TEXT("UPlayableGameInstance::SetGameModeData - GameModeData OK"));
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("UPlayableGameInstance::SetGameModeData - GameModeData NULL"));
	}
}

void UPlayableGameInstance::SetTeamData(TMap<uint8, FMainMenuTeamStruct> InMap)
{
	TeamData = InMap;
}
