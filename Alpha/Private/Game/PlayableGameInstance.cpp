#include "PlayableGameInstance.h"
#include "Data/Game/GameInstanceData.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"

void UPlayableGameInstance::AssignData(UGameInstanceData* InData)
{
	GameData = InData;
}
