// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Data/Combat/Effects/BaseEffectData.h"
#include "ModifyTagEffectData.generated.h"

UENUM(BlueprintType)
enum class EModifyTagType : uint8 {
	TAUNT UMETA(DisplayName = "Taunt")
};

USTRUCT(BlueprintType)
struct FModifyTagEffectDataStruct {
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		EModifyTagType Type;
};

/**
 * 
 */
UCLASS()
class ALPHA_API UModifyTagEffectData : public UBaseEffectData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FModifyTagEffectDataStruct ModifyTagEffectDataStruct;
	
};
