#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "PlayableGameInstance.generated.h"

class UGameInstanceData;

UCLASS()
class ALPHA_API UPlayableGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UGameInstanceData* GameInstanceData;
	
};
