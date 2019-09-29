// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GameModeData.generated.h"

class AAIController;
class AGameMode;

USTRUCT(BlueprintType)
struct FGameModeDataStruct
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		uint8 MaxPlayersPerTeam;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<AAIController> AIControllerClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<AGameMode> GameModeClass;
};

UCLASS(BlueprintType, DefaultToInstanced, EditInlineNew)
class ALPHA_API UGameModeData : public UDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FGameModeDataStruct GameModeDataStruct;
};
