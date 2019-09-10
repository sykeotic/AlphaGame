// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "FactionData.generated.h"

/**
 * 
 */
UCLASS()
class ALPHA_API UFactionData : public UDataAsset
{
	GENERATED_BODY()
	
public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<TSubclassOf<AActor*>> AvailableHeroes;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<TSubclassOf<AActor*>> AvailablePawns;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FName FactionName;
};
