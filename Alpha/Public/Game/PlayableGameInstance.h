#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Data/Game/GameInstanceData.h"
#include "PlayableGameInstance.generated.h"

UCLASS(BlueprintType)
class ALPHA_API UPlayableGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UGameInstanceData* GameInstanceData;

	virtual void Init() override;
	
};