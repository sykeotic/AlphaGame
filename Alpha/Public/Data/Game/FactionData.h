// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "FactionData.generated.h"

class UBasePawnData;

/**
 * 
 */
UCLASS(BlueprintType, DefaultToInstanced, EditInlineNew)
class ALPHA_API UFactionData : public UDataAsset
{
	GENERATED_BODY()
	
public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<UBasePawnData*> AvailableHeroes;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<UBasePawnData*> AvailablePawns;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FName FactionName;
};
