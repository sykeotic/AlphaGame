#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GameInstanceData.generated.h"

class UFactionData;
class ULevelData;
class UUserWidget;
class USoundBase;

UCLASS(BlueprintType)
class ALPHA_API UGameInstanceData : public UDataAsset
{
	GENERATED_BODY()

public:

	//UPROPERTY(EditAnywhere, BlueprintReadWrite)
	//	TArray<UFactionData*> AvailableFactions;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite)
	//	TArray<ULevelData*> AvailableLevels;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite)
	//	TAssetPtr<UWorld> MainMenuLevel;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite)
	//	TArray<USoundBase*> MenuMusic;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite)
	//	TSubclassOf<UUserWidget> MainMenuWidgetClass;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite)
	//	TSubclassOf<UUserWidget> GameMenuWidgetClass;
};
