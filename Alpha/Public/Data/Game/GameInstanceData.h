#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Engine/DataTable.h"
#include "GameInstanceData.generated.h"

class UFactionData;
class ULevelData;
class UUserWidget;
class UPlayerControllerData;
class USoundBase;

USTRUCT(BlueprintType)
struct FGameInstanceDataStruct : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UPlayerControllerData* PlayerControllerData;
};

UCLASS(BlueprintType, DefaultToInstanced, EditInlineNew)
class ALPHA_API UGameInstanceData : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGameInstanceDataStruct GameInstanceData;
};
