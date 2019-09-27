// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GameModeData.generated.h"

class AAIController;

USTRUCT(BlueprintType)
struct FGameModeDataStruct : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		uint8 MaxPlayersPerTeam;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<AAIController> AIControllerClass;
};

UCLASS(BlueprintType, DefaultToInstanced, EditInlineNew)
class ALPHA_API UGameModeData : public UDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FGameModeDataStruct GameModeDataStruct;
};
