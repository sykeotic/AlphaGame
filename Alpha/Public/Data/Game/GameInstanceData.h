#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GameInstanceData.generated.h"

class UFactionData;
class USoundBase;

UCLASS()
class ALPHA_API UGameInstanceData : public UDataAsset
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<UFactionData*> AvailableFactions;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<UWorld*> AvailableMaps;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<USoundBase*> MenuMusic;
};
