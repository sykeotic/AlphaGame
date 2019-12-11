// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GameModeData.generated.h"

class AAIController;
class AGameMode;
class UPlayerControllerData;
class UAIData;
class UFeedback;

UCLASS(BlueprintType)
class ALPHA_API UGameModeData : public UDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		uint8 MaxPlayersPerTeam;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UAIData* AIData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<AGameMode> GameModeClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UPlayerControllerData* PlayerControllerData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UFeedback* SpawnFX;
};
